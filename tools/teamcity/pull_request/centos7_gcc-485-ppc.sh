#!/bin/bash


branch=$1
build_num=$2
docker_image="multiarch/qemu-user-static:register"

docker ps 1>/dev/null 2>&1 || prefix=sudo;

${prefix} docker pull ${docker_image}
if [ "$?" != "0" ] ; then
  yes | ${prefix} docker system prune -a
fi

${prefix} docker run --rm --privileged ${docker_image} --reset
if [ "$?" != "0" ] ; then
  exit 1
fi

sh ./tools/package_docker.sh docker-bioinformatics.dockerhub.illumina.com/ppc64le/interop_centos7 ${build_num} "-DDISABLE_DYNAMIC_ARRAY=ON -DENABLE_FLOAT_COMPRESSION=OFF -DENABLE_DOCS=OFF -DENABLE_SWIG=OFF -DBUILD_SAMPLE_SHEET_ONLY=ON -DENABLE_EXAMPLES=OFF -DENABLE_APPS=OFF"
if [ "$?" != "0" ] ; then
  exit 1
fi
