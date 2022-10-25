"""@package interop         {#interop_core}
Core routines to simplify using the InterOp Library

InterOp is built around a single data structure alled a `run_metrics` object. This contains the full set of InterOps
along with the RunInfo.xml and some of the RunParameters.xml.

A run metrics object can be read in as follows:
>>> from interop import read
>>> run_metrics = read("some/path/run_folder_name") # doctest: +SKIP

Core routines take the run_metrics object and convert it into a table represented by a structured NumPy array. This can,
in turn, be converted to a pandas DataFrame or any other data structure.

The core routines include the following:

>>> from interop import index_summary
>>> index_summary(run_metrics_with_indexing)
array([(1, 0.4556, 1015.5555, 520.6667, 1536.2222, 1800., 2000.)],
      dtype=[('Lane', '<u2'), ('Mapped Reads Cv', '<f4'), ('Max Mapped Reads', '<f4'), ('Min Mapped Reads', '<f4'), ('Total Fraction Mapped Reads', '<f4'), ('Total Pf Reads', '<f4'), ('Total Reads', '<f4')])

>>> from interop import summary
>>> summary(run_metrics_example)
array([(0.36666667, 6.6666665, 0., 0., 0.)],
      dtype=[('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4')])

>>> from interop import indexing
>>> indexing(run_metrics_with_indexing)
array([(1., 1101., 'ATCACGAC-AAGGTTCA', '1', 4570., 900., 507.77777),
       (1., 1101., 'ATCACGAC-GGGGGGGG', '2', 2343., 900., 260.33334),
       (1., 1102., 'ATCACGAC-AAGGTTCA', '1', 4570.,   0.,   0.     ),
       (1., 1102., 'ATCACGAC-GGGGGGGG', '2', 2343.,   0.,   0.     )],
      dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Barcode', 'O'), ('SampleID', 'O'), ('Cluster Count', '<f4'), ('Cluster Count PF', '<f4'), ('% Demux', '<f4')])

>>> from interop import imaging
>>> imaging(run_metrics_example)
rec.array([(1., 1101., 1., 1., 1., 0.1, 10., 10., 25. , 33.3, 33.3, 33.3, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
           (1., 1101., 2., 1., 2., 0.2,  5., 15., 12.5, 42.9, 28.6, 28.6, 0.,  5., 15., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
           (1., 1101., 3., 1., 3., 0.3, 10., 10., 25. , 33.3, 50. , 16.7, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
           (1., 1101., 4., 2., 1., 0.4, 10.,  5., 25. , 16.7, 50. , 33.3, 0., 10.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
           (1., 1101., 5., 3., 1., 0.5, 15.,  5., 37.5, 20. , 40. , 40. , 0., 15.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.)],
          dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Cycle', '<f4'), ('Read', '<f4'), ('Cycle Within Read', '<f4'), ('Error Rate', '<f4'), ('P90/green', '<f4'), ('P90/blue', '<f4'), ('% No Calls', '<f4'), ('% Base/A', '<f4'), ('% Base/C', '<f4'), ('% Base/G', '<f4'), ('% Base/T', '<f4'), ('Fwhm/green', '<f4'), ('Fwhm/blue', '<f4'), ('Corrected/A', '<f4'), ('Corrected/C', '<f4'), ('Corrected/G', '<f4'), ('Corrected/T', '<f4'), ('Called/A', '<f4'), ('Called/C', '<f4'), ('Called/G', '<f4'), ('Called/T', '<f4'), ('Signal To Noise', '<f4'), ('Surface', '<f4'), ('Swath', '<f4'), ('Tile Number', '<f4')])

Any of the core routines above can take a `run_metrics` object or a string containing a file path to a valid run folder.

>>> ar = index_summary("some/path/run_folder_name") # doctest: +SKIP

The structured NumPy array can be converted to a Pandas DataFrame just so:

>>> import pandas as pd # doctest: +SKIP
>>> df = pd.DataFrame(ar) # doctest: +SKIP

For more information see the documentation around each function below.
"""

import interop.py_interop_run_metrics as interop_metrics
import interop.py_interop_metrics as interop_metric_sets
import interop.py_interop_run as interop_run
import interop.py_interop_table as interop_table
import interop.py_interop_summary as interop_summary
import numpy as np
import os

_summary_levels = ('Total', 'NonIndex', 'Read', 'Lane', 'Surface')
_index_summary_levels = ('Lane', 'Barcode')


