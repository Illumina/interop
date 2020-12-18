#!/usr/bin/env bash

echo "Install requirements using HomeBrew"
#sw_vers

set +x

brew update #> /dev/null
#brew list
#brew list cmake > /dev/null || time brew upgrade cmake
#brew install zlib
#time brew install swig@3
#brew unlink swig || true
#brew link swig@3 --force
#time brew install doxygen
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
dotnet --version

BREW_LOCAL_BOTTLE_METADATA="$HOME/local_bottle_metadata"
# Auto cleanup can delete locally-built bottles
# when the caching logic isn't prepared for that
export HOMEBREW_NO_INSTALL_CLEANUP=1

# Don't query analytical info online on `brew info`,
#  this takes several seconds and we don't need it
# see https://docs.brew.sh/Manpage , "info formula" section
export HOMEBREW_NO_GITHUB_API=1

#Packages already installed in the current session to avoid checking them again
_BREW_ALREADY_INSTALLED='$'     #$ = illegal package name; a blank line would cause macos grep to swallow everything

function brew_cache_cleanup
{
    #Cleanup caching directories
    # Is supposed to be called in before_cache

    #Lefovers from some failure probably
    rm -f "$BREW_LOCAL_BOTTLE_METADATA"/*.tar.gz

    #`brew cleanup` may delete locally-built bottles that weren't needed this time
    # so we're saving and restoring them
    local BOTTLE_LINK BOTTLE
    for BOTTLE_LINK in "$BREW_LOCAL_BOTTLE_METADATA"/*.lnk; do
        [ -e "$BOTTLE_LINK" ] || break
        BOTTLE=$(cat "$BOTTLE_LINK")
        ln "$BOTTLE" "$BREW_LOCAL_BOTTLE_METADATA/"
    done
    brew cleanup
    local BOTTLE_BASENAME
    for BOTTLE_LINK in "$BREW_LOCAL_BOTTLE_METADATA"/*.lnk; do
        [ -e "$BOTTLE_LINK" ] || break
        BOTTLE=$(cat "$BOTTLE_LINK")
        BOTTLE_BASENAME=$(basename "$BOTTLE")
        if test ! -e "$BOTTLE"; then
            echo "Restoring: $BOTTLE_BASENAME"
            mv "$BREW_LOCAL_BOTTLE_METADATA/$BOTTLE_BASENAME" "$BOTTLE"
        else
            rm "$BREW_LOCAL_BOTTLE_METADATA/$BOTTLE_BASENAME"
        fi
    done
}

