Using InterOp in Python      {#python_binding}
=======================

Let's learn by example. Throughout this tutorial, we'll walk you through the steps necessary to produce the metrics
in various ways found in SAV.

## Install

If you do not have the Python InterOp library installed, then you can do the following:

    $ pip install -f https://github.com/Illumina/interop/releases/latest interop

Note, only Python versions 2.7 (UCS-4), 3.5 and 3.6 are currently 
supported as binary builds. Other Python versions must be built 
from source.

If you are new to Python or unsure what UCS-4 means, then please use
Anaconda or Miniconda.

You can then test the install as follows:
    
    $ python -m interop --test

## Introductory Tutorials

 * [SAV Summary Table](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_01_Intro.ipynb)

