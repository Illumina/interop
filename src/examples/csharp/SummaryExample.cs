
// @ [Create Summary Model from Run Folder]
using System;
using Illumina.InterOp.Run;
using Illumina.InterOp.Metrics;
using Illumina.InterOp.RunMetrics;
using Illumina.InterOp.Comm;
using Illumina.InterOp.Summary;

class SummaryExample
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

		run_metrics metrics = new run_metrics();
		uchar_vector interopsToLoad = new uchar_vector();

		// Load only the metrics required by summary
		c_csharp_run_metrics.list_summary_metrics_to_load(interopsToLoad);
		metrics.read(args[0], interopsToLoad);


        run_summary summary = new run_summary();
        c_csharp_summary.summarize_run_metrics(metrics, summary);


		Console.WriteLine("Yield: {0}", summary.total_summary().yield_g());
		return 0;
	}
}
// @ [Create Summary Model from Run Folder]
