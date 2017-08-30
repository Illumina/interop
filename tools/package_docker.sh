#!/usr/bin/env bash

set -e

image=ezralanglois/interop

if [ ! -e `pwd`/dist ] ; then
    mkdir `pwd`/dist
fi

docker ps 1>/dev/null 2>&1 || prefix=sudo;
${prefix} docker rmi $(docker images --filter "dangling=true") >/dev/null 2>&1 || true
${prefix} docker rm $(docker ps -a) >/dev/null 2>&1 || true
${prefix} docker pull $image
${prefix} docker run --rm -w /tmp --user `id -u`:`id -g` -v `pwd`:/src:ro -v `pwd`/dist:/dist:rw $image sh /src/tools/package.sh /src /dist teamcity OFF Release
