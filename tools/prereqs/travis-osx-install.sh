#!/usr/bin/env bash

#sw_vers

#brew update > /dev/null
#brew list
which cmake || brew unlink cmake
brew install cmake
brew link cmake
brew install zlib
brew install swig@3
brew unlink swig || true
brew link swig@3 --force
brew install doxygen
brew install wget
#brew install mono
brew remove mono || true
brew install nuget
brew install coreutils || brew install gstat
#brew install openssl@1.1
brew uninstall openssl && brew install https://raw.githubusercontent.com/Homebrew/homebrew-core/8b9d6d688f483a0f33fcfc93d433de501b9c3513/Formula/openssl.rb
brew outdated pyenv || brew upgrade pyenv
brew cask list dotnet-sdk > /dev/null || brew cask install dotnet-sdk
brew list readline > /dev/null || brew install readline
brew list xz > /dev/null || brew install xz

curl -o miniconda.sh  https://repo.anaconda.com/miniconda/Miniconda3-latest-MacOSX-x86_64.sh
bash miniconda.sh -b -p $HOME/miniconda
source $HOME/miniconda/etc/profile.d/conda.sh

echo "OpenSSL: $(brew --prefix openssl)"
export PATH=$(pyenv root)/shims:${PATH}

which conda
dotnet --version

