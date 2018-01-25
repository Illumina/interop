Using InterOp in Python      {#python_binding}
=======================

Let's learn by example. Throughout this tutorial, we'll walk you through the steps necessary to produce the metrics
in various ways found in SAV.

## Install

If you do not have the Python InterOp library installed, then you can do the following:

    $ pip install interop

Older versions (prior to 1.1.3) can be installed using:


    $ pip install -f https://github.com/Illumina/interop/releases/tag/v1.1.2 interop
    $ pip install -f https://github.com/Illumina/interop/releases/latest interop

Note, only Python versions 2.7, 3.4, 3.5 and 3.6 are currently 
supported as binary builds. Other Python versions must be built 
from source.

You can then test the install as follows:
    
    $ python -m interop --test

If you see this error:

    RuntimeError: module compiled against API version 0xb but this version of numpy is 0xa

Then upgrade numpy and try again.

## Introductory Tutorials

 * [SAV Summary Table](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_01_Intro.ipynb)
 * [Q-Score Histogram](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_02_Plot_Bar.ipynb)
 * [Low-level Extraction](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_03_Low_Level.ipynb)

