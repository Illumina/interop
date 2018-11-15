# sudo docker build --rm -t docker-bioinformatics.dockerhub.illumina.com/interop_centos5 -f ./tools/docker/DockerFile_Centos5 tools

FROM quay.io/pypa/manylinux1_x86_64

ENV JAVA_HOME /usr/java/jdk1.8.0_131
ENV JUNIT_HOME /opt/junit
ENV NUNIT_DIR /opt/nunit/NUnit-2.6.4

COPY prereqs/docker-centos5-install.sh /docker-centos5-install.sh
COPY patches/mono_patch.txt /mono_patch.txt
RUN bash docker-centos5-install.sh && rm -fr docker-centos5-install.sh


