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

export PATH=/usr/local/share/dotnet:${PATH}

if hash dotnet 2> /dev/null; then
echo ".NET Core installed"
else
wget https://download.microsoft.com/download/1/1/5/115B762D-2B41-4AF3-9A63-92D9680B9409/dotnet-sdk-2.1.4-osx-x64.pkg
sudo /usr/sbin/installer -pkg dotnet-sdk-2.1.4-osx-x64.pkg -target /
rm -fr dotnet-sdk-2.1.4-osx-x64.pkg
fi

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

dotnet clean
