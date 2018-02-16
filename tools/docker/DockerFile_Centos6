# sudo docker build --rm -t docker-bioinformatics.dockerhub.illumina.com/interop_centos6 -f ./tools/docker/DockerFile_Centos6 tools

FROM centos:6

ENV JAVA_HOME /usr/java/jdk1.8.0_131
ENV JUNIT_HOME /opt/junit
ENV NUNIT_DIR /opt/nunit/NUnit-2.6.4

COPY prereqs/docker-centos6-install.sh /docker-centos6-install.sh
RUN bash docker-centos6-install.sh && rm -fr docker-centos6-install.sh