def index_summary(run_metrics, level='Lane', columns=None, dtype='f4', **extra):
    """ Index summary table

    >>> from interop import index_summary
    >>> ar = index_summary("some/path/run_folder_name") # doctest: +SKIP

    >>> index_summary(run_metrics_with_indexing)
    array([(1, 0.4556, 1015.5555, 520.6667, 1536.2222, 1800., 2000.)],
          dtype=[('Lane', '<u2'), ('Mapped Reads Cv', '<f4'), ('Max Mapped Reads', '<f4'), ('Min Mapped Reads', '<f4'), ('Total Fraction Mapped Reads', '<f4'), ('Total Pf Reads', '<f4'), ('Total Reads', '<f4')])

    >>> index_summary(run_metrics_with_indexing, level='Barcode')
    array([(1, 18280., 1015.5555, 1., 'ATCACGAC', 'AAGGTTCA', 'TSCAIndexes', '1'),
           (1,  9372.,  520.6667, 2., 'ATCACGAC', 'GGGGGGGG', 'TSCAIndexes', '2')],
          dtype=[('Lane', '<u2'), ('Cluster Count', '<f4'), ('Fraction Mapped', '<f4'), ('Id', '<f4'), ('Index1', 'O'), ('Index2', 'O'), ('Project Name', 'O'), ('Sample Id', 'O')])

    >>> index_summary(run_metrics_with_indexing, columns=['Total Fraction Mapped Reads'])
    array([(1, 1536.2222)],
          dtype=[('Lane', '<u2'), ('Total Fraction Mapped Reads', '<f4')])

    >>> index_summary(run_metrics_with_indexing, columns=['Incorrect'])
    Traceback (most recent call last):
    ...
    ValueError: Column `Incorrect` not found in: ['Mapped Reads Cv', 'Max Mapped Reads', 'Min Mapped Reads', 'Total Fraction Mapped Reads', 'Total Pf Reads', 'Total Reads'] - column not consistent with level or misspelled

    >>> index_summary(run_metrics_with_indexing, level='Incorrect')
    Traceback (most recent call last):
    ...
    ValueError: level=Incorrect not in ('Lane', 'Barcode')

    :param run_metrics: py_interop_run_metrics.run_metrics or string run folder path
    :param level: level of the data to summarize, valid values include: 'Total', 'NonIndex', 'Read', 'Lane', 'Surface' (Default: Total)
    :param columns: list of columns (valid values depend on the level) see `summary_columns`
    :param dtype: data type for the array (Default: 'f4')
    :param extra: all extra parameters are passed to `read` if the first parameter is a str file path to a run folder
    :return: structured with column names and dype - np.array
    """

    if columns is None:
        columns = index_summary_columns(level)
    else:
        if level not in _index_summary_levels:
            raise ValueError("level={} not in {}".format(str(level), str(_index_summary_levels)))

    extra['valid_to_load'] = create_valid_to_load(('Index', 'Tile', ))
    run_metrics = read(run_metrics, **extra)
    if run_metrics.empty():
        return np.asarray([])

    if not isinstance(run_metrics, interop_metrics.run_metrics):
        raise ValueError("Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`")

    if not isinstance(dtype, str):
        dtype = np.dtype(dtype).str
    summary_obj = interop_summary.index_flowcell_summary()
    interop_summary.summarize_index_metrics(run_metrics, summary_obj)

    if isinstance(columns, str):
        columns = (columns, )
    column_map = index_summary_columns(level, ret_dict=True)
    for col in columns:
        if col not in column_map:
            raise ValueError("Column `{}` not found in: {} - column not consistent with level or misspelled".format(
                col, str(sorted([k for k in column_map.keys()]))))

    def summarize(summary_object, column_list, extra_row=None):

        row_vals = []
        column_header = []
        if extra_row is None:
            extra_row = []
        for column in column_list:
            method_name = column_map[column]
            val = getattr(summary_object, method_name)()
            column_header.append(column)
            row_vals.append(val)
        return tuple(extra_row+row_vals), column_header

    extra_cols = [('Lane', np.uint16)]
    if level == 'Lane':
        data = []
        lane_count = summary_obj.size()
        column_count = None
        for lane_index in range(lane_count):
            lane_summary = summary_obj.at(lane_index)
            lane_number = lane_index+1
            row, header = summarize(lane_summary
                                                 , columns
                                                 , [lane_number])
            if column_count is None:
                column_count = len(row)
            else:
                assert column_count == len(row)
            data.append(row)
        if len(data) == 0:
            return np.asarray([])
        return np.asarray(data, dtype=extra_cols+[(col, dtype) for col in header])

    data = []
    lane_count = summary_obj.size()
    column_count = None
    for lane_index in range(lane_count):
        lane_summary = summary_obj.at(lane_index)
        lane_number = lane_index+1
        sample_count = lane_summary.size()
        for sample_index in range(sample_count):
            sample_summary = lane_summary.at(sample_index)
            row, header = summarize(sample_summary
                                    , columns
                                    , [lane_number])
            if column_count is None:
                column_count = len(row)
            else:
                assert column_count == len(row)
            data.append(row)
    if len(data) == 0:
        return np.asarray([])
    _str_types = ('Index1', 'Index2', 'Sample Id', 'Project Name')
    return np.asarray(data, dtype=extra_cols+[(col, dtype if col not in _str_types else np.object) for col in header])


def index_summary_columns(level='Lane', ret_dict=False):
    """ List the columns of the `index_summary` table

    >>> from interop import index_summary_columns
    >>> index_summary_columns()
    ('Mapped Reads Cv', 'Max Mapped Reads', 'Min Mapped Reads', 'Total Fraction Mapped Reads', 'Total Pf Reads', 'Total Reads')


    >>> index_summary_columns('Barcode')
    ('Cluster Count', 'Fraction Mapped', 'Id', 'Index1', 'Index2', 'Project Name', 'Sample Id')

    :param level: level of the data to summarize, valid values include: 'Lane', 'Barcode' (Default: Lane)
    :param ret_dict: if true, return a dict mapping from column name to method name (Default: False)
    :return: tuple of columns (or dictionary mapping column name to method depending on `ret_dict` parameter)
    """

    if level not in _index_summary_levels:
        raise ValueError("level={} not in {}".format(str(level), str(_index_summary_levels)))
    summary_obj = interop_summary.index_lane_summary() if level == 'Lane' else interop_summary.index_count_summary()
    exclude_attrs = ('thisown', 'this', '_s', 'at', 'size', 'resize', 'lane', 'surface', 'cycle_state', 'clear', 'reserve', 'sort', 'push_back', 'set', 'add', 'update_fraction_mapped')
    methods = tuple([v for v in dir(summary_obj) if not v.startswith('__') and v not in exclude_attrs])

    def to_column_name(method):
        return " ".join([v.capitalize().replace("Percent", "%") .replace("Gt", ">=") for v in method.split('_')])

    if ret_dict:
        return dict([(to_column_name(v), v) for v in methods])
    return tuple([to_column_name(c) for c in methods])


