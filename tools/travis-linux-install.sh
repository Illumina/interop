#!/bin/bash

lsb_release -a

# How To: https://conda.io/docs/travis.html
wget https://repo.continuum.io/miniconda/Miniconda2-latest-Linux-x86_64.sh -O miniconda.sh;
bash miniconda.sh -b -p $HOME/miniconda
export PATH="$HOME/miniconda/bin:$PATH"
hash -r
conda config --set always_yes yes --set changeps1 no
conda update -q conda
conda install numpy
conda info -a

if [ ! -e $HOME/NUnit-2.6.4/bin/nunit.framework.dll ]; then
    travis_retry wget https://github.com/nunit/nunitv2/releases/download/2.6.4/NUnit-2.6.4.zip
    unzip NUnit-2.6.4.zip -d $HOME
fi





