#!/usr/bin/env bash

echo "Install requirements using HomeBrew"
#sw_vers

set +x

brew update #> /dev/null
#brew list
#brew list cmake > /dev/null || time brew upgrade cmake
#brew list zlib || brew install zlib
brew list swig@3 || brew install swig@3
brew unlink swig || true
brew link swig@3 --force
#brew install doxygen
#brew install wget
#brew install nuget
#brew install coreutils || brew install gstat

#brew tap isen-ng/dotnet-sdk-versions
#brew cask list dotnet-sdk > /dev/null || time brew cask install dotnet-sdk2-2-400

time curl -o miniconda.sh  https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
time bash miniconda.sh -b -p $HOME/miniconda
source $HOME/miniconda/etc/profile.d/conda.sh

echo "OpenSSL: $(brew --prefix openssl)"
export PATH=$(pyenv root)/shims:${PATH}

which conda
#dotnet --version