def summary(run_metrics, level='Total', columns=None, dtype='f4', ignore_missing_columns=True, **extra):
    """ Generate a summary table with the given level, columns and dtype from a run_metrics object or run_folder path

    Note that not all columns will be included if InterOp files are missing or purposing excluded using `valid_to_load`.

    The following examples show the different levels that one can summarize the data including:

     - Total (Default)
     - NonIndex
     - Read
     - Lane
     - Summary

    >>> from interop import summary
    >>> ar = summary("some/path/run_folder_name") # doctest: +SKIP
    >>> ar = summary("some/path/run_folder_name", valid_to_load=['Error']) # doctest: +SKIP


    >>> summary(run_metrics_example)
    array([(0.36666667, 6.6666665, 0., 0., 0.)],
          dtype=[('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4')])

    >>> summary(run_metrics_example, 'Total')
    array([(0.36666667, 6.6666665, 0., 0., 0.)],
          dtype=[('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4')])

    >>> summary(run_metrics_example, 'NonIndex')
    array([(0.2, 10., 0., 0., 0.)],
          dtype=[('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4')])

    >>> summary(run_metrics_example, 'Read')
    array([(1, 78, 0.2, 10., 0., 0., 0.), (2, 89, 0.4,  5., 0., 0., 0.),
           (3, 89, 0.5,  5., 0., 0., 0.)],
          dtype=[('ReadNumber', '<u2'), ('IsIndex', 'u1'), ('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4')])

    >>> summary(run_metrics_example, 'Lane')
    array([(1, 78, 1, 0.2, 10., 0., 0., 0., 1.),
           (2, 89, 1, 0.4,  5., 0., 0., 0., 1.),
           (3, 89, 1, 0.5,  5., 0., 0., 0., 1.)],
          dtype=[('ReadNumber', '<u2'), ('IsIndex', 'u1'), ('Lane', '<u2'), ('Error Rate', '<f4'), ('First Cycle Intensity', '<f4'), ('Projected Yield G', '<f4'), ('Reads', '<f4'), ('Reads Pf', '<f4'), ('Tile Count', '<f4')])

    For a single surface, as is this example, nothing is reported.
    >>> summary(run_metrics_example, 'Surface')
    array([], dtype=float64)

    We can select specific columns using the `columns` parameter
    >>> summary(run_metrics_example, 'Total', columns=['First Cycle Intensity', 'Error Rate'])
    array([(6.6666665, 0.36666667)],
          dtype=[('First Cycle Intensity', '<f4'), ('Error Rate', '<f4')])

    If a column values are NaN, or missing, then it will automatically be excluded
    >>> summary(run_metrics_example, 'Total', columns=['% Aligned', 'Error Rate'])
    array([(0.36666667,)], dtype=[('Error Rate', '<f4')])

    To include missing columns, set `ignore_missing_columns=False`
    >>> summary(run_metrics_example, 'Total', ignore_missing_columns=False, columns=['% Aligned', 'Error Rate'])
    array([(nan, 0.36666667)],
          dtype=[('% Aligned', '<f4'), ('Error Rate', '<f4')])

    >>> summary(run_metrics_example, 'Total', columns=['Incorrect'])
    Traceback (most recent call last):
     ...
    ValueError: Column `Incorrect` not found in: ['Error Rate', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupancy Proxy', '% Occupied', 'Projected Yield G', 'Yield G'] - column not consistent with level or misspelled


    :param run_metrics: py_interop_run_metrics.run_metrics or string run folder path
    :param level: level of the data to summarize, valid values include: 'Total', 'NonIndex', 'Read', 'Lane', 'Surface' (Default: Total)
    :param columns: list of columns (valid values depend on the level) see `summary_columns`
    :param dtype: data type for the array (Default: 'f4')
    :param ignore_missing_columns: ignore missing columns, e.g. those with NaN values (Default: True)
    :param extra: all extra parameters are passed to `read` if the first parameter is a str file path to a run folder
    :return: structured with column names and dype - np.array
    """

    if columns is None:
        columns = summary_columns(level)
    else:
        if level not in _summary_levels:
            raise ValueError("level={} not in {}".format(str(level), str(_summary_levels)))

    if isinstance(run_metrics, str):
        if extra.get('valid_to_load', None) is None:
            extra['valid_to_load'] = load_summary_metrics()
    run_metrics = read(run_metrics, **extra)
    if run_metrics.empty():
        return np.asarray([])

    if not isinstance(run_metrics, interop_metrics.run_metrics):
        raise ValueError("Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`")

    run_summary = interop_summary.run_summary()
    interop_summary.summarize_run_metrics(run_metrics, run_summary, False, False)

    if isinstance(columns, str):
        columns = (columns, )
    column_map = summary_columns(level, ret_dict=True)
    for col in columns:
        if col not in column_map:
            raise ValueError("Column `{}` not found in: {} - column not consistent with level or misspelled".format(
                col, str(sorted([k for k in column_map.keys()]))))
    if not isinstance(dtype, str):
        dtype = np.dtype(dtype).str

    def summarize(summary_object, column_list, extra_row=None, ignore_missing=ignore_missing_columns):
        row_vals = []
        column_header = []
        column_subset = []
        if extra_row is None:
            extra_row = []
        for column in column_list:
            method_name, subcols = column_map[column]
            if callable(subcols):
                subcols = subcols(run_metrics.run_info())
            val = getattr(summary_object, method_name)()
            if hasattr(val, 'mean'):
                val = val.mean()
            if ignore_missing and not np.any(np.isfinite(val)):
                continue
            if len(subcols) > 1:
                assert len(val) == len(subcols)
                column_header.extend(subcols)
                row_vals.extend(val)
            else:
                column_header.append(column)
                row_vals.append(val)
            column_subset.append(column)
        return tuple(extra_row+row_vals), column_header, column_subset

    if level in ('Total', 'NonIndex'):
        summary_obj = run_summary.total_summary() if level == 'Total' else run_summary.nonindex_summary()
        row, header, _ = summarize(summary_obj, columns)
        header = [(col, dtype) for col in header]
        return np.asarray([row], dtype=header)

    extra_cols = [('ReadNumber', np.uint16), ('IsIndex', np.uint8)]
    if level == 'Read':
        data = []
        read_count = run_summary.size()
        column_count = None
        for read_index in range(read_count):
            read_summary = run_summary.at(read_index)
            read_info = read_summary.read()
            read_number = read_info.number()
            is_index = read_info.is_index()
            read_summary = read_summary.summary()
            row, header, columns_sel = summarize(read_summary
                                     , columns
                                     , [read_number, int(ord('Y' if is_index else 'N'))]
                                     , ignore_missing_columns and column_count is None)
            if column_count is None:
                column_count = len(row)
                columns = columns_sel
            data.append(row)
        return np.asarray(data, dtype=extra_cols+[(col, dtype) for col in header])

    extra_cols += [('Lane', np.uint16)]
    if level == 'Lane':
        data = []
        read_count = run_summary.size()
        column_count = None
        for read_index in range(read_count):
            read_summary = run_summary.at(read_index)
            read_info = read_summary.read()
            read_number = read_info.number()
            is_index = read_info.is_index()
            for lane_index in range(read_summary.size()):
                lane_summary = read_summary.at(lane_index)
                lane_number = lane_summary.lane()
                row, header, columns_sel = summarize(lane_summary
                                         , columns
                                         , [read_number, int(ord('Y' if is_index else 'N')), lane_number]
                                         , ignore_missing_columns and column_count is None)
                if column_count is None:
                    column_count = len(row)
                    columns = columns_sel
                data.append(row)
        return np.asarray(data, dtype=extra_cols+[(col, dtype) for col in header])

    data = []
    read_count = run_summary.size()
    column_count = None
    extra_cols += [('Surface', np.uint16)]
    for read_index in range(read_count):
        read_summary = run_summary.at(read_index)
        read_info = read_summary.read()
        read_number = read_info.number()
        is_index = read_info.is_index()
        for lane_index in range(read_summary.size()):
            lane_summary = read_summary.at(lane_index)
            lane_number = lane_summary.lane()
            for surface_index in range(lane_summary.size()):
                surface_summary = lane_summary.at(surface_index)
                surface_number = surface_summary.surface()
                row, header, columns_sel = summarize(surface_summary
                                         , columns
                                         , [read_number, int(ord('Y' if is_index else 'N')), lane_number, surface_number]
                                         , ignore_missing_columns and column_count is None)
                if column_count is None:
                    column_count = len(row)
                    columns = columns_sel
                data.append(row)
    if len(data) == 0:
        return np.asarray([])
    return np.asarray(data, dtype=extra_cols+[(col, dtype) for col in header])


def load_summary_metrics():
    """ List of valid summary metrics to load

    >>> from interop import load_to_string_list
    >>> from interop import load_summary_metrics
    >>> load_to_string_list(load_summary_metrics())
    ['CorrectedInt', 'Error', 'Extraction', 'Q', 'Tile', 'QByLane', 'QCollapsed', 'EmpiricalPhasing', 'ExtendedTile']

    :return: valid_to_load
    """

    valid_to_load = interop_run.uchar_vector(interop_run.MetricCount, 0)
    interop_metrics.list_summary_metrics_to_load(valid_to_load, interop_run.NovaSeq)
    return valid_to_load


