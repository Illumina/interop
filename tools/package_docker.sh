#!/usr/bin/env bash

set -e

image=ezralanglois/interop
pyver="None"

if [ ! -z $1 ] ; then
    image=$1
fi

if [ ! -z "$2" ] ; then
    build_number=$2
fi

if [ ! -z "$3" ] ; then
    extra=$3
fi

if [ ! -e `pwd`/dist ] ; then
    mkdir `pwd`/dist
fi

sudo systemctl status docker || sudo systemctl start docker || true

docker ps 1>/dev/null 2>&1 || prefix=sudo;
#${prefix} docker rmi $(docker images --filter "dangling=true") >/dev/null 2>&1 || true
${prefix} docker rmi $(docker images) >/dev/null 2>&1 || true
${prefix} docker rm $(docker ps -a) >/dev/null 2>&1 || true
${prefix} docker pull $image
${prefix} docker run --rm -w /tmp --user `id -u`:`id -g` -v `pwd`:/src:ro -v `pwd`/dist:/dist:rw $image sh /src/tools/package.sh /src /dist teamcity OFF Release $pyver $build_number "$extra"
