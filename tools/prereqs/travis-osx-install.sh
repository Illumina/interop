#!/usr/bin/env bash

#sw_vers

#brew update > /dev/null
#brew list
which cmake || brew unlink cmake
time brew install cmake
brew link cmake
brew install zlib
time brew install swig@3
brew unlink swig || true
brew link swig@3 --force
time brew install doxygen
brew install wget
#brew install mono
brew remove mono || true
brew install nuget
brew install coreutils || brew install gstat
#brew install openssl@1.1
brew uninstall openssl && brew install https://raw.githubusercontent.com/Homebrew/homebrew-core/8b9d6d688f483a0f33fcfc93d433de501b9c3513/Formula/openssl.rb
brew outdated pyenv || brew upgrade pyenv
brew cask list dotnet-sdk > /dev/null || time brew cask install dotnet-sdk

curl -o miniconda.sh  https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
time bash miniconda.sh -b -p $HOME/miniconda
source $HOME/miniconda/etc/profile.d/conda.sh

echo "OpenSSL: $(brew --prefix openssl)"
export PATH=$(pyenv root)/shims:${PATH}

which conda
dotnet --version