def summary_columns(level='Total', ret_dict=False):
    """ Get a list of column names supported at each level of the summary table

    >>> from interop import summary_columns

    The default columns are for the Run/Read level
    >>> summary_columns()
    ('Cluster Count', 'Cluster Count Pf', 'Error Rate', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupancy Proxy', '% Occupied', 'Projected Yield G', 'Reads', 'Reads Pf', 'Yield G')
    >>> summary_columns(level='Total')
    ('Cluster Count', 'Cluster Count Pf', 'Error Rate', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupancy Proxy', '% Occupied', 'Projected Yield G', 'Reads', 'Reads Pf', 'Yield G')
    >>> summary_columns(level='NonIndex')
    ('Cluster Count', 'Cluster Count Pf', 'Error Rate', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupancy Proxy', '% Occupied', 'Projected Yield G', 'Reads', 'Reads Pf', 'Yield G')
    >>> summary_columns(level='Read')
    ('Cluster Count', 'Cluster Count Pf', 'Error Rate', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupancy Proxy', '% Occupied', 'Projected Yield G', 'Reads', 'Reads Pf', 'Yield G')

    The lane/surface level give another set of columns for the summary table
    >>> summary_columns(level='Lane')
    ('Cluster Count', 'Cluster Count Pf', 'Density', 'Density Pf', 'Error Rate', 'Error Rate 100', 'Error Rate 35', 'Error Rate 50', 'Error Rate 75', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupied', '% Pf', 'Phasing', 'Phasing Offset', 'Phasing Slope', 'Prephasing', 'Prephasing Offset', 'Prephasing Slope', 'Projected Yield G', 'Reads', 'Reads Pf', 'Tile Count', 'Yield G')
    >>> summary_columns(level='Surface')
    ('Cluster Count', 'Cluster Count Pf', 'Density', 'Density Pf', 'Error Rate', 'Error Rate 100', 'Error Rate 35', 'Error Rate 50', 'Error Rate 75', 'First Cycle Intensity', '% Aligned', '% >= Q30', '% Occupied', '% Pf', 'Phasing', 'Phasing Offset', 'Phasing Slope', 'Prephasing', 'Prephasing Offset', 'Prephasing Slope', 'Projected Yield G', 'Reads', 'Reads Pf', 'Tile Count', 'Yield G')

    :param level: level of the data to summarize, valid values include: 'Run', 'Read', 'Lane', 'Surface' (Default: Run)
    :param ret_dict: if true, return a dict mapping from column name to method name (Default: False)
    :return: tuple of columns - each column is a tuple, or a tuple of lambda functions that take the run_info as an argument
    """

    if level not in _summary_levels:
        raise ValueError("level={} not in {}".format(str(level), str(_summary_levels)))
    if level == 'Lane' or level == 'Surface':
        summary_obj = interop_summary.lane_summary()
    else:
        summary_obj = interop_summary.read_summary().summary()
    exclude_attrs = ('thisown', 'this', '_s', 'at', 'size', 'resize', 'resize_stat', 'lane', 'surface', 'cycle_state')
    methods = tuple([v for v in dir(summary_obj) if not v.startswith('__') and v not in exclude_attrs])

    def to_column_name(method):
        return " ".join([v.capitalize().replace("Percent", "%") .replace("Gt", ">=") for v in method.split('_')])

    def sub_cols(method):

        column_name = to_column_name(method)
        if method == 'percent_base':
            base_map = {interop_run.A: 'A', interop_run.C: 'C', interop_run.G: 'G', interop_run.T: 'T'}
            return tuple([column_name+" "+base_map[i] for i in range(4)])
        if method == 'fwhm':
            def fwhm_columns(run_info):
                return tuple([column_name+" "+channel for channel in run_info.channels()])
            fwhm_columns.__name__ = 'fwhm_columns'
            return fwhm_columns
        if method == 'resynthesis':
            def resynthesis_columns(run_info):
                return tuple([column_name+" "+channel for channel in run_info.channels()])
            resynthesis_columns.__name__ = 'resynthesis_columns'
            return resynthesis_columns
        return tuple()

    if ret_dict:
        return dict([(to_column_name(v), (v, sub_cols(v))) for v in methods])
    return tuple([to_column_name(c) for c in methods])


