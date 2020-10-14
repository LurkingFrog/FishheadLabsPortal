#! /usr/bin/env zsh


export CLICOLOR_FORCE=1
export RUST_BACKTRACE=1;
export RUST_LOG=warn,test=debug,cache=debug,client=debug,server=debug;
export USER_ID=$(id -u):$(id -g)
# export RUSTFLAGS="-Z macro-backtrace -Z debug-macros"


# This is the root of the project. It gets mounted wholesale in the development docker environment
export WORKDIR=/home/dfogelson/Foundry/FishheadLabsPortal


# An easier to read variable for calling docker compose. We'll set this programmatically
COMPOSE_FILE=$WORKDIR/dev.docker-compose.yml
COMPOSE=
LOGS_PID=

#RipGrep is used to parse files - easier to extract data from the compose files
RG=

# Standard PSQL calls to the postgres database. Set when we start Postgres
PSQL=
PG_USER=
PG_PASS=


# Some project groupings. These are used so I can figure out the correct containers to manage
ALL=('cache' 'postgres' 'google_sheets' 'client' 'portal' 'server' 'External/Gappi/' 'External/Wrapi' 'External/Subpar')
SERVER="cache postgres server sheets External"
CLIENT="cache client portal"


# If Diesel is missing on ubuntu:
#> sudo apt install libpq-dev
#> sudo ln -s /usr/lib/x86_64-linux-gnu/libzmq.so.5 /usr/lib/x86_64-linux-gnu/libpq.so
#> cargo install diesel_cli --no-default-features --features postgres --verbose

SEP="\n\n<---------------------->\n\n"

# Ensure we have a value of docker compose
function get_docker_compose {
  echo -e "\n\tFinding Docker Compose"
  COMPOSE=($(command -v docker-compose) -f $COMPOSE_FILE)
  if [ $? -ne 0 ]; then
    echo -e "\tDocker-compose not installed: Attempting to install it now";
    sudo curl \
      -L "https://github.com/docker/compose/releases/download/1.26.2/docker-compose-$(uname -s)-$(uname -m)" \
      -o /usr/local/bin/docker-compose
    sudo chmod 755 /usr/local/bin/docker-compose


    COMPOSE=($(command -v docker-compose) -f $COMPOSE_FILE)
    if [ $? -ne 0 ]; then
      echo -e "\tFailed to get docker-compose"
      exit $?
    fi
  fi
  echo -e "\tFinished installing docker-compose."
  echo -e "\tUsing docker-compose command $COMPOSE"
}

function get_ripgrep {
  echo -e "${SEP}getting ripgrep"
  RG=$(command -v rg)
  if [ $? -ne 0 ]; then
    echo -e "\tRipgrep not installed: Attempting to install it now";
    $(sudo apt install ripgrep)

    RG=$(command -v rg)
    if [ $? -ne 0 ]; then
      echo -e "\tFailed to get ripgrep"
      exit $?
    fi
  fi
  echo -e "\tFinished installing ripgrep."
  echo -e "\tUsing ripgrep command $RG"
}


# Simple function to make sure the postgres container is running before anything else
function wait_postgres {
  AWAKE=0
  while [ $AWAKE -lt 60 ]; do
    # Query Postgres

    RESULT=$($PSQL -c 'SELECT 1')
    if [[ $RESULT =~ '1 row' ]]; then
      AWAKE=3000
      echo -e "\tPostgres is now awake and running"
    else
      echo -e "\tNot awake yet. Sleeping again.  Time waiting: $AWAKE"
      AWAKE=$(($AWAKE+1))
      sleep 1
    fi
  done
}


function reset_db {
  echo -e "$SEP"
  # FIXME: If this is changed in the file but the persistent data volume isn't destroyed, this breaks.
  PG_PASS=$(IFS=":" rg "POSTGRES_PASSWORD:\s*(.+)\s*\$" -or '\$1' $COMPOSE_FILE | read RESULT; echo $RESULT)
  PG_PORT=$(IFS=":" rg "\-\s*(\d+)\s*:\s*5432\s*\$" -or '\$1' $COMPOSE_FILE | read RESULT; echo $RESULT)

  # HACK: Very brittle here, as I'm not encoding anything so an @ in the password will break it
  DB_URL=postgres://${PGUSER:-postgres}:${PG_PASS:-password}@localhost:${PG_PORT:-5432}/headless
  echo -e "\tSetting Postgres connection string to: '$DB_URL'"
  echo -e "DATABASE_URL=$DB_URL" > $WORKDIR/.env

  echo -e "\tResetting the DB if it exists"
  diesel database reset --database-url $DB_URL

  echo -e "\tAnd running the DB setup with migrations"
  diesel setup --database-url $DB_URL \
  && diesel migration run --database-url $DB_URL \
  && diesel migration redo --database-url $DB_URL
  diesel migration list --database-url $DB_URL
}


