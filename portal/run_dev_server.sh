cd "$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
rm ./watcher.out ./server.out .bsb.lock

echo -e "Installing npm modules"
npm i

npm run clean

echo -e "Starting the watcher in the background"
nohup npm run start > watcher.out &

echo -e "Starting the web server"
nohup npm run server > server.out &

echo -e "\n\n"
ps aux | grep npm
echo -e "\n\n"

echo -. "Starting to watch the output"
tail -f watcher.out server.out
