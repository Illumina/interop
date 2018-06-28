# docker build --rm -t ezralanglois/interop_centos7 -f ./tools/docker/DockerFile_Centos7 tools

FROM centos:7

ENV JAVA_HOME /usr/java/jdk1.8.0_131
ENV JUNIT_HOME /opt/junit
ENV NUNIT_DIR /opt/nunit/NUnit-2.6.4
ENV PATH="/opt/dotnet:${PATH}"

COPY prereqs/docker-centos7-install.sh /docker-centos7-install.sh
RUN bash docker-centos7-install.sh && rm -fr docker-centos7-install.sh


