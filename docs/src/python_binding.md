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

Note, only Python versions 2.7, 3.6, 3.7, 3.8, 3.9 and 3.10 are currently 
supported as binary builds. Other Python versions must be built 
from source.

You can then test the install as follows:
    
    $ python -m interop --test

If you see this error:

    RuntimeError: module compiled against API version 0xb but this version of numpy is 0xa

Then upgrade numpy and try again.

## New simplified interface

      from interop import *
      ar = imaging("path/to/run_folder")

See new [interop.core](namespacecore.html) wrapper for the simplified interface


## Tips & Tricks

1. To see which methods/fields are available:

    $ dir(python_swig_object_instance)


## Introductory Tutorials

 * [SAV Summary Table](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_01_Intro.ipynb)
 * [Q-Score Histogram](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_02_Plot_Bar.ipynb)
 * [Low-level Extraction](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_03_Low_Level.ipynb)
 * [Indexing Metrics](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_04_Indexing_Metrics.ipynb)
 * [Imaging Table](https://github.com/Illumina/interop/blob/master/docs/src/Tutorial_05_Imaging_Table.ipynb)


## Introduction to the API

The following is a list of all modulates available in the Python API:

    from interop import py_interop_run
    from interop import py_interop_metrics
    from interop import py_interop_plot
    from interop import py_interop_comm
    from interop import py_interop_table
    from interop import py_interop_run_metrics

### py_interop_run

This module includes all the C++ classes, exceptions and enums available in 
    
    interop/model/run/*.h
    interop/util/xml_exceptions.h
    interop/constants/enums.h
    interop/logic/utils/enums.h
    
#### Basic Types

Many of the underlying data structures for InterOp use the `std::vector` class. These are some
basic types made available to Python.

  - string_vector
  - ulong_vector
  - ushort_vector
  - uint_vector
  - float_vector
  - bool_vector
  - uchar_vector
  - read_info_vector

#### interop/model/run

~~~~~~~~~~~~~{.py}
from interop import py_interop_run

run_info = py_interop_run.info()
run_info.read("RunFolder")

print("Number of cycles %d" % run_info.total_cycles()
~~~~~~~~~~~~~


See also:

  - [run_info](@ref run_info)
  - [run_parameters](@ref run_parameters)


#### interop/util/xml_exceptions.h


~~~~~~~~~~~~~{.py}
from interop import py_interop_run
run_info = py_interop_run.info()
try:
    run_info.read("file/not/found")
except py_interop_run.xml_file_not_found_exception as ex:
    print(ex)
~~~~~~~~~~~~~

See also:

  - [xml_exceptions](@ref xml_exceptions)


#### interop/constants/enums.h

 - py_interop_run.to_string_XX
 - py_interop_run.parse_XX

where XX can be 

 - ::illumina::interop::constants::metric_type
 - metric_group
 - tile_naming_method
 - dna_bases
 - surface_type
 - instrument_type
 - metric_base_type
 - plot_colors
 - bar_plot_options
 - metric_data
 - metric_feature_type

~~~~~~~~~~~~~{.py}
> from interop import py_interop_run
> print py_interop_run.FWHM
1
> print py_interop_run.to_string_metric_type(1)
FWHM
> print py_interop_run.parse_metric_type("FWHM")
1
> names = py_interop_run.string_vector()
> py_interop_run.list_metric_type(names)
> print [names[i] for i in range(len(names))]
['Intensity', 'FWHM', 'BasePercent', 'PercentNoCall', 'Q20Percent', 'Q30Percent', 'AccumPercentQ20', 'AccumPercentQ30', 'QScore', 'Clusters', 'ClustersPF', 'ClusterCount', 'ClusterCountPF', 'ErrorRate', 'PercentPhasing', 'PercentPrephasing', 'PercentAligned', 'Phasing', 'PrePhasing', 'CorrectedIntensity', 'CalledIntensity', 'SignalToNoise', 'OccupiedCountK', 'PercentOccupied', 'PercentPF', 'MinContrast', 'MaxContrast', 'SubtilePFPercent', 'SubtileClustersPF', 'SubtileDensityPF', 'SubtileFwhm', 'SubtileQ30Percent', 'SubtileDensity', 'SubtileClusters', 'DistortionByTile', 'EventByCycle', 'EventByType', 'MaxResidualR', 'Theta', 'ThetaZ', 'PercentDemultiplexed', 'MetricTypeCount', 'UnknownMetricType']
~~~~~~~~~~~~~

See also:

  - [constants](@ref constants)

