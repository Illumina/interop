/** @page apps Applications
 *
 * This developer application is to help create unit tests. The dumpbin application simulates InterOp files, and writes
 * the binary files as a set command separated bytes to the console as text.
 *
 * Running the Program
 * -------------------
 *
 * The program runs as follows:
 *
 *      $ simulate 140131_1287_0851_A01n401drr
 *
 *      @TODO Move all test headers to main interop and use metric_set instead of vector -> reuse here
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/version.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

/** Exit codes that can be produced by the application
 */
enum exit_codes
{
    /** The program exited cleanly, 0 */
    SUCCESS,
    /** Invalid arguments were given to the application*/
    INVALID_ARGUMENTS,
    /** Empty InterOp directory*/
    NO_INTEROPS_FOUND,
    /** InterOp file has a bad format */
    BAD_FORMAT,
    /** Unknown error has occurred*/
    UNEXPECTED_EXCEPTION,
    /** InterOp file has not records */
    EMPTY_INTEROP
};

/** Call back functor for writing binary metric data as a string
 */
struct metric_writer
{
    /** Constructor
     *
     * @param out output stream
     * @param max_line number of characters before wrapping
     */
    metric_writer(std::ostream& out, const size_t max_line=80) : m_out(out), m_max_line(max_line){}
    /** Function operator overload to write data
     *
     * @param metrics set of metrics
     */
    template<class MetricSet>
    void operator()(const MetricSet& metrics)const
    {
        if(metrics.size()==0) return;
        std::ostringstream fout;
        io::write_metrics(fout, metrics, metrics.version());
        m_out << metrics.prefix() << std::endl;
        write_bytes_as_string(m_out, fout.str(), m_max_line);
    }
private:
    static void write_bytes_as_string(std::ostream& out, const std::string& buffer, const size_t k_max_line)
    {
        std::string::const_iterator sit = buffer.begin();
        if (sit == buffer.end()) return;

        out << ::int16_t(*sit);
        size_t char_count = length_of(::int16_t(*sit));
        for (++sit; sit != buffer.end(); ++sit, ++char_count) {
            const ::int16_t val = ::int16_t(*sit);
            char_count += 1 + length_of(val);
            if (char_count > k_max_line) {
                out << "\n";
                char_count = 1 + length_of(val);
            }
            out << "," << val;
        }
        out << std::endl;

    }
    static size_t length_of(const ::int16_t val)
    {
        if(val <= -100) return 4;
        if(val >= 100) return 3;
        if(val <= -10) return 3;
        if(val >= 10) return 2;
        if(val < 0) return 2;
        return 1;
    }
private:
    std::ostream& m_out;
    size_t m_max_line;

};

void simulate_metrics(run_metrics& run);

int main(int, char**)
{

    metric_writer write_metrics(std::cout);
    std::cout << "# Version: " << INTEROP_VERSION << std::endl;
    run_metrics subset;
    simulate_metrics(subset);
    try{
        subset.metrics_callback(write_metrics);
    }catch(const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << std::endl;
        return UNEXPECTED_EXCEPTION;
    }
    std::cout << "\n" << std::endl;
    return SUCCESS;
}

/** Convert an array to a vector
 *
 * Determines the length of the stack array automatically.
 *
 * @param vals array pointer
 * @return vector of values
 */
template<typename T, size_t N>
static std::vector<T> to_vector(const T (&vals)[N])
{
    return std::vector<T>(vals, vals + N);
}

