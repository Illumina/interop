#!/usr/bin/env bash

#sw_vers

#brew update > /dev/null
#brew list
set +x
brew list cmake > /dev/null || time brew upgrade cmake
brew install zlib
time brew install swig@3
brew unlink swig || true
brew link swig@3 --force
time brew install doxygen
brew install wget
brew install nuget
brew install coreutils || brew install gstat

brew tap isen-ng/dotnet-sdk-versions
brew cask list dotnet-sdk > /dev/null || time brew cask install dotnet-sdk2-2-400
#brew cask list dotnet-sdk > /dev/null || time brew cask install dotnet-sdk

time curl -o miniconda.sh  https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
time bash miniconda.sh -b -p $HOME/miniconda
source $HOME/miniconda/etc/profile.d/conda.sh

echo "OpenSSL: $(brew --prefix openssl)"
export PATH=$(pyenv root)/shims:${PATH}

which conda
dotnet --version

