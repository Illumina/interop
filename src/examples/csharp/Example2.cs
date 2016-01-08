/// @ [Reporting Tile Metrics in CSharp]
using System;
using System.Collections.Generic;


class TileSummary
{
    public float cluster_count;
	public float cluster_count_pf;
	public uint tile_count;
};

class Example2
{
	static int Main(string[] args)
	{

        int ret;
        if((ret = Helper.CheckArguments(args)) != 0) return ret;

		tile_metrics tile_metric_set = new tile_metrics ();
		if ((ret = Helper.ReadInterop (args [0], tile_metric_set)) != 0)
			return ret;

        Dictionary<uint, TileSummary> lane_summary_map = new Dictionary<uint, TileSummary>();
        foreach(var metric in tile_metric_set.metrics())
        {
			if (!lane_summary_map.ContainsKey (metric.lane ()))
				lane_summary_map.Add (metric.lane (), new TileSummary ());
            lane_summary_map[metric.lane()].cluster_count += metric.clusterCount();
            lane_summary_map[metric.lane()].cluster_count_pf += metric.clusterCountPf();
            lane_summary_map[metric.lane()].tile_count += 1;
        }
		foreach (KeyValuePair<uint, TileSummary> pair in lane_summary_map)
        {
			Console.WriteLine("Lane: {0}", pair.Key);
            Console.WriteLine("Tiles: {0}", pair.Value.tile_count);
			Console.WriteLine("Cluster PF (%): {0}", pair.Value.cluster_count_pf /  pair.Value.cluster_count * 100);
        }

		return 0;
	}
}
/// @ [Reporting Tile Metrics in CSharp]

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
	public static int ReadInterop(string filename, tile_metrics tile_metric_set)
	{
		try
		{
			c_csharp_interop.read_interop (filename, tile_metric_set);
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