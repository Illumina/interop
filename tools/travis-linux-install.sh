#!/bin/bash

lsb_release -a

#sudo apt-get update -qq
sudo apt-get install -qq -y mono-complete
sudo apt-get install -qq libgtest-dev

#nuget install NUnit.Runners -Version 2.6.4 -OutputDirectory testrunner
if [ ! -e $HOME/NUnit-2.6.4 ]; then
    wget https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip
    unzip NUnit-2.6.4.zip -d $HOME
fi







