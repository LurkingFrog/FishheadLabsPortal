#!/bin/sh
# Copied from https://github.com/nodejs/docker-node/blob/master/docker-entrypoint.sh

set -e

if [ "${1#-}" != "${1}" ] || [ -z "$(command -v "${1}")" ]; then
  set -- node "$@"
fi

exec "$@"