#!/usr/bin/env bash

sw_vers

brew update
brew list
brew unlink cmake
brew install cmake
brew install https://s3.amazonaws.com/travisbuilds.swig.org/mono.rb

# add swig
# add nunit