void simulate_metrics(run_metrics& run)
{
    typedef model::metric_base::metric_set< extraction_metric > extraction_metric_set_t;
    typedef model::metric_base::metric_set< image_metric > image_metric_set_t;
    typedef model::metric_base::metric_set< index_metric > index_metric_set_t;
    typedef model::metric_base::metric_set< phasing_metric > phasing_metric_set_t;
    typedef model::metric_base::metric_set< q_metric > q_metric_set_t;
    typedef model::metric_base::metric_set< tile_metric > tile_metric_set_t;


    run.get<extraction_metric>() = extraction_metric_set_t(extraction_metric::header_type(2), 3);
    typedef extraction_metric::ushort_t ushort_t;
    const ushort_t p90_1[]  = {877,518};
    const float focus1[] = {2.14784f,2.12109f};
    const ushort_t p90_2[]  = {862,508};
    const float focus2[] = {2.1639f,2.08307f};
    const ushort_t p90_3[]  = {887,519};
    const float focus3[] = {2.15751f,2.09462f};
    run.get<extraction_metric>().insert(extraction_metric(3,211011,1, to_vector(p90_1), to_vector(focus1)));
    run.get<extraction_metric>().insert(extraction_metric(3,211012,1, to_vector(p90_2), to_vector(focus2)));
    run.get<extraction_metric>().insert(extraction_metric(3,211013,1, to_vector(p90_3), to_vector(focus3)));

    const size_t channel_count = 2;
    const ushort_t min_contrast1[]  = {231, 207};
    const ushort_t min_contrast2[]  = {229, 205};
    const ushort_t min_contrast3[]  = {231, 222};

    const ushort_t max_contrast1[]  = {462, 387};
    const ushort_t max_contrast2[]  = {457, 387};
    const ushort_t max_contrast3[]  = {473, 416};
    run.get<image_metric>() = image_metric_set_t(image_metric::header_type(2), 3);
    run.get<image_metric>().insert(image_metric(7, 111014, 1, channel_count, to_vector(min_contrast1), to_vector(max_contrast1)));
    run.get<image_metric>().insert(image_metric(7, 121014, 1, channel_count, to_vector(min_contrast2), to_vector(max_contrast2)));
    run.get<image_metric>().insert(image_metric(7, 211014, 1, channel_count, to_vector(min_contrast3), to_vector(max_contrast3)));


    index_metric::index_array_t indices1;
    indices1.push_back(index_metric::index_info_t ("ATCACGAC-AAGGTTCA", "1", "TSCAIndexes", 4570));
    index_metric::index_array_t indices2;
    indices2.push_back(index_metric::index_info_t ("ACAGTGGT-AAGGTTCA", "2", "TSCAIndexes", 4477));
    index_metric::index_array_t indices3;
    indices3.push_back(index_metric::index_info_t ("CAGATCCA-AAGGTTCA", "3", "TSCAIndexes", 4578));
    run.get<index_metric>() = index_metric_set_t(index_metric::header_type(), 2);
    run.get<index_metric>().insert(index_metric(1, 121106, 3, indices1));
    run.get<index_metric>().insert(index_metric(2, 121106, 3, indices2));
    run.get<index_metric>().insert(index_metric(3, 121106, 3, indices3));


    run.get<phasing_metric>() = phasing_metric_set_t(phasing_metric::header_type(), 2);
    run.get<phasing_metric>().insert(phasing_metric(4, 1116, 2, 1.16406322f, 1.92952883f));
    run.get<phasing_metric>().insert(phasing_metric(4, 1116, 3, 0.582031608f, 2.12364316f));
    run.get<phasing_metric>().insert(phasing_metric(4, 1116, 4, 0.291015804f, 2.20452404f));


    typedef q_metric::uint_t uint_t;
    const uint_t hist_all1[] = {0, 267962, 118703, 4284, 2796110, 0, 0};
    const uint_t hist_all2[] = {0,241483, 44960, 1100, 2899568, 0 ,0};
    const uint_t hist_all3[] = {0,212144, 53942, 427, 2920598, 0, 0};
    std::vector<uint_t> hist_tmp(50, 0);
    typedef q_metric::header_type::qscore_bin_vector_type qscore_bin_vector_type;
    typedef q_metric::header_type::bin_t bin_t;
    typedef bin_t::bin_type bin_type;
    typedef q_metric::uint_t uint_t;
    const uint_t bin_count = 7;

    const bin_type lower[] = {2, 10, 20, 25, 30, 35, 40};
    const bin_type upper[] = {9, 19, 24, 29, 34, 39, 40};
    const bin_type value[] = {2, 14, 21, 27, 32, 36, 40};
    qscore_bin_vector_type headervec;
    for(uint_t i=0;i<bin_count;i++)
        headervec.push_back(bin_t(lower[i], upper[i], value[i]));
    run.get<q_metric>() = q_metric_set_t(q_metric::header_type(headervec), 7);
    run.get<q_metric>().insert(q_metric(7, 111014, 1, to_vector(hist_all1)));
    run.get<q_metric>().insert(q_metric(7, 111014, 2, to_vector(hist_all2)));
    run.get<q_metric>().insert(q_metric(7, 111014, 3, to_vector(hist_all3)));

    tile_metric::read_metric_vector reads1;
    reads1.push_back(tile_metric::read_metric_type(1, 2.61630869f, 0.0797112584f, 0.119908921f));
    reads1.push_back(tile_metric::read_metric_type(2, 2.61630869f, 0.0797112584f, 0.119908921f));

    run.get<tile_metric>() = tile_metric_set_t(tile_metric::header_type(2.7476099f), 3);
    run.get<tile_metric>().insert(tile_metric(7, 111014, 2355119.25f,1158081.50f,6470949,3181956,reads1));
    run.get<tile_metric>().insert(tile_metric(7, 121014, 2355119.25f,1174757.75f,6470949,3227776,
                                              tile_metric::read_metric_vector(1, tile_metric::read_metric_type(1, 2.62243795f, 0.129267812f, 0.135128692f))));
    run.get<tile_metric>().insert(tile_metric(7, 211014, 2355119.25f,1211592.38f,6470949,3328983,
                                              tile_metric::read_metric_vector(1, tile_metric::read_metric_type(1, 2.490309f, 0.11908555f, 0.092706576f))));
}


