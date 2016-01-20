
%define SHARED_METRIC_SET_METHODS(metric_t)
    private long _dataSourceLength;
    private bool _dataSourceExists;
    public long DataSourceLength { get{return _dataSourceLength;} set{_dataSourceLength=value;} }
    public bool DataSourceExists { get{return _dataSourceExists;} set{_dataSourceExists=value;} }
    public byte Version { get{ return (byte)version(); } }
    public global::System.Int64 GetKey(int lane, int tile)
    {
        return (global::System.Int64)metric_t.id((ulong)lane, (ulong)tile);
    }
    public bool HasData { get { return size() > 0; } }
    public metric_t GetMetric(long key)
    {
        return get_metric((ulong)key);
    }
    public metric_t GetMetric(int lane, int tile)
    {
        try{
            return get_metric((uint)lane, (uint)tile);
        }catch(global::System.IndexOutOfRangeException){return null;}
    }
    public metric_t GetMetric(ulong key)
    {
        return get_metric(key);
    }
     public global::System.Collections.Generic.IEnumerable< metric_t > GetMetricsInLane(int lane)
        {
            return global::System.Linq.Enumerable.Where(metrics(), m => m.lane() == lane);
        }
        public global::System.Collections.Generic.List<uint> GetTiles(int lane)
        {
            return global::System.Linq.Enumerable.ToList(tile_numbers_for_lane((uint)lane));
        }
        public global::System.Collections.Generic.IEnumerable< metric_t >  GetMetricsForTile(int lane, int tile)
        {
            return global::System.Linq.Enumerable.Where(metrics(), m => m.lane() == lane && m.tile() == tile);
        }
        /*
        public global::System.Collections.Generic.IEnumerable< metric_t > AllMetrics { get { return metrics(); } }
        public global::System.Collections.Generic.IEnumerable<global::System.UInt64> AllKeys { get { return keys(); } }
        */
        public global::System.Collections.Generic.IEnumerable< metric_t > AllMetrics
        {
            get
            {
                 global::System.Collections.Generic.Dictionary<global::System.Int64, metric_t> Lookup = new global::System.Collections.Generic.Dictionary<global::System.Int64, metric_t>();
                 foreach(var metric in metrics())
                 {
                    try{
                        Lookup.Add((global::System.Int64)metric.id(), metric);
                    }catch(global::System.Exception ex)
                    {
                        global::System.Console.WriteLine(metric.Lane+"_"+metric.Tile+" == "+Lookup[(global::System.Int64)metric.id()].Lane+"_"+Lookup[(global::System.Int64)metric.id()].Tile);
                        throw ex;
                    }
                 }
                return Lookup.Values;
            }
        }
        public global::System.Collections.Generic.IEnumerable<global::System.UInt64> AllKeys
        {
            get
            {
                 global::System.Collections.Generic.Dictionary<global::System.UInt64, metric_t> Lookup = new global::System.Collections.Generic.Dictionary<global::System.UInt64, metric_t>();
                 foreach(var metric in metrics())
                 {
                    Lookup.Add((global::System.UInt64)metric.id(), metric);
                 }
                return Lookup.Keys;
            }
        }
%enddef

%define EXTEND_METRIC_SET(metric_t)
    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %typemap(cscode) illumina::interop::model::metric_base::metric_set<metric_t> %{
        SHARED_METRIC_SET_METHODS(metric_t)
    %}
%enddef


%define EXTEND_CYCLE_METRIC_SET(metric_t)

    using namespace illumina::interop::model::metrics;
    namespace metric_base = illumina::interop::model::metric_base;

    %typemap(cscode) illumina::interop::model::metric_base::metric_set<metric_t> %{

        SHARED_METRIC_SET_METHODS(metric_t)

        public int MaxCycle { get{ return (int)max_cycle(); } }
        public global::System.Int64 GetKey(int lane, int tile, int cycle)
        {
            return (global::System.Int64)metric_t.id((ulong)lane, (ulong)tile, (ulong)cycle);
        }
        public metric_t GetMetric(int lane, int tile, int cycle)
        {
            try{
                return get_metric((uint)lane, (uint)tile, (uint)cycle);
            }catch(global::System.IndexOutOfRangeException){return null;}
        }
        public global::System.Collections.Generic.IEnumerable< metric_t > GetMetricsByCycle(global::System.Collections.Generic.IEnumerable<int> cycles)
        {
            return global::System.Linq.Enumerable.Where(metrics(), m => global::System.Linq.Enumerable.Contains<int>(cycles, (int)m.cycle()));
        }
        public global::System.Collections.Generic.IEnumerable< metric_t> GetMetricsByCycle(int lane, int tile, global::System.Collections.Generic.IEnumerable<int> cycles)
        {
            var results = new global::System.Collections.Generic.List< metric_t>();
            foreach (var cycle in cycles)
            {
                var metric = get_metric((uint)lane, (uint)tile, (uint)cycle);
                if (metric != null) results.Add(metric);
            }
            return results;
        }
    %}
%enddef


%typemap(cscode) illumina::interop::model::metrics::tile_metrics %{
     private int _controlLane;
     public int ControlLane { get{return _controlLane;} set{_controlLane=value;} }
     private global::System.Collections.Generic.List<int> _tiles;
     public global::System.Collections.Generic.List<int> Tiles { get{return _tiles;} set{_tiles=value;} }
%}

%typemap(cscode) illumina::interop::model::metrics::q_metrics %{
    public class QScoreBin
    {
         private byte _lower;
         private byte _upper;
         private byte _value;
         public byte Lower { get{return _lower;} set{_lower=value;} }
         public byte Upper { get{return _upper;} set{_upper=value;} }
         public byte Value { get{return _value;} set{_value=value;} }
         public QScoreBin(){}
         public QScoreBin(byte lower, byte upper, byte value)
         {
            Lower = lower;
            Upper = upper;
            Value = value;
         }
    };
    private global::System.Collections.Generic.List<QScoreBin> _qScoreBins;
    public global::System.Collections.Generic.List<QScoreBin> QScoreBins { get{return _qScoreBins;} set{_qScoreBins=value;} }

    public bool IsBinned { get { return binCount() > 0; }}
    public int NumQVals(){ return (int)histBinCount(); }
    public bool IsCompressed { get { return NumQVals() > 0 && NumQVals() != 50; } }
    public int MaxQVal(){ return (int)max_q_value(); }
%}



%typemap(cscode) illumina::interop::model::metrics::tile_metric %{
    public int LatestExtractedCycle;
    public int LatestCalledCycle;
    public int LatestQScoredCycle;
    public int LatestErrorCycle;
    public int cycle(){return -1;}// Hack for bad interface above
%}
%typemap(cscode) illumina::interop::model::metrics::index_metric %{
    public int cycle(){return -1;}// Hack for bad interface above
%}


%typemap(cscode) illumina::interop::model::metric_base::base_metric_header %{
   public int max_cycle(){return 0;} // Hack for bad interface above
%}

%typemap(cscode) illumina::interop::model::metric_base::base_metric %{

    public int Tile { get { return (int)tile(); } }
    public int Lane { get { return (int)lane(); } }
 %}


%typemap(cscode) illumina::interop::model::metric_base::base_cycle_metric %{

    public /*new*/ int Cycle { get { return (int)cycle(); } }
 %}

