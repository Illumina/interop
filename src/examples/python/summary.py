"""
This example application displays the mean error rate averaged over all lanes for each read
"""


# @ [Reporting Summary Metrics in Python]

from interop import py_interop_run_metrics
from interop import py_interop_summary
from interop import py_interop_run
import numpy
import logging
import sys
import os


def main():
    """ Retrieve run folder paths from the command line
    Ensure only metrics required for summary are loaded
    Load the run metrics
    Calculate the summary metrics
    Display error by lane, read
    """
    logging.basicConfig(level=logging.INFO)

    run_metrics = py_interop_run_metrics.run_metrics()
    summary = py_interop_summary.run_summary()

    valid_to_load = py_interop_run.uchar_vector(py_interop_run.MetricCount, 0)
    py_interop_run_metrics.list_summary_metrics_to_load(valid_to_load)

    for run_folder_path in sys.argv[1:]:
        run_folder = os.path.basename(run_folder_path)
        try:
            run_metrics.read(run_folder_path, valid_to_load)
        except Exception, ex:
            logging.warn("Skipping - cannot read RunInfo.xml: %s - %s"%(run_folder, str(ex)))
            continue
        py_interop_summary.summarize_run_metrics(run_metrics, summary)
        error_rate_read_lane_surface = numpy.zeros((summary.size(), summary.lane_count(), summary.surface_count()))
        for read_index in xrange(summary.size()):
            for lane_index in xrange(summary.lane_count()):
                for surface_index in xrange(summary.surface_count()):
                    error_rate_read_lane_surface[read_index, lane_index, surface_index] = \
                        summary.at(read_index).at(lane_index).at(surface_index).error_rate().mean()
        logging.info("Run Folder: "+run_folder)
        for read_index in xrange(summary.size()):
            read_summary = summary.at(read_index)
            logging.info("Read "+str(read_summary.read().number())+" - Top Surface Mean Error: "+str(error_rate_read_lane_surface[read_index, :, 0].mean()))

if __name__ == '__main__':
    main()

# @ [Reporting Summary Metrics in Python]
