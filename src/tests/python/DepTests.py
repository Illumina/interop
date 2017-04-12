"""
Unit tests to ensure the dependent SWIG wrapped modules are properly imported
"""
import unittest
import os
try:
    from interop import py_interop_run_metrics
except:
    import sys
    sys.path.append(".")
    from interop import py_interop_run_metrics

class DependencyTests(unittest.TestCase):
    """ Unit tests to ensure the dependent SWIG wrapped modules are properly imported
    """

    def test_xml_file_not_found_exception(self):
        """
        Test whether xml_file_not_found_exception in py_interop_run can be used
        """

        run = py_interop_run_metrics.run_metrics()
        try:
            run.read('file/not/found')
            self.fail("xml_file_not_found_exception should have been thrown")
        except Exception as ex:
            self.assertEqual(str(ex).split('\n')[0], "cannot open file " + os.path.join("file/not/found", "RunInfo.xml"))

if __name__ == '__main__':
    unittest.main()
