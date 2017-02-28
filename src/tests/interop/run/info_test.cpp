/** Test parsing of the run info XML file
 *
 *  @file
 *  @date 11/7/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#include <vector>
#include <algorithm>
#include <gtest/gtest.h>
#include "interop/util/length_of.h"
#include "interop/constants/enums.h"
#include "interop/model/run/info.h"
#include "src/tests/interop/run/info_test.h"

using namespace illumina::interop::model;
using namespace illumina::interop;
using namespace illumina::interop::unittest;


/** Fixture to list enum values */
template<typename T>
struct run_info_fixture : public ::testing::Test, public T
{
    /** Constructor */
    run_info_fixture()
    {
    }
};
template<typename T>
struct read_fixture
{
    /** Constructor */
    read_fixture()
    {
        T::create_expected(expected_run_info);
        std::string xml_file;
        T::create_string(xml_file);
        run_info.parse(&xml_file[0]);
    }
    run::info expected_run_info;
    run::info run_info;
};
template<typename T>
struct write_read_fixture
{
    /** Constructor */
    write_read_fixture()
    {
        T::create_expected(expected_run_info);
        std::ostringstream fout;
        expected_run_info.write(fout);
        std::string tmp = fout.str();
        run_info.parse(&tmp[0]);
    }
    run::info expected_run_info;
    run::info run_info;
};
typedef ::testing::Types<
        read_fixture<miseq_run_info>,
        read_fixture<hiseq4k_run_info>,
        read_fixture<miniseq_run_info>,
        read_fixture<nextseq_550_run_info>,
        read_fixture<hiseq2500_run_info>,
        read_fixture<novaseq_run_info>,


        write_read_fixture<miseq_run_info>,
        write_read_fixture<hiseq4k_run_info>,
        write_read_fixture<miniseq_run_info>,
        write_read_fixture<nextseq_550_run_info>,
        write_read_fixture<hiseq2500_run_info>,
        write_read_fixture<novaseq_run_info>
> run_info_list;
TYPED_TEST_CASE(run_info_fixture, run_info_list);

/** Confirm that every enum does not conflict with unknown */
TYPED_TEST(run_info_fixture, run_info_test)
{
    EXPECT_EQ(TestFixture::run_info.name(), TestFixture::expected_run_info.name());
    EXPECT_EQ(TestFixture::run_info.date(), TestFixture::expected_run_info.date());
    EXPECT_EQ(TestFixture::run_info.instrument_name(), TestFixture::expected_run_info.instrument_name());
    EXPECT_EQ(TestFixture::run_info.flowcell_id(), TestFixture::expected_run_info.flowcell_id());
    EXPECT_EQ(TestFixture::run_info.run_number(), TestFixture::expected_run_info.run_number());
    EXPECT_EQ(TestFixture::run_info.version(), TestFixture::expected_run_info.version());
    EXPECT_EQ(TestFixture::run_info.flowcell().lane_count(), TestFixture::expected_run_info.flowcell().lane_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().surface_count(), TestFixture::expected_run_info.flowcell().surface_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().swath_count(), TestFixture::expected_run_info.flowcell().swath_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().tile_count(), TestFixture::expected_run_info.flowcell().tile_count());
    EXPECT_EQ(TestFixture::run_info.flowcell().sections_per_lane(), TestFixture::expected_run_info.flowcell().sections_per_lane());
    EXPECT_EQ(TestFixture::run_info.flowcell().lanes_per_section(), TestFixture::expected_run_info.flowcell().lanes_per_section());
    EXPECT_EQ(TestFixture::run_info.flowcell().naming_method(), TestFixture::expected_run_info.flowcell().naming_method());
    EXPECT_EQ(TestFixture::run_info.dimensions_of_image().width(), TestFixture::expected_run_info.dimensions_of_image().width());
    EXPECT_EQ(TestFixture::run_info.dimensions_of_image().height(), TestFixture::expected_run_info.dimensions_of_image().height());
    EXPECT_EQ(TestFixture::run_info.channels().size(), TestFixture::expected_run_info.channels().size());

    for(size_t i=0;i<std::min(TestFixture::run_info.channels().size(), TestFixture::expected_run_info.channels().size());++i)
        EXPECT_EQ(TestFixture::run_info.channels()[i], TestFixture::expected_run_info.channels()[i]);
    EXPECT_EQ(TestFixture::run_info.reads().size(), TestFixture::expected_run_info.reads().size());
    for(size_t i=0;i<std::min(TestFixture::run_info.reads().size(), TestFixture::expected_run_info.reads().size());++i)
    {
        EXPECT_EQ(TestFixture::run_info.reads()[i].number(), TestFixture::expected_run_info.reads()[i].number());
        EXPECT_EQ(TestFixture::run_info.reads()[i].is_index(), TestFixture::expected_run_info.reads()[i].is_index());
        EXPECT_EQ(TestFixture::run_info.reads()[i].first_cycle(), TestFixture::expected_run_info.reads()[i].first_cycle());
        EXPECT_EQ(TestFixture::run_info.reads()[i].last_cycle(), TestFixture::expected_run_info.reads()[i].last_cycle());
    }
    EXPECT_EQ(TestFixture::run_info.flowcell().tiles().size(), TestFixture::expected_run_info.flowcell().tiles().size());
    for(size_t i=0;i<std::min(TestFixture::run_info.flowcell().tiles().size(), TestFixture::expected_run_info.flowcell().tiles().size());++i)
    {
        EXPECT_EQ(TestFixture::run_info.flowcell().tiles()[i], TestFixture::expected_run_info.flowcell().tiles()[i]);
    }
}
