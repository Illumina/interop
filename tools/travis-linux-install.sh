#!/bin/bash

lsb_release -a

sudo apt-get -qq update
sudo apt-get install -qq -y mono-devel monodevelop-nunit nunit-console
sudo apt-get install -qq -y swig





