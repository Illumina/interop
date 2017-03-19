Using InterOp in Python      {#python_binding}
=======================

Let's learn by example. Throughout this tutorial, we'll walk you through the steps necessary to produce the metrics
found in the SAV summary tab.

## Install 

If you do not have the Python InterOp library installed, then you can do one of the following:

~~~~~~~~~~~~~{.sh}
$ pip install -f https://github.com/Illumina/interop/releases/latest interop
~~~~~~~~~~~~~

## Verify Install

You can verify that InterOp is properly installed:

~~~~~~~~~~~~~{.sh}
python -m interop --version
~~~~~~~~~~~~~

You can also verify that InterOp works properly:

~~~~~~~~~~~~~{.sh}
python -m interop --test
~~~~~~~~~~~~~

## Reading Binary InterOp Files

The `run_metrics` class encapsulates the model for all the individual InterOp files as well as containing information
from the `RunInfo.xml`. The [Modules](modules.html) page contains a subset of the applications programmer's interface
for all the major classes in C++. The available Python models all have the same names (with a few exceptions) and take
the same parameters. This page is useful for accessing specific values loaded from the individual files.

~~~~~~~~~~~~~{.py}
run_metrics = py_interop_run_metrics.run_metrics()
~~~~~~~~~~~~~

The `run_metrics` class can read binary InterOp files from the disk (or alternatively from a binary buffer).

~~~~~~~~~~~~~{.py}
run_metrics.read(run_folder)
~~~~~~~~~~~~~

## Getting SAV Summary Tab-like Metrics

By default, the `run_metrics` class loads all the InterOp files. The InterOp library can provide a list of all necessary 
InterOp files for a specific application. The following shows how to generate that list for the summary statistics:

~~~~~~~~~~~~~{.py}
valid_to_load = py_interop_run.uchar_vector(py_interop_run.MetricCount, 0)
py_interop_run_metrics.list_summary_metrics_to_load(valid_to_load)
~~~~~~~~~~~~~

The `run_metrics` class can use this list to load only the required InterOp files as follows:

~~~~~~~~~~~~~{.py}
run_metrics.read(run_folder, valid_to_load)
~~~~~~~~~~~~~

The `run_summary` class encapsulates all the metrics displayed on the SAV summary tab. This class contains a tree-like
structure where metrics describing the run summary are at the root, there is a branch for each read summary, and 
a sub branch for each read/lane summary.

~~~~~~~~~~~~~{.py}
summary = py_interop_summary.run_summary()
~~~~~~~~~~~~~

The `run_summary' object can be populated from the `run_metrics` object just so:

~~~~~~~~~~~~~{.py}
py_interop_summary.summarize_run_metrics(run_metrics, summary)
~~~~~~~~~~~~~

For the following, we will take a single metric, `error_rate` and show how to access it on the three different levels:
  1. Run Summary
 ~~~~~~~~~~~~~{.py}
 print "Error rate in the total run summary:", summary.total_summary().error_rate()
 print "Error rate in the non-index run summary: ", summary.nonindex_summary().error_rate()
 ~~~~~~~~~~~~~
  2. Read Summary
 ~~~~~~~~~~~~~{.py}
 print "Error rate in read", summary.at(0).read(), "is", summary.at(0).summary().error_rate()
 ~~~~~~~~~~~~~
  3. Read/Lane Summary
 ~~~~~~~~~~~~~{.py}
 print "Mean error rate in read", summary.at(0).read()
 print "and lane", summary.at(0).at(0).lane()
 print "is", summary.at(0).at(0).error_rate().mean()
 ~~~~~~~~~~~~~

