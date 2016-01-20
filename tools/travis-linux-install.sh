#!/bin/bash

lsb_release -a

sudo apt-get -qq update
sudo apt-get -qq install mono-devel monodevelop-nunit nunit-console
sudo apt-get -qq install swig



