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

which pyenv || brew unlink pyenv
brew install pyenv
brew upgrade pyenv
brew link pyenv

export PATH=/usr/local/share/dotnet:${PATH}

if hash dotnet 2> /dev/null; then
echo ".NET Core installed"
else
wget https://download.microsoft.com/download/1/1/5/115B762D-2B41-4AF3-9A63-92D9680B9409/dotnet-sdk-2.1.4-osx-x64.pkg
sudo /usr/sbin/installer -pkg dotnet-sdk-2.1.4-osx-x64.pkg -target /
rm -fr dotnet-sdk-2.1.4-osx-x64.pkg
fi

echo "OpenSSL: $(brew --prefix openssl)"
CPPFLAGS="-I/usr/local/opt/openssl/include"
CFLAGS="-I/usr/local/opt/openssl/include"
pyenv install 2.7.11
pyenv global 2.7.11
export PATH=$(pyenv root)/shims:${PATH}

which conda

pip install numpy
pip install wheel
pip install delocate

dotnet --version
