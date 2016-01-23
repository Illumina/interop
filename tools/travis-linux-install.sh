#!/bin/bash

lsb_release -a

sudo apt-get -qq update
sudo apt-get install -qq -y mono-devel monodevelop-nunit nunit-console

sudo apt-get install -y libgtest-dev
sudo add-apt-repository -y ppa:teward/swig3.0
sudo add-apt-repository -y ppa:beineri/opt-qt551-trusty
sudo apt-get update
sudo apt-get install -qq -y libicu-dev swig3.0 doxygen graphviz