function rebuild_project {
  echo -e "${SEP}Building and running the full test\n"
  # cargo run && build_docs
  cargo test --package test_suite test_replicant_full -- --nocapture \
  2> >(sed '/ \(\(backtrace::\)\|\(core::\)\|\(<\?std::\)\|\(rust_begin_unwind\)\)/, +1d') \
  && build_docs
  # cargo run -p diesel_updates \
#
  echo -e "\n"
}


function pg_init {
  echo -e "\n\tInitializing postgres container"
  $COMPOSE up -d --remove-orphans postgres

  PSQL=($COMPOSE exec -T postgres psql -U postgres)
  wait_postgres
  reset_db
}



# function build_book {
#   echo -e "\nBuilding the book"
#   cd ./the_process_foundry/book
#   mdbook clean \
#   && mdbook build . \
#   && cp -R ./build/* ~/Foundry/lurkingfrog.github.io/the_process_foundry/book
#   cd ../..
# }

function build_docs {
  echo -e "\nBuilding the documentation"
  cargo doc --no-deps \
  && mkdir -p  ~/Foundry/lurkingfrog.github.io/protean/docs \
  && cp -R target/doc/* ~/Foundry/lurkingfrog.github.io/protean/docs
}

function rebuild_compose {
    echo -e "${SEP}Rebuilding Docker Compose"
    $COMPOSE down
    # $COMPOSE up -d --remove-orphans postgres
    wait_postgres
    reset_db
}

# Remove all the docker containers before exiting
function tearDown {
  echo -e "All done, tearing down"
  #/usr/bin/docker-compose -f scripts/docker/dev.docker-compose.yml down
}

# See if a term is contained in a list
function isIn() [[ "$2" =~ "\b$1\b" ]]


function restart_service() {
  echo -e "(Re)starting Service $1"

  # Cleanup ended all docker processes
  EXITED=$($COMPOSE ps -a | grep Exit | cut -d ' ' -f 1 )
  if [[ $EXITED != "" ]]; then
    xargs sudo docker rm $EXITED
  fi

  COUNT=$($COMPOSE ps | grep "_$1_" | wc -l)
  if [ $COUNT = 1 ]; then
    $COMPOSE restart $1
  else
    $COMPOSE up -d --remove-orphans $1
  fi

  sleep 1
}


# Run the web portal locally. Mounting and running in docker seems to cause a conflict with VS Code, so
# doing it manually for now
function restart_portal() {
  echo -e "(Re)starting  Web Portal"
  cd $WORKDIR/portal

  if [[ $PORTAL_SERVER_PID != "" ]]; then
    echo -e "Killing the portal server with pid ${PORTAL_SERVER_PID}"
    kill ${PORTAL_SERVER_PID}
  fi

  if [[ $PORTAL_COMPILER_PID != "" ]]; then
    echo -e "Killing the portal compiler with pid ${PORTAL_COMPILER_PID}"
    kill ${PORTAL_COMPILER_PID}
  fi

  # Kill anything that looks like it was running from this directory
  for x in $(ps aux | grep $WORKDIR | head -n-1 | sed -E "s|^(\w+)\+\s+([0-9]+).+\$|\2|"); do \
    echo -e "Killing pid $x"; \
    kill $x; \
  done

  rm -f .bsb.lock
  npm run clean
  npm run build

  if [ $? -ne 0 ]; then
    echo -e "Got a compiler error on the portal. Not starting the server"
  else
    npm run start &
    PORTAL_COMPILER_PID=$!

    npm run server &
    PORTAL_SERVER_PID=$!
  fi
  cd $WORKDIR
}

function test_server() {
  echo -e "${SEP}Running Test on server"
  $COMPOSE ps -a | grep Exit | cut -d ' ' -f 1 | xargs sudo docker rm
  $COMPOSE run -e RUST_LOG=${RUST_LOG} $1 cargo test -- --nocapture
  $COMPOSE logs -f $1 &
  sleep 2
}

function print_logging() {
  echo -e "\n\n\n   ---->  Printing the logging";
  if [[ $LOGS_PID != "" ]]; then
    echo -e "We have a PID: $LOGS_PID"
    kill $LOGS_PID
  fi
  $COMPOSE logs -f web_compiler &
  LOGS_PID=$!
}

function init {
  echo -e "${SEP}Running initialization"

  get_docker_compose
  get_ripgrep

  # pg_init
  # restart_service server
  $COMPOSE down

  # Clean up some holdovers
  sudo chown -R ${USER}:${USER} $WORKDIR
  cd ./portal
  npm run clean
  npm i
  cd ..


  restart_portal
  print_logging

  mkdir -p $WORKDIR/pdfs
}

# ----------------------------------------------------------
#
#                      Main Function
#
# ----------------------------------------------------------

space=" "
modify="${space}MODIFY${space}"
INIT_DIR=$(pwd)

cd $WORKDIR

# Initialize items like docker compose
init

# And run it the first time before the loop so we don't have to wait for the update
# rebuild_project

while true; do
  command -v inotifywait > /dev/null 2>&1 || $(echo -e "InotifyWait not installed" && exit 1)
  EVENT=$(inotifywait -q -r --exclude target -e modify \
    $WORKDIR/watcher.sh \
    $WORKDIR/Cargo.toml \
    $(for x in $ALL; do echo -e "$WORKDIR/$x\n"; done) \
  )

  FILE_PATH=${EVENT/${modify}/}
  PROJECT=
  [[ $FILE_PATH =~ "^$INIT_DIR/([^/]+?)/" ]] && PROJECT=${match[1]}

  # Root cases
  if [[ $PROJECT == "" ]]; then
    echo -e "Empty Project match, '$PROJECT'"
    if [[ $FILE_PATH =~ ".+/watcher.sh$" ]]; then
      echo -e "Matched Watcher.sh. Exiting so we can restart"
      tearDown
      sleep 1
      exit 0


    elif [[ $FILE_PATH =~ ".?/Cargo.toml$" ]]; then
      # Give it time, since VSCode conflicts and wants to do the same
      sleep 5

      # Restart all the projects if we changed the workspace
      rebuild_project

    elif [[ $FILE_PATH =~ "^.?/.+.docker-compose.yml$" ]]; then
      rebuild_compose

    fi

  elif isIn $PROJECT $SERVER; then
    if [[ $FILE_PATH =~ ".?/Cargo.toml$" ]]; then
      # restart_service server
      test_server server

    elif [[ $FILE_PATH =~ "^.?/.+.rs$" ]]; then
      # restart_service server
      test_server server

    elif [[ $FILE_PATH =~ "^.?/.+.sql$" ]]; then
      reset_db

    elif [[ $FILE_PATH =~ "^.?/.+.docker-compose.yml$" ]]; then
      rebuild_compose

    else
      echo -en "No Match on '${FILE_PATH}'': Continuing\n"
    fi
    print_logging

  elif isIn $PROJECT $CLIENT; then
    if [[ $FILE_PATH =~ ".?/package.json$" ]]; then
      cd $WORKDIR/portal
      npm i
      restart_portal

    elif [[ $FILE_PATH =~ ".?/bsconfig.json$" ]]; then
      restart_portal

    elif [[ $FILE_PATH =~ ".?/webpack.config.js$" ]]; then
      restart_portal
    # elif [[ $FILE_PATH =~ "^.?/.+.re$" ]]; then
      # Doing nothing, as watch should handle this

    #   echo ""

    # elif [[ $FILE_PATH =~ "^.?/.+.html$" ]]; then
    #   # Doing nothing, as watch should handle this
    #   echo ""
    # elif [[ $FILE_PATH =~ "^.?/.+\.[rei?|resi?|html|css]$" ]]; then
    # print_logging
    # sleep 2

    fi
  fi
  # Changes don't need to be that fast, so give it a break in between
  sleep 2
done
