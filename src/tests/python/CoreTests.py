"""
Unit tests for core functionality for the SWIG binding
"""
import unittest
import numpy
import py_interop_run
import py_interop_metrics
import py_interop_plot
import py_interop_comm
import py_interop_table
import py_interop_run_metrics

class CoreTests(unittest.TestCase):
    """ Unit tests for the core functionality in the binding
    """

    def test_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        expected_metrics = py_interop_metrics.base_error_metrics(3)
        try:
            expected_metrics.at(1)
            self.fail("index_out_of_bounds_exception should have been thrown")
        except py_interop_run.index_out_of_bounds_exception, ex:
            self.assertEqual(str(ex).split('\n')[0], "Index out of bounds")

    def test_exception_base(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        expected_metrics = py_interop_metrics.base_error_metrics(3)
        try:
            expected_metrics.at(1)
            self.fail("index_out_of_bounds_exception should have been thrown")
        except Exception, ex:
            self.assertEqual(str(ex).split('\n')[0], "Index out of bounds")

    def test_run_metrics_typedef_wrapping(self):
        """
        Test that the metric typedefs in run_metrics are properly wrapped
        """

        run_metrics = py_interop_run_metrics.run_metrics()
        metric_set_getters = [method for method in dir(run_metrics) if method.endswith('metric_set') and not method.startswith('set_')]
        for getter in metric_set_getters:
            metric_set = getattr(run_metrics, getter)()
            self.assertTrue(hasattr(metric_set, 'size'))
            self.assertEqual(metric_set.size(),0)

    def test_plot_flowcell_map(self):
        """
        Test that plot_flowcell_map is properly wrapped
        """

        run = py_interop_run_metrics.run_metrics()
        options = py_interop_plot.filter_options(run.run_info().flowcell().naming_method())
        bufferSize = py_interop_plot.calculate_flowcell_buffer_size(run, options)
        dataBuffer = numpy.zeros(bufferSize, dtype=numpy.float32)
        idBuffer = numpy.zeros(bufferSize, dtype=numpy.uint32)
        data = py_interop_plot.flowcell_data()
        try:
            py_interop_plot.plot_flowcell_map2(run, py_interop_run.Intensity, options, data, dataBuffer, idBuffer)
        except py_interop_run.invalid_filter_option:
            pass

    def test_plot_qscore_heatmap(self):
        """
        Test that plot_flowcell_map is properly wrapped
        """

        run = py_interop_run_metrics.run_metrics()
        options = py_interop_plot.filter_options(run.run_info().flowcell().naming_method())
        rows = py_interop_plot.count_rows_for_heatmap(run)
        cols = py_interop_plot.count_columns_for_heatmap(run)
        dataBuffer = numpy.zeros((rows, cols), dtype=numpy.float32)
        data = py_interop_plot.heatmap_data()
        try:
            py_interop_plot.plot_qscore_heatmap(run, options, data, dataBuffer.ravel())
        except py_interop_run.invalid_filter_option: pass

    def test_list_by_cycle_metrics(self):
        """
        Test if list_by_cycle_metrics matches expected type
        """

        typeList = py_interop_metrics.metric_type_description_vector()
        py_interop_plot.list_by_cycle_metrics(typeList, False)
        self.assertEqual(typeList[0].value(), py_interop_run.Intensity)

    def test_populate_imaging_table(self):
        """
        Test if imaging logic can be properly used
        """

        tmp = numpy.asarray([2,38
            ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
            ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
            ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120],
                            dtype=numpy.uint8)
        run = py_interop_run_metrics.run_metrics()
        py_interop_comm.read_interop_from_buffer(tmp, run.extraction_metric_set())
        self.assertEqual(run.extraction_metric_set().size(), 3)

        reads = py_interop_run.read_info_vector()
        reads.append(py_interop_run.read_info(1, 1, 26))
        reads.append(py_interop_run.read_info(2, 27, 76))
        run.run_info(py_interop_run.info(
            py_interop_run.flowcell_layout(2, 2, 2, 16),
            reads
        ))
        run.legacy_channel_update(py_interop_run.HiSeq)
        columns = py_interop_table.imaging_column_vector()
        py_interop_table.create_imaging_table_columns(run, columns)
        row_offsets = py_interop_table.map_id_offset()
        py_interop_table.count_table_rows(run, row_offsets)
        column_count = py_interop_table.count_table_columns(columns)
        data = numpy.zeros((len(row_offsets), column_count), dtype=numpy.float32)
        py_interop_table.populate_imaging_table_data(run, columns, row_offsets, data.ravel())
        self.assertEqual(data[0, 0], 7)

    def test_count_imaging_table_columns(self):
        """
        Test if imaging logic is properly wrapped
        """

        columns = py_interop_table.imaging_column_vector()
        self.assertEqual(py_interop_table.count_table_columns(columns), 0)

    def test_to_metric_feature(self):
        """
        Test if imaging logic is properly wrapped
        """

        metric_type = py_interop_run.Intensity
        self.assertEqual(py_interop_run_metrics.to_feature(metric_type), 18)

    def test_run_metrics_member(self):
        """
        Test if the run metrics object is properly wrapped
        """

        run_metrics = py_interop_run_metrics.run_metrics()
        self.assertEqual(run_metrics.extraction_metric_set().size(), 0)

    def test_parse_metric_type(self):
        """
        Test if the enum parsing is properly wrapped
        """

        self.assertEqual(py_interop_run.parse_metric_type("Intensity"), py_interop_run.Intensity)

    def test_to_string_metric_type(self):
        """
        Test if the enum to_string is properly wrapped
        """

        self.assertEqual(py_interop_run.to_string_metric_type(py_interop_run.Intensity), "Intensity")

    def test_to_string_metric_group(self):
        """
        Test if the enum to_string is properly wrapped
        """

        self.assertEqual(py_interop_run.to_string_metric_group(py_interop_run.Error), "Error")


if __name__ == '__main__':
    unittest.main()
