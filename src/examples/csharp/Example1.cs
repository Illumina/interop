
// @ [Reading a binary InterOp file in CSharp]
using System;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;

class Example1
{
	static int Main(string[] args)
	{
		if (args.Length != 1)
		{
			if (args.Length < 1)
				Console.WriteLine ("No run folder");
			else
				Console.WriteLine ("Too many arguments");
			return 1;
		}


		tile_metrics tile_metric_set = new tile_metrics ();
		try
		{
			c_csharp_metrics.read_interop (args [0], tile_metric_set);
		}
		catch(incomplete_file_exception){}
		catch(file_not_found_exception ex)
		{

			Console.WriteLine("File not found: "+args [0]);
			Console.WriteLine (ex);
			return 1;
		}
		catch(bad_format_exception ex)
		{
			Console.WriteLine (ex);
			return 1;
		}
		Console.Write ("Tile metric set contains {0} metrics", tile_metric_set.size());
		return 0;
	}
}
// @ [Reading a binary InterOp file in CSharp]

