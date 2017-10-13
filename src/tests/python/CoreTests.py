"""
Unit tests for core functionality for the SWIG binding
"""
import unittest
import numpy
import os

try:
    from interop import py_interop_run
except:
    import sys
    sys.path.append(".")
    from interop import py_interop_run

from interop import py_interop_metrics
from interop import py_interop_plot
from interop import py_interop_comm
from interop import py_interop_table
from interop import py_interop_run_metrics

class CoreTests(unittest.TestCase):
    """ Unit tests for the core functionality in the binding
    """

    def test_invalid_read_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.read(8)
            self.fail("invalid_read_exception should have been thrown")
        except py_interop_run.invalid_read_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Read number not found: 8")

    def test_invalid_tile_naming_method(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.validate()
            self.fail("invalid_tile_naming_method should have been thrown")
        except py_interop_run.invalid_tile_naming_method as ex:
            self.assertEqual(str(ex).split('\n')[0], "Unknown tile naming method")

    def test_invalid_run_info_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.validate(20, 9999, "Test")
            self.fail("invalid_run_info_exception should have been thrown")
        except py_interop_run.invalid_run_info_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Lane identifier exceeds number of lanes in RunInfo.xml for record 20_9999 in file Test - 20 > 1")

    def test_invalid_run_info_cycle_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info(py_interop_run.flowcell_layout(8, 2, 4, 99))
        try:
            run_info.validate_cycle(1, 1101, 3000, "Test")
            self.fail("invalid_run_info_cycle_exception should have been thrown")
        except py_interop_run.invalid_run_info_cycle_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Cycle number exceeds number of cycles in RunInfo.xml for record 1_1101 @ 3000 in file Test - 3000 > 0")

    def test_xml_file_not_found_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.read("file/not/found")
            self.fail("xml_file_not_found_exception should have been thrown")
        except py_interop_run.xml_file_not_found_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "cannot open file " + os.path.join("file/not/found", "RunInfo.xml"))

    def test_xml_parse_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.parse("<RunInfo></RunInfo")
            self.fail("xml_parse_exception should have been thrown")
        except py_interop_run.xml_parse_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "expected >")

    def test_empty_xml_format_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.parse("")
            self.fail("empty_xml_format_exception should have been thrown")
        except py_interop_run.empty_xml_format_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Root node not found")

    def test_missing_xml_element_exception(self):
        """
        """

        run_info = py_interop_run.info()
        try:
            run_info.parse("<RunInfo><Run><FlowcellLayout> <TileSet> </TileSet></FlowcellLayout></Run></RunInfo>")
            self.fail("missing_xml_element_exception should have been thrown")
        except py_interop_run.missing_xml_element_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Cannot find attribute: TileNamingConvention")

    def test_bad_xml_format_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        try:
            run_info.parse("<RunInfo></RunInfo>")
            self.fail("bad_xml_format_exception should have been thrown")
        except py_interop_run.bad_xml_format_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Run node not found")

    def test_index_out_of_bounds_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        TODO: flush this out for the entire model
        """

        expected_metrics = py_interop_metrics.base_error_metrics(3)
        try:
            expected_metrics.at(1)
            self.fail("index_out_of_bounds_exception should have been thrown")
        except py_interop_metrics.index_out_of_bounds_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Index out of bounds - 1 >= 0")

    def test_bad_format_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        tmp = numpy.asarray(
            [0,3
            ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
            ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
            ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120],
            dtype=numpy.uint8)
        run = py_interop_run_metrics.run_metrics()
        try:
            py_interop_comm.read_interop_from_buffer(tmp, run.extraction_metric_set())
            self.fail("bad_format_exception should have been thrown")
        except py_interop_comm.bad_format_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "No format found to parse ExtractionMetricsOut.bin with version: 0 of 2")

    def test_incomplete_file_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        tmp = numpy.asarray(
            [3,38
                ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120],
            dtype=numpy.uint8)
        run = py_interop_run_metrics.run_metrics()
        try:
            py_interop_comm.read_interop_from_buffer(tmp, run.extraction_metric_set())
            self.fail("incomplete_file_exception should have been thrown")
        except py_interop_comm.incomplete_file_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Insufficient data read from the file, got: 13 != expected: 50 for Extraction  v3")

    def test_invalid_argument(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        tmp = numpy.asarray(
            [2,38
                ,7,0,90,4,1,0,-12,-56,15,64,-98,35,12,64,0,0,0,0,0,0,0,0,46,1,17,1,0,0,0,0,96,-41,-104,36,122,-86,-46,-120
                ,7,0,-66,4,1,0,96,-43,14,64,-63,49,13,64,0,0,0,0,0,0,0,0,56,1,17,1,0,0,0,0,112,125,77,38,122,-86,-46,-120
                ,7,0,66,8,1,0,74,-68,6,64,-118,-7,8,64,0,0,0,0,0,0,0,0,93,1,46,1,0,0,0,0,-47,-104,2,40,122,-86,-46,-120],
                dtype=numpy.uint8)
        run = py_interop_run_metrics.run_metrics()
        py_interop_comm.read_interop_from_buffer(tmp, run.extraction_metric_set())
        try:
            buf = numpy.zeros(3, dtype=numpy.uint8)
            py_interop_comm.write_interop_to_buffer(run.extraction_metric_set(), buf)
            self.fail("invalid_argument should have been thrown")
        except py_interop_comm.invalid_argument as ex:
            self.assertEqual(str(ex).split('\n')[0], "Buffer size too small")

    def test_invalid_filter_option(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_info = py_interop_run.info()
        options = py_interop_plot.filter_options(py_interop_run.FourDigit)
        try:
            options.validate(py_interop_run.Intensity, run_info)
            self.fail("invalid_filter_option should have been thrown")
        except py_interop_plot.invalid_filter_option as ex:
            self.assertEqual(str(ex).split('\n')[0], "Invalid tile naming method: does not match RunInfo.xml: FourDigit != UnknownTileNamingMethod")

    def test_invalid_column_type(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        channels = py_interop_run.string_vector()
        filled = py_interop_run.bool_vector()
        columns = py_interop_table.imaging_column_vector()
        try:
            py_interop_table.create_imaging_table_columns(channels, filled, columns)
            self.fail("invalid_column_type should have been thrown")
        except py_interop_table.invalid_column_type as ex:
            self.assertEqual(str(ex).split('\n')[0], "Filled vector does not match number of column names")

    def test_invalid_parameter(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        valid_to_load = py_interop_run.uchar_vector(2)
        run_metrics = py_interop_run_metrics.run_metrics()
        try:
            run_metrics.read_metrics("", 3, valid_to_load, 1)
            self.fail("invalid_parameter should have been thrown")
        except py_interop_run_metrics.invalid_parameter as ex:
            self.assertEqual(str(ex).split('\n')[0], "Boolean array valid_to_load does not match expected number of metrics: 2 != 12")

    def test_invalid_metric_type(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        valid_to_load = py_interop_run.uchar_vector()
        try:
            py_interop_run_metrics.list_metrics_to_load("Unknown", valid_to_load)
            self.fail("invalid_metric_type should have been thrown")
        except py_interop_run_metrics.invalid_metric_type as ex:
            self.assertEqual(str(ex).split('\n')[0], "Unsupported metric type: Unknown")

    def test_invalid_channel_exception(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        run_metrics = py_interop_run_metrics.run_metrics()
        try:
            run_metrics.finalize_after_load();
            self.fail("invalid_channel_exception should have been thrown")
        except py_interop_run_metrics.invalid_channel_exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Channel names are missing from the RunInfo.xml, and RunParameters.xml does not contain sufficient information on the instrument run.")

    def test_exception_base(self):
        """
        Test that exceptions can be caught and they have the expected message
        """

        expected_metrics = py_interop_metrics.base_error_metrics(3)
        try:
            expected_metrics.at(1)
            self.fail("index_out_of_bounds_exception should have been thrown")
        except Exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "Index out of bounds - 1 >= 0")

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
        except py_interop_plot.invalid_filter_option:
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
        except py_interop_plot.invalid_filter_option: pass

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
