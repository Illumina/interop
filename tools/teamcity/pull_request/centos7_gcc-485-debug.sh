#!/bin/bash

build_number=$1
sh ./tools/package_docker.sh docker-bioinformatics.dockerhub.illumina.com/interop_centos7 "${build_number}" "-DENABLE_VERACODE=ON" Debug


