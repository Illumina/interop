#!/usr/bin/env bash

sw_vers

brew update > /dev/null
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
brew install openssl@1.1
brew outdated pyenv || brew upgrade pyenv
brew cask install dotnet-sdk
brew install readline
brew install openssl

echo "OpenSSL: $(brew --prefix openssl)"
CFLAGS="-I$(brew --prefix openssl)/include -I$(xcrun --show-sdk-path)/usr/include"
LDFLAGS="-L$(brew --prefix openssl)/lib"
pyenv install 2.7.12
pyenv global 2.7.12
export PATH=$(pyenv root)/shims:${PATH}

which conda

pip install numpy
pip install wheel
pip install delocate

dotnet --version

