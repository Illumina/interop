/** Example 2: Reporting Tile Metrics
 *
 *
 *  @file
 *  @date 11/6/15
 *  @version 1.0
 *  @copyright GNU Public License.
 */




// @ [Reporting Tile Metrics]
#include <iostream>
#include <map>
#include "interop/model/metric_base/metric_set.h"
#include "interop/model/metrics/tile_metric.h"
#include "interop/io/metric_file_stream.h"

using namespace illumina::interop::model::metric_base;
using namespace illumina::interop::model::metrics;
using namespace illumina::interop::io;
int read_interop_file(const char* filename, metric_set<tile_metric>& tile_metric_set);
int check_args(int argc);

struct tile_summary
{
    tile_summary():cluster_count(0), cluster_count_pf(0), tile_count(0){}
    float cluster_count;
    float cluster_count_pf;
    size_t tile_count;
};
typedef std::map<size_t, tile_summary> lane_summary_map_t;

int main(int argc, char** argv)
{
    int ret;
    if((ret = check_args(argc)) != 0) return ret;
    metric_set<tile_metric> tile_metric_set;

    if((ret = read_interop_file(argv[1], tile_metric_set)) != 0) return ret;


    lane_summary_map_t lane_summary_map;
    for(metric_set<tile_metric>::metric_array_t::const_iterator beg = tile_metric_set.begin(), end = tile_metric_set.end();beg != end;++beg)
    {
        lane_summary_map[beg->lane()].cluster_count += beg->cluster_count();
        lane_summary_map[beg->lane()].cluster_count_pf += beg->cluster_count_pf();
        lane_summary_map[beg->lane()].tile_count += 1;
    }

    for(lane_summary_map_t::const_iterator beg = lane_summary_map.begin(), end = lane_summary_map.end();beg != end;++beg)
    {
        std::cout << "Lane: " << beg->first << std::endl;
        std::cout << "Tiles " << beg->second.tile_count << std::endl;
        std::cout << "Cluster PF (%): " << beg->second.cluster_count_pf /  beg->second.cluster_count * 100 << std::endl;
    }

    return 0;
}

// @ [Reporting Tile Metrics]
int check_args(int argc)
{
    if(argc != 2)
    {
        if(argc < 2) std::cerr << "No run folder" << std::endl;
        else std::cerr << "Too many arguments" << std::endl;
        return 1;
    }
    return 0;
}

int read_interop_file(const char* filename, metric_set<tile_metric>& tile_metric_set)
{
    try {
        read_interop(filename,
                     tile_metric_set);
    }
    catch(const incomplete_file_exception&){}// Ignore incomplete files
    catch(const bad_format_exception& ex)
    {
        std::cerr << "InterOp did not have the expected format: " << ex.what() << std::endl;
        return 1;
    }
    catch(const file_not_found_exception& ex)
    {
        std::cerr << "Count not find InterOp file: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}


