/// @ [Reporting Extraction Metrics in CSharp]
using System;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.Comm;


class Example3
{
	static int Main(string[] args)
	{

        int ret;
        if((ret = Helper.CheckArguments(args)) != 0) return ret;

		base_extraction_metrics extraction_metric_set = new base_extraction_metrics ();
		if ((ret = Helper.ReadInterop (args [0], extraction_metric_set)) != 0)
			return ret;

		Console.WriteLine("Time: {0}", DateTime.FromBinary((Int64)extraction_metric_set.at(0).date_time_csharp().value));
		return 0;
	}
}
/// @ [Reporting Extraction Metrics in CSharp]

class Helper
{
    public static int CheckArguments (string[] args)
	{
		if (args.Length != 1) {
			if (args.Length < 1)
				Console.WriteLine ("No run folder");
			else
				Console.WriteLine ("Too many arguments");
			return 1;
		}
		return 0;
	}
	public static int ReadInterop(string filename, base_extraction_metrics extraction_metric_set)
	{
		try
		{
			c_csharp_comm.read_interop (filename, extraction_metric_set);
		}
		catch(incomplete_file_exception){}
		catch(file_not_found_exception ex)
		{

			Console.WriteLine("File not found: "+filename);
			Console.WriteLine (ex);
			return 1;
		}
		catch(bad_format_exception ex)
		{
			Console.WriteLine (ex);
			return 1;
		}
		return 0;
	}
}
