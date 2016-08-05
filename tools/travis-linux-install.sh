#!/bin/bash

lsb_release -a

if [ ! -e $HOME/NUnit-2.6.4/bin/nunit.framework.dll ]; then
    wget https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip
    unzip NUnit-2.6.4.zip -d $HOME
fi





