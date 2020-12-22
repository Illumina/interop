#!/usr/bin/env bash

echo "Install requirements using HomeBrew"
export HOMEBREW_NO_INSTALL_CLEANUP=1
export HOMEBREW_NO_AUTO_UPDATE=1 # Fix

set -x

brew list swig@3 || brew install swig@3
brew unlink swig || true
brew link swig@3 --force
brew install doxygen
brew install nuget

brew tap isen-ng/dotnet-sdk-versions
brew cask list dotnet-sdk || brew cask install dotnet-sdk2-2-400

brew list miniconda

set +x
echo "Install Anaconda"
set -x
time curl -o miniconda.sh  https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
time bash miniconda.sh -b -p $HOME/miniconda
source $HOME/miniconda/etc/profile.d/conda.sh

which conda
dotnet --version

set +x
echo "Finished requirements"