def indexing(run_metrics, per_sample=True, dtype='f4', stype='O', **extra):
    """ Convert InterOp run_metrics (or read run_metrics from disk) to a numpy structured array containing an
    indexing table

    We can read an indexing table directly from a run folder. Note, this does not load all metrics, only those required
    by the indexing table, e.g. IndexMetricsOut.bin

    >>> from interop import indexing
    >>> ar = indexing("some/path/run_folder_name") # doctest: +SKIP

    Note that `valid_to_load` in `read` is ignored.


    We can also convert a `run_metrics` object to an indexing table as follows
    >>> ar = indexing(run_metrics_with_indexing)
    >>> ar
    array([(1., 1101., 'ATCACGAC-AAGGTTCA', '1', 4570., 900., 507.77777),
           (1., 1101., 'ATCACGAC-GGGGGGGG', '2', 2343., 900., 260.33334),
           (1., 1102., 'ATCACGAC-AAGGTTCA', '1', 4570.,   0.,   0.     ),
           (1., 1102., 'ATCACGAC-GGGGGGGG', '2', 2343.,   0.,   0.     )],
          dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Barcode', 'O'), ('SampleID', 'O'), ('Cluster Count', '<f4'), ('Cluster Count PF', '<f4'), ('% Demux', '<f4')])

    The `indexing` function also provides an overall sample view by setting `per_sample=False`.

    >>> ar = indexing(run_metrics_with_indexing, per_sample=False)
    >>> ar
    array([(1., 1101., 1000., 900., 768.11115),
           (1., 1102.,    0.,   0.,   0.     )],
          dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Cluster Count', '<f4'), ('Cluster Count PF', '<f4'), ('% Demux', '<f4')])

    :param run_metrics: py_interop_run_metrics.run_metrics or string run folder path
    :param per_sample: return demux per sample (Default: True)
    :param dtype: data type for the array (Default: 'f4')
    :param stype: string type for the array (Default: 'O')
    :param extra: all extra parameters are passed to `read` if the first parameter is a str file path to a run folder
    :return: structured with column names and dype - np.array
    """

    extra['valid_to_load'] = create_valid_to_load(('Index', 'Tile'))
    run_metrics = read(run_metrics, **extra)
    if not isinstance(run_metrics, interop_metrics.run_metrics):
        raise ValueError("Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`")

    if run_metrics.empty():
        return np.asarray([])
    if not isinstance(dtype, str):
        dtype = np.dtype(dtype).str
    if not isinstance(stype, str):
        stype = np.dtype(stype).str

    index_metric_set = run_metrics.index_metric_set()
    column_names = [("Lane", dtype), ("Tile", dtype)]
    if per_sample:
        column_names += [("Barcode", stype), ("SampleID", stype)]
    column_names += [("Cluster Count", dtype), ("Cluster Count PF", dtype), ("% Demux", dtype)]
    sample_count = len(index_metric_set.index_order()) if per_sample else 1
    if sample_count == 0:
        raise ValueError("No samples found, was `finalize_after_load` called in run_metrics?")
    reads_list = run_metrics.run_info().reads()
    index_read_count = sum([1 for i in range(len(reads_list)) if reads_list[i].is_index()])
    table = np.zeros(index_metric_set.size()//index_read_count*sample_count, dtype=column_names)
    select_read = index_metric_set.at(0).read() if index_metric_set.size() > 0 else 1
    k = 0
    for i in range(index_metric_set.size()):
        metric = index_metric_set.at(i)
        if metric.read() != select_read:
            continue
        if per_sample:
            for index_info in metric.indices():
                if metric.cluster_count_pf() > 0:
                    percent_demux = float(index_info.cluster_count()) / np.float32(metric.cluster_count_pf()) * 100.0
                else:
                    percent_demux = 0
                table[k] = (metric.lane()
                            , metric.tile()
                            , index_info.index_seq()
                            , index_info.sample_id()
                            , index_info.cluster_count()
                            , metric.cluster_count_pf()
                            , percent_demux
                            )
                k += 1
        else:
            percent_demux = metric.percent_demultiplexed("")
            if not np.isfinite(percent_demux):
                percent_demux = 0.0
            table[k] = (metric.lane()
                        , metric.tile()
                        , metric.cluster_count()
                        , metric.cluster_count_pf()
                        , percent_demux
                        )
            k += 1
    return table


def imaging(run_metrics, dtype='f4', **extra):
    """ Convert InterOp run_metrics (or read run_metrics from disk) to a numpy structured array containing the imaging
    table

    We can read an imaging table directly from a run folder. Note, this does not load all metrics, only those required
    by the imaging table. See `load_imaging_metrics` for that list.

    Also note that loading only tile level metrics (e.g. metrics without cycles) will result in an empty table. This is
    a limitation of the imaging table.

    >>> from interop import imaging
    >>> from interop import load_imaging_metrics
    >>> import interop.py_interop_run_metrics as interop_metrics
    >>> import numpy as np
    >>> ar = imaging("some/path/run_folder_name") # doctest: +SKIP

    The above function is equivalent to
    >>> ar = imaging("some/path/run_folder_name", valid_to_load=load_imaging_metrics()) # doctest: +SKIP

    We can select a subset of metrics to include based on metric groups
    >>> ar = imaging("some/path/run_folder_name", valid_to_load=['Error']) # doctest: +SKIP

    See `read` below for more examples.

    The following example will rely on an existing run_metrics object (possibly created by the `read` function below).

    >>> ar = imaging(run_metrics_example)
    >>> ar
    rec.array([(1., 1101., 1., 1., 1., 0.1, 10., 10., 25. , 33.3, 33.3, 33.3, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 2., 1., 2., 0.2,  5., 15., 12.5, 42.9, 28.6, 28.6, 0.,  5., 15., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 3., 1., 3., 0.3, 10., 10., 25. , 33.3, 50. , 16.7, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 4., 2., 1., 0.4, 10.,  5., 25. , 16.7, 50. , 33.3, 0., 10.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 5., 3., 1., 0.5, 15.,  5., 37.5, 20. , 40. , 40. , 0., 15.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.)],
              dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Cycle', '<f4'), ('Read', '<f4'), ('Cycle Within Read', '<f4'), ('Error Rate', '<f4'), ('P90/green', '<f4'), ('P90/blue', '<f4'), ('% No Calls', '<f4'), ('% Base/A', '<f4'), ('% Base/C', '<f4'), ('% Base/G', '<f4'), ('% Base/T', '<f4'), ('Fwhm/green', '<f4'), ('Fwhm/blue', '<f4'), ('Corrected/A', '<f4'), ('Corrected/C', '<f4'), ('Corrected/G', '<f4'), ('Corrected/T', '<f4'), ('Called/A', '<f4'), ('Called/C', '<f4'), ('Called/G', '<f4'), ('Called/T', '<f4'), ('Signal To Noise', '<f4'), ('Surface', '<f4'), ('Swath', '<f4'), ('Tile Number', '<f4')])

    >>> ar.dtype
    dtype((numpy.record, [('Lane', '<f4'), ('Tile', '<f4'), ('Cycle', '<f4'), ('Read', '<f4'), ('Cycle Within Read', '<f4'), ('Error Rate', '<f4'), ('P90/green', '<f4'), ('P90/blue', '<f4'), ('% No Calls', '<f4'), ('% Base/A', '<f4'), ('% Base/C', '<f4'), ('% Base/G', '<f4'), ('% Base/T', '<f4'), ('Fwhm/green', '<f4'), ('Fwhm/blue', '<f4'), ('Corrected/A', '<f4'), ('Corrected/C', '<f4'), ('Corrected/G', '<f4'), ('Corrected/T', '<f4'), ('Called/A', '<f4'), ('Called/C', '<f4'), ('Called/G', '<f4'), ('Called/T', '<f4'), ('Signal To Noise', '<f4'), ('Surface', '<f4'), ('Swath', '<f4'), ('Tile Number', '<f4')]))

    We can convert the numpy array to a Pandas DataFrame as follows:

    >>> import pandas as pd  # doctest: +SKIP
    >>> df = pd.DataFrame(ar)  # doctest: +SKIP
    >>> df  # doctest: +SKIP
       Lane  ...  Tile Number
    0   1.0  ...          1.0
    1   1.0  ...          1.0
    2   1.0  ...          1.0
    3   1.0  ...          1.0
    4   1.0  ...          1.0
    <BLANKLINE>
    [5 rows x 27 columns]

    You can also change the dtype of the resulting data array table.
    >>> imaging(run_metrics_example, dtype=np.float32)
    rec.array([(1., 1101., 1., 1., 1., 0.1, 10., 10., 25. , 33.3, 33.3, 33.3, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 2., 1., 2., 0.2,  5., 15., 12.5, 42.9, 28.6, 28.6, 0.,  5., 15., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 3., 1., 3., 0.3, 10., 10., 25. , 33.3, 50. , 16.7, 0., 10., 10., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 4., 2., 1., 0.4, 10.,  5., 25. , 16.7, 50. , 33.3, 0., 10.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.),
               (1., 1101., 5., 3., 1., 0.5, 15.,  5., 37.5, 20. , 40. , 40. , 0., 15.,  5., nan, nan, nan, nan, nan, nan, nan, nan, nan, 1., 1., 1.)],
              dtype=[('Lane', '<f4'), ('Tile', '<f4'), ('Cycle', '<f4'), ('Read', '<f4'), ('Cycle Within Read', '<f4'), ('Error Rate', '<f4'), ('P90/green', '<f4'), ('P90/blue', '<f4'), ('% No Calls', '<f4'), ('% Base/A', '<f4'), ('% Base/C', '<f4'), ('% Base/G', '<f4'), ('% Base/T', '<f4'), ('Fwhm/green', '<f4'), ('Fwhm/blue', '<f4'), ('Corrected/A', '<f4'), ('Corrected/C', '<f4'), ('Corrected/G', '<f4'), ('Corrected/T', '<f4'), ('Called/A', '<f4'), ('Called/C', '<f4'), ('Called/G', '<f4'), ('Called/T', '<f4'), ('Signal To Noise', '<f4'), ('Surface', '<f4'), ('Swath', '<f4'), ('Tile Number', '<f4')])

    Here is the output if an empty run_metrics was provided
    >>> imaging(interop_metrics.run_metrics())
    array([], dtype=float64)

    Here is an example exception if an improper input is given
    >>> imaging(None)
    Traceback (most recent call last):
    ...
    ValueError: Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`

    :param run_metrics: py_interop_run_metrics.run_metrics or str file path to a run folder
    :param dtype: data type for the array (Default: 'f4')
    :param extra: all extra parameters are passed to `read` if the first parameter is a str file path to a run folder
    :return: structured with column names and dype - np.array
    """

    if isinstance(run_metrics, str):
        if extra.get('valid_to_load', None) is None:
            extra['valid_to_load'] = load_imaging_metrics()
        run_metrics = read(run_metrics, **extra)
    if not isinstance(run_metrics, interop_metrics.run_metrics):
        raise ValueError("Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`")

    if run_metrics.empty():
        return np.asarray([])

    headers, columns = _imaging_columns(run_metrics)
    row_offsets = interop_table.map_id_offset()
    interop_table.count_table_rows(run_metrics, row_offsets)
    column_count = interop_table.count_table_columns(columns)
    data = np.zeros((len(row_offsets), column_count), dtype=dtype)
    interop_table.populate_imaging_table_data(run_metrics, columns, row_offsets, data.ravel())

    if not isinstance(dtype, str):
        dtype = np.dtype(dtype).str
    return np.core.records.fromarrays(data.transpose()
                                      , names=",".join(headers)
                                      , formats=",".join([dtype for _ in headers]))


def imaging_columns(run_metrics, **extra):
    """ Get a list of imaging table columns

    >>> from interop import imaging_columns
    >>> from interop import load_imaging_metrics
    >>> import interop.py_interop_run_metrics as interop_metrics
    >>> import numpy as np
    >>> ar = imaging_columns("some/path/run_folder_name") # doctest: +SKIP

    The above function is equivalent to
    >>> ar = imaging_columns("some/path/run_folder_name", valid_to_load=load_imaging_metrics()) # doctest: +SKIP

    We can select a subset of metrics to include based on metric groups
    >>> ar = imaging_columns("some/path/run_folder_name", valid_to_load=['Error']) # doctest: +SKIP

    See `read` below for more examples.

    The following example will rely on an existing run_metrics object (possibly created by the `read` function below).

    >>> imaging_columns(run_metrics_example)
    ['Lane', 'Tile', 'Cycle', 'Read', 'Cycle Within Read', 'Error Rate', 'P90/green', 'P90/blue', '% No Calls', '% Base/A', '% Base/C', '% Base/G', '% Base/T', 'Fwhm/green', 'Fwhm/blue', 'Corrected/A', 'Corrected/C', 'Corrected/G', 'Corrected/T', 'Called/A', 'Called/C', 'Called/G', 'Called/T', 'Signal To Noise', 'Surface', 'Swath', 'Tile Number']

    :param run_metrics: py_interop_run_metrics.run_metrics or str file path to a run folder
    :param extra: all extra parameters are passed to `read` if the first parameter is a str file path to a run folder
    :return: list of string headers
    """

    if isinstance(run_metrics, str):
        if extra.get('valid_to_load', None) is None:
            extra['valid_to_load'] = load_imaging_metrics()
        run_metrics = read(run_metrics, **extra)
    if not isinstance(run_metrics, interop_metrics.run_metrics):
        raise ValueError("Expected interop.py_interop_run_metrics.run_metrics or str for `run_metrics`")

    return _imaging_columns(run_metrics)[0]


def _imaging_columns(run_metrics):
    """ Internal function for getting the imaging columns

    :param run_metrics: py_interop_run_metrics.run_metrics or str file path to a run folder
    :return: a tuple including (headers, imaging_column_vector)
    """

    columns = interop_table.imaging_column_vector()
    interop_table.create_imaging_table_columns(run_metrics, columns)

    headers = []
    for i in range(columns.size()):
        column = columns[i]
        if column.has_children():
            headers.extend([str(column.name()) + "/" + str(subname).strip() for subname in column.subcolumns()])
        else:
            headers.append(str(column.name()))
    return headers, columns


def read(run, valid_to_load=None, requires=None, search_paths=None, **extra):
    """ Read InterOp metrics into a run_metrics object

    - List of validate valid_to_load names can be gotten using `list_interop_files`
    - If run is `interop.py_interop_run_metrics.run_metrics` then run is returned.
    - If an InterOp file is missing from the `requires` list, then an empty run_metrics object is returned

    Read in all metrics from a run folder
    >>> from interop import read
    >>> metrics = read("some/path/run_folder_name") # doctest: +SKIP

    Read in only ErrorMetricsOut.bin in a run folder
    >>> metrics = read("some/path/run_folder_name", valid_to_load=['Error']) # doctest: +SKIP

    Read in ErrorMetricsOut.bin and ExtractionMetricsOut.bin but if ErrorMetricsOut.bin is missing return an empty
    >>> metrics = read("some/path/run_folder_name", valid_to_load=['Error', 'Extraction'], requires=['Error']) # doctest: +SKIP

    Read in IndexMetricsOut.bin and search for it outside the run folder in `fastq/reports`
    >>> metrics = read("some/path/run_folder_name", valid_to_load=['Index'], search_paths=['fastq/reports']) # doctest: +SKIP

    Read in a run folder that is not found
    >>> metrics = read("some/non/existing/run_folder_name")
    Traceback (most recent call last):
    ...
    interop.py_interop_run.xml_file_not_found_exception: cannot open file some/non/existing/run_folder_name/RunInfo.xml

    Read from a None object
    >>> metrics = read(None)
    Traceback (most recent call last):
    ...
    ValueError: invalid null reference in method 'run_metrics_read', argument 2 of type 'std::string const &'

    :param run: string path including name of run folder (or run_metrics object)
    :param valid_to_load: list of strings containing InterOp metric names (Default: None, load everything)
    :param requires: list of required metric (Default: None, check nothing)
    :param search_paths: list of paths to search when looking for `IndexMetricsOut.bin` (Default: None, do not search)
    :return: interop.py_interop_run_metrics.run_metrics
    """

    if isinstance(run, interop_metrics.run_metrics):
        return run

    if search_paths is None:
        search_paths = (os.path.join('Analysis', '1', 'Data', 'Reports'))
    if isinstance(search_paths, str):
        search_paths = [search_paths]
    if valid_to_load is None:
        valid_to_load = []
    if requires is None:
        requires = []

    run_metrics = interop_metrics.run_metrics()
    valid_to_load = create_valid_to_load(valid_to_load)
    if valid_to_load is not None:
        run_metrics.read(run, valid_to_load)
    else:
        run_metrics.read(run)

    if (valid_to_load is None or 'Index' in load_to_string_list(valid_to_load)) and run_metrics.index_metric_set().empty():
        for path in search_paths:
            filename = os.path.join(run, path, "IndexMetricsOut.bin")
            if not os.path.exists(filename):
                continue
            if run_metrics.run_info().name() == "":
                run_metrics.read_xml(run)
            run_metrics = read_metric(filename, run_metrics=run_metrics, finalize=True)
            break
    for group in requires:
        if run_metrics.is_group_empty(group):
            return interop_metrics.run_metrics()

    return run_metrics


def read_metric(filename, run_metrics=None, finalize=False):
    """ Read a specific metric from a file into a run_metrics object

    This function allows incremental reading of metric files from disk. The last call should set
    `finalize=True`.

    Read in `ErrorMetricsOut.bin` into a run_metrics object and finalize since this is the only metric we plan to read

    >>> from interop import read_metric
    >>> metrics = read_metric("some/path/run_folder_name/InterOp/ErrorMetricsOut.bin", finalize=True) # doctest: +SKIP

    :param filename: path to InterOp file
    :param run_metrics: existing run_metrics object (Default None, one will be created)
    :param finalize: if true, then call finalize_after_load (last call to `read_metric` should set finalize=True)
    :return: interop.py_interop_run_metrics.run_metrics
    """

    if run_metrics is None:
        run_metrics = interop_metrics.run_metrics()
    metric_group = group_from_filename(filename)
    data = np.fromfile(filename, dtype=np.uint8)
    run_metrics.read_metrics_from_buffer(metric_group, data)
    if finalize:
        run_metrics.finalize_after_load()
    return run_metrics


def create_valid_to_load(interop_prefixes):
    """ Create list of metrics valid to load by the InterOp library

    List of validate metric_names can be gotten using `list_interop_files`

    >>> from interop import create_valid_to_load
    >>> int(create_valid_to_load(['Extraction'])[0])
    0
    >>> create_valid_to_load(0)
    Traceback (most recent call last):
        ...
    TypeError: Parameter valid_to_load must be a collection of values

    :param interop_prefixes: list of strings containing InterOp metric names
    :return: py_interop_run.uchar_vector
    """

    if not hasattr(interop_prefixes, '__len__'):
        raise TypeError("Parameter valid_to_load must be a collection of values")

    if interop_prefixes is None or len(interop_prefixes) == 0:
        return None

    if isinstance(interop_prefixes, interop_run.uchar_vector):
        return interop_prefixes

    valid_to_load = interop_run.uchar_vector(interop_run.MetricCount, 0)
    enable_metrics(valid_to_load, interop_prefixes)

    return valid_to_load


def enable_metrics(valid_to_load, interop_prefixes):
    """ Enable metrics in valid_to_load

    >>> from interop import enable_metrics, load_to_string_list
    >>> import interop.py_interop_run as interop_run
    >>> valid_to_load = interop_run.uchar_vector(interop_run.MetricCount, 0)
    >>> load_to_string_list(enable_metrics(valid_to_load, 'Extraction'))
    ['Extraction']
    >>> load_to_string_list(enable_metrics(valid_to_load, ['Error', 'Q']))
    ['Error', 'Extraction', 'Q']

    Nothing changes when passing in an empty list
    >>> load_to_string_list(enable_metrics(valid_to_load, []))
    ['Error', 'Extraction', 'Q']

    Here are some example exceptions when the improper parameter is given

    >>> enable_metrics(valid_to_load, None)
    Traceback (most recent call last):
      ...
    TypeError: 'NoneType' object is not iterable
    >>> enable_metrics(None, [])
    Traceback (most recent call last):
    ...
    TypeError: Parameter valid_to_load must be of type interop.py_interop_run.uchar_vector
    >>> enable_metrics("None", [])
    Traceback (most recent call last):
    ...
    TypeError: Parameter valid_to_load must be of type interop.py_interop_run.uchar_vector


    :param valid_to_load: interop_run.uchar_vector (boolean array)
    :param interop_prefixes: list of metrics to enable
    :return: interop_run.uchar_vector (It is updated in-place so the return can be ignored)
    """
    if not isinstance(valid_to_load, interop_run.uchar_vector):
        raise TypeError("Parameter valid_to_load must be of type interop.py_interop_run.uchar_vector")

    if isinstance(interop_prefixes, str):
        interop_prefixes = [interop_prefixes]

    for metric_name in interop_prefixes:
        if interop_run.parse_metric_group(metric_name) >= interop_run.MetricCount:
            raise ValueError("Cannot parse metric file name: {}".format(metric_name))
        valid_to_load[interop_run.parse_metric_group(metric_name)] = 1
    return valid_to_load


def load_to_string_list(valid_to_load):
    """ Create a string list of names for each enabled metric in `valid_to_load`

    >>> from interop import create_valid_to_load, load_to_string_list
    >>> import interop.py_interop_run as interop_run
    >>> valid_to_load = create_valid_to_load('Extraction')
    >>> load_to_string_list(valid_to_load)
    ['Extraction']
    >>> valid_to_load = interop_run.uchar_vector(interop_run.MetricCount, 1)
    >>> load_to_string_list(valid_to_load)
    ['CorrectedInt', 'Error', 'Extraction', 'Image', 'Index', 'Q', 'Tile', 'QByLane', 'QCollapsed', 'EmpiricalPhasing', 'DynamicPhasing', 'ExtendedTile', 'SummaryRun']

    :param valid_to_load: boolean buffer
    :return: list of strings containing the name of each metric enabled in `valid_to_load`
    """

    if not isinstance(valid_to_load, interop_run.uchar_vector):
        raise TypeError("Parameter valid_to_load must be of type interop.py_interop_run.uchar_vector")

    names = []
    for i in range(interop_run.MetricCount):
        if valid_to_load[i] > 0:
            names.append(interop_run.to_string_metric_group(i))
    return names


def group_from_filename(filename):
    """ Get the metric group id from an InterOp filename path

    >>> from interop import group_from_filename
    >>> import interop.py_interop_run as interop_run
    >>> group_from_filename("some/path/run/InterOp/ExtractionMetricsOut.bin")
    2
    >>> interop_run.Extraction
    2

    This group id can be used to load a metric from a binary buffer as in `interop.core.read_metric`

    :param filename: path to interop metric
    :return: interop_run.metric_group
    """

    metric_name = os.path.basename(filename)
    metric_name, ext = os.path.splitext(metric_name)
    if ext != '.bin':
        raise ValueError("InterOp file must have `bin` extension: {}".format(filename))
    if metric_name.endswith('Out'):
        metric_name = metric_name[:-3]
    if metric_name.endswith('Metrics'):
        metric_name = metric_name[:-7]
    else:
        if ext != 'bin':
            raise ValueError("InterOp file must have `Metrics.bin` or `MetricsOut.bin` suffix: {}".format(filename))
    group = interop_run.parse_metric_group(metric_name)
    if group >= interop_run.MetricCount:
        raise ValueError("Cannot identify InteropMetric from: {}".format(filename))
    return group


def load_imaging_metrics():
    """ List of valid imaging metrics to load

    >>> from interop import load_to_string_list
    >>> from interop import load_imaging_metrics
    >>> load_to_string_list(load_imaging_metrics())
    ['CorrectedInt', 'Error', 'Extraction', 'Image', 'Q', 'Tile', 'QByLane', 'QCollapsed', 'EmpiricalPhasing', 'DynamicPhasing', 'ExtendedTile']

    :return: valid_to_load
    """

    valid_to_load = interop_run.uchar_vector(interop_run.MetricCount, 0)
    interop_table.list_imaging_table_metrics_to_load(valid_to_load)
    return valid_to_load

########################################################################################################################
# Functions and other code to support doc tests
########################################################################################################################


def _run_info_example_fixture(tiles=None):
    """Fixture used for doctests"""

    run_name = "111111_UNKNOWN_1_XXYT"
    run_info_version = 6
    run_date, instrument_name, run_number, flowcell_id = run_name.split('_')
    if tiles is None:
        tiles = ['1_1101']
    lane_count = len(set(tile.split('_')[0] for tile in tiles))
    surface_count = len(set(tile.split('_')[1][0] for tile in tiles))
    swath_count = len(set(tile.split('_')[1][1] for tile in tiles))
    tile_count = len(set(tile.split('_')[1][2:] for tile in tiles))
    sections_per_lane = 1
    lanes_per_section = 1
    naming_method = interop_run.FourDigit
    flowcell_layout = interop_run.flowcell_layout(lane_count
                                                  , surface_count
                                                  , swath_count
                                                  , tile_count
                                                  , sections_per_lane
                                                  , lanes_per_section
                                                  , tiles
                                                  , naming_method
                                                  , flowcell_id)
    channels = ['green', 'blue']
    width = 7875
    height = 10500
    image_dimensions = interop_run.image_dimensions(width, height)
    reads = interop_run.read_info_vector()
    is_reverse_complement = False
    first_cycle = 1
    for read_num, cycle_count, is_index in [(1, 3, False), (2, 1, True), (3, 1, True)]:
        last_cycle = first_cycle + cycle_count
        reads.push_back(interop_run.read_info(read_num, first_cycle, last_cycle, is_index, is_reverse_complement))
        first_cycle = last_cycle
    run_info = interop_run.info(run_name
                                , run_date
                                , instrument_name
                                , int(run_number)
                                , run_info_version
                                , flowcell_layout
                                , channels
                                , image_dimensions
                                , reads)
    return run_info


def _run_metrics_example_fixture():
    """Fixture used for doctests"""

    run_info = _run_info_example_fixture()
    metrics = interop_metrics.run_metrics(run_info)
    adapter_rate = np.nan
    tile_num = 1101
    lane_num = 1

    error_metric_set = metrics.error_metric_set()
    for cycle, error_rate in enumerate([0.1, 0.2, 0.3, 0.4, 0.5]):
        error_metric_set.insert(interop_metric_sets.error_metric(lane_num, tile_num, cycle+1, error_rate, adapter_rate))

    corrected_int_metric_set = metrics.corrected_intensity_metric_set()
    for cycle, call_counts in enumerate([[10, 10, 10, 10, 0], [5, 15, 10, 10, 0], [10, 10, 15, 5, 0], [10, 5, 15, 10, 0], [15, 5, 10, 10, 0]]):
        call_counts = np.array(call_counts, dtype=np.uint32)
        corrected_int_metric_set.insert(interop_metric_sets.corrected_intensity_metric(
            lane_num, tile_num, cycle+1, call_counts, 0, 0))  # Dummy 0s are to work around swig bug

    extraction_metric_set = metrics.extraction_metric_set()
    for cycle, call_counts in enumerate([[10, 10], [5, 15], [10, 10], [10, 5], [15, 5]]):
        intensity_array = np.array(call_counts, dtype=np.uint16)
        focus_array = np.array(call_counts, dtype=np.float32)
        extraction_metric_set.insert(interop_metric_sets.extraction_metric(
            lane_num, tile_num, cycle+1, intensity_array, focus_array, 0))
    return metrics


def _index_metrics_example_fixture():
    """Fixture used for doctests"""

    run_info = _run_info_example_fixture(['1_1101', '1_1102'])
    metrics = interop_metrics.run_metrics(run_info)
    index_metric_set = metrics.index_metric_set()

    indices = interop_metric_sets.index_info_vector()
    for barcode, sample_id, sample_proj, cluster_count in [
        ("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570)
        , ("ATCACGAC-GGGGGGGG", "2", "TSCAIndexes", 2343)
    ]:
        indices.push_back(interop_metric_sets.index_info(barcode, sample_id, sample_proj, cluster_count))
    tile_num = 1101
    lane_num = 1

    for read_num in [2, 3]:
        index_metric_set.insert(interop_metric_sets.index_metric(lane_num, tile_num, read_num, indices));

    tile_metric_set = metrics.tile_metric_set()
    reads = interop_metric_sets.read_metric_vector()
    cluster_density = 1000
    cluster_density_pf = 900
    cluster_count = 1000
    cluster_count_pf = 900
    tile_metric_set.insert(interop_metric_sets.tile_metric(lane_num
                                                           , tile_num
                                                           , cluster_density
                                                           , cluster_density_pf
                                                           , cluster_count
                                                           , cluster_count_pf
                                                           , reads))

    tile_num = 1102
    lane_num = 1
    cluster_density = 0
    cluster_density_pf = 0
    cluster_count = 0
    cluster_count_pf = 0

    for read_num in [2, 3]:
        index_metric_set.insert(interop_metric_sets.index_metric(lane_num, tile_num, read_num, indices));
    tile_metric_set.insert(interop_metric_sets.tile_metric(lane_num
                                                           , tile_num
                                                           , cluster_density
                                                           , cluster_density_pf
                                                           , cluster_count
                                                           , cluster_count_pf
                                                           , reads))

    metrics.finalize_after_load()
    return metrics


# class RunFolderToDiskFixture(object):
#
#     def __init__(self):
#
#         self.run_folder = os.path.absolue("./210326_")
#         run = _run_metrics_example_fixture()
#         run.write
#
#     def __del__(self):
#         if self.run_folder is not None:
#             import shutil
#             shutil.deltree(self.run_folder)


def _run_doctests():

    import interop.core
    import doctest
    import sys
    failure_count, test_count = doctest.testmod(interop.core
                                                , optionflags=doctest.IGNORE_EXCEPTION_DETAIL
                                                , globs=dict(
        run_metrics_with_indexing=_index_metrics_example_fixture(),
        run_metrics_example=_run_metrics_example_fixture()
    ))
    if failure_count > 0:
        sys.exit(1)



if __name__ == "__main__":

    _run_doctests()
