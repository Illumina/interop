/** Unit test fixtures for the run info
 *
 *
 *  @file
 *  @date 2/9/17
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include "interop/util/length_of.h"
#include "interop/model/run/info.h"


namespace illumina{ namespace interop { namespace unittest
{
    /** Provide an example MiSeq RunInfo v2
     */
    struct miseq_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\"?>\n"
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"
                    "  <Run Id=\"120705_M00903_0009_A000000000-A12V4\" Number=\"8\">\n"
                    "    <Flowcell>000000000-A12V4</Flowcell>\n"
                    "    <Instrument>M00903</Instrument>\n"
                    "    <Date>120705</Date>\n"
                    "    <Reads>\n"
                    "      <Read NumCycles=\"251\" Number=\"1\" IsIndexedRead=\"N\" />\n"
                    "      <Read NumCycles=\"12\" Number=\"2\" IsIndexedRead=\"Y\" />\n"
                    "      <Read NumCycles=\"251\" Number=\"3\" IsIndexedRead=\"N\" />\n"
                    "    </Reads>\n"
                    "    <FlowcellLayout LaneCount=\"1\" SurfaceCount=\"2\" SwathCount=\"1\" TileCount=\"14\" />\n"
                    "  </Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard MiSeq runinfo with an arbitrary read structure
         *
         * @param expected_run_info
         * @param reads
         */
        static void create_expected(model::run::info& expected_run_info,  const std::vector<model::run::read_info>& reads)
        {
            expected_run_info = model::run::info("120705_M00903_0009_A000000000-A12V4" /* run id */,
                                          "120705" /* date */ ,
                                          "M00903" /* instrument name*/,
                                          8 /* run number */,
                                          2 /** RunInfo version */,
                                          model::run::flowcell_layout(1 /*lane count*/,
                                                               2 /*surface count*/,
                                                               1 /*swath count */,
                                                               14 /*tile count */,
                                                               1 /* sections per lane */,
                                                               1 /* lanes per section*/,
                                                               std::vector<std::string> () /* Tiles */,
                                                               constants::UnknownTileNamingMethod,
                                                               "000000000-A12V4" /* flowcell id */),
                                          std::vector<std::string>() /*Image channels */,
                                          model::run::image_dimensions(/*Width*/ 0, /*Height*/ 0),
                                          reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       251,          false),
                    model::run::read_info(2,         252,      263,          true),
                    model::run::read_info(3,         264,      514,          false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };


    /** Provide an example HiSeq 2500 RunInfo v2
     */
    struct hiseq2500_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\"?>\n"
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"2\">\n"
                    "  <Run Id=\"151014_D00154_0539_AHCTK2BCXX_HiSeq 2500_TruSeq Exome_12 replicates of NA12878\" Number=\"539\">\n"
                    "    <Flowcell>HCTK2BCXX</Flowcell>\n"
                    "    <Instrument>D00154</Instrument>\n"
                    "    <Date>151014</Date>\n"
                    "    <Reads>\n"
                    "      <Read Number=\"1\" NumCycles=\"76\" IsIndexedRead=\"N\" />\n"
                    "      <Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\" />\n"
                    "      <Read Number=\"3\" NumCycles=\"76\" IsIndexedRead=\"N\" />\n"
                    "    </Reads>\n"
                    "    <FlowcellLayout LaneCount=\"2\" SurfaceCount=\"2\" SwathCount=\"2\" TileCount=\"16\" />\n"
                    "    <AlignToPhiX>\n"
                    "      <Lane>1</Lane>\n"
                    "      <Lane>2</Lane>\n"
                    "    </AlignToPhiX>\n"
                    "  </Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void create_expected(model::run::info& expected_run_info, const std::vector<model::run::read_info>& reads)
        {
            expected_run_info = model::run::info("151014_D00154_0539_AHCTK2BCXX_HiSeq 2500_TruSeq Exome_12 replicates of NA12878" /* run id */,
                                                 "151014" /* date */ ,
                                                 "D00154" /* instrument name*/,
                                                 539 /* run number */,
                                                 2 /** RunInfo version */,
                                                 model::run::flowcell_layout(2 /*lane count*/,
                                                                             2 /*surface count*/,
                                                                             2 /*swath count */,
                                                                             16 /*tile count */,
                                                                             1 /* sections per lane */,
                                                                             1 /* lanes per section*/,
                                                                             std::vector<std::string>() /* Tiles */,
                                                                             constants::UnknownTileNamingMethod,
                                                                             "HCTK2BCXX" /* flowcell id */),
                                                 std::vector<std::string>() /*Image channels */,
                                                 model::run::image_dimensions(/*Width*/ 0, /*Height*/ 0),
                                                 reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       76,          false),
                    model::run::read_info(2,         77,     84,         true),
                    model::run::read_info(3,         85,     160,         false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };

    /** Provide an example HiSeq 4k RunInfo v3
     */
    struct hiseq4k_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\"?>\n"
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"3\">\n"
                    "  <Run Id=\"160223_196_2371_AH7MF5BBXX\" Number=\"2371\">\n"
                    "    <Flowcell>H7MF5BBXX</Flowcell>\n"
                    "    <Instrument>196</Instrument>\n"
                    "    <Date>160223</Date>\n"
                    "    <Reads>\n"
                    "      <Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\" />\n"
                    "      <Read Number=\"2\" NumCycles=\"151\" IsIndexedRead=\"N\" />\n"
                    "    </Reads>\n"
                    "    <FlowcellLayout LaneCount=\"8\" SurfaceCount=\"2\" SwathCount=\"2\" TileCount=\"28\">\n"
                    "      <TileSet TileNamingConvention=\"FourDigit\">\n"
                    "        <Tiles>\n"
                    "          <Tile>1_1101</Tile>\n"
                    "          <Tile>1_1102</Tile>\n"
                    "        </Tiles>\n"
                    "      </TileSet>\n"
                    "    </FlowcellLayout>\n"
                    "    <AlignToPhiX />\n"
                    "    <ImageDimensions Width=\"3188\" Height=\"4826\" />\n"
                    "    <ImageChannels>\n"
                    "      <Name>A</Name>\n"
                    "      <Name>G</Name>\n"
                    "      <Name>T</Name>\n"
                    "      <Name>C</Name>\n"
                    "    </ImageChannels>\n"
                    "  </Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void create_expected(model::run::info& expected_run_info, const std::vector<model::run::read_info>& reads)
        {
            std::string tiles[] = {
                    "1_1101",
                    "1_1102"
            };
            const std::string channels[] = {"a", "g", "t", "c"};
            expected_run_info = model::run::info("160223_196_2371_AH7MF5BBXX" /* run id */,
                                          "160223" /* date */ ,
                                          "196" /* instrument name*/,
                                          2371 /* run number */,
                                          3 /** RunInfo version */,
                                          model::run::flowcell_layout(8 /*lane count*/,
                                                               2 /*surface count*/,
                                                               2 /*swath count */,
                                                               28 /*tile count */,
                                                               1 /* sections per lane */,
                                                               1 /* lanes per section*/,
                                                               util::to_vector(tiles) /* Tiles */,
                                                               constants::FourDigit,
                                                               "H7MF5BBXX" /* flowcell id */),
                                          util::to_vector(channels) /*Image channels */,
                                          model::run::image_dimensions(/*Width*/ 3188, /*Height*/ 4826),
                                          reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       151,          false),
                    model::run::read_info(2,         152,     302,         false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };

    /** Provide an example MiniSeq RunInfo v4
     */
    struct miniseq_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\"?>\n"
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"4\">\n"
                    "  <Run Id=\"160822_ML-P2-01_0042_A000H02MNK\" Number=\"42\">\n"
                    "    <Flowcell>000H02MNK</Flowcell>\n"
                    "    <Instrument>ML-P2-01</Instrument>\n"
                    "    <Date>160822</Date>\n"
                    "    <Reads>\n"
                    "      <Read Number=\"1\" NumCycles=\"150\" IsIndexedRead=\"N\" />\n"
                    "      <Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\" />\n"
                    "      <Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\" />\n"
                    "      <Read Number=\"4\" NumCycles=\"150\" IsIndexedRead=\"N\" />\n"
                    "    </Reads>\n"
                    "    <FlowcellLayout LaneCount=\"1\" SurfaceCount=\"2\" SwathCount=\"3\" TileCount=\"10\" SectionPerLane=\"1\" LanePerSection=\"1\">\n"
                    "      <TileSet TileNamingConvention=\"FiveDigit\">\n"
                    "        <Tiles>\n"
                    "          <Tile>1_11102</Tile>\n"
                    "          <Tile>1_21102</Tile>\n"
                    "        </Tiles>\n"
                    "      </TileSet>\n"
                    "    </FlowcellLayout>\n"
                    "    <ImageDimensions Width=\"2592\" Height=\"1944\" />\n"
                    "    <ImageChannels>\n"
                    "      <Name>Red</Name>\n"
                    "      <Name>Green</Name>\n"
                    "    </ImageChannels>\n"
                    "  </Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void create_expected(model::run::info& expected_run_info, const std::vector<model::run::read_info>& reads)
        {
            std::string tiles[] = {
                    "1_11102",
                    "1_21102"
            };
            const std::string channels[] = {"red", "green"};
            expected_run_info = model::run::info("160822_ML-P2-01_0042_A000H02MNK" /* run id */,
                                                 "160822" /* date */ ,
                                                 "ML-P2-01" /* instrument name*/,
                                                 42 /* run number */,
                                                 4 /** RunInfo version */,
                                                 model::run::flowcell_layout(1 /*lane count*/,
                                                                             2 /*surface count*/,
                                                                             3 /*swath count */,
                                                                             10 /*tile count */,
                                                                             1 /* sections per lane */,
                                                                             1 /* lanes per section*/,
                                                                             util::to_vector(tiles) /* Tiles */,
                                                                             constants::FiveDigit,
                                                                             "000H02MNK" /* flowcell id */),
                                                 util::to_vector(channels) /*Image channels */,
                                                 model::run::image_dimensions(/*Width*/ 2592, /*Height*/ 1944),
                                                 reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       150,         false),
                    model::run::read_info(2,         151,     158,         true),
                    model::run::read_info(3,         159,     166,         true),
                    model::run::read_info(4,         167,     316,         false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };


    /** Provide an example NextSeq 550 RunInfo v4
     */
    struct nextseq_550_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\"?>\n"
                    "<RunInfo xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" Version=\"4\">\n"
                    "  <Run Id=\"160630_NS500179_0099_AHW3YTBGXX\" Number=\"99\">\n"
                    "    <Flowcell>HW3YTBGXX</Flowcell>\n"
                    "    <Instrument>NS500179</Instrument>\n"
                    "    <Date>160630</Date>\n"
                    "    <Reads>\n"
                    "      <Read Number=\"1\" NumCycles=\"101\" IsIndexedRead=\"N\" />\n"
                    "      <Read Number=\"2\" NumCycles=\"6\" IsIndexedRead=\"Y\" />\n"
                    "      <Read Number=\"3\" NumCycles=\"101\" IsIndexedRead=\"N\" />\n"
                    "    </Reads>\n"
                    "    <FlowcellLayout LaneCount=\"4\" SurfaceCount=\"2\" SwathCount=\"3\" TileCount=\"12\" SectionPerLane=\"3\" LanePerSection=\"2\">\n"
                    "      <TileSet TileNamingConvention=\"FiveDigit\">\n"
                    "        <Tiles>\n"
                    "          <Tile>1_11101</Tile>\n"
                    "          <Tile>1_21101</Tile>\n"
                    "        </Tiles>\n"
                    "      </TileSet>\n"
                    "    </FlowcellLayout>\n"
                    "    <ImageDimensions Width=\"2592\" Height=\"1944\" />\n"
                    "    <ImageChannels>\n"
                    "      <Name>Red</Name>\n"
                    "      <Name>Green</Name>\n"
                    "    </ImageChannels>\n"
                    "  </Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void create_expected(model::run::info& expected_run_info, const std::vector<model::run::read_info>& reads)
        {
            std::string tiles[] = {
                    "1_11101",
                    "1_21101"
            };
            const std::string channels[] = {"red", "green"};
            expected_run_info = model::run::info("160630_NS500179_0099_AHW3YTBGXX" /* run id */,
                                                 "160630" /* date */ ,
                                                 "NS500179" /* instrument name*/,
                                                 99 /* run number */,
                                                 4 /** RunInfo version */,
                                                 model::run::flowcell_layout(4 /*lane count*/,
                                                                             2 /*surface count*/,
                                                                             3 /*swath count */,
                                                                             12 /*tile count */,
                                                                             3 /* sections per lane */,
                                                                             2 /* lanes per section*/,
                                                                             util::to_vector(tiles) /* Tiles */,
                                                                             constants::FiveDigit,
                                                                             "HW3YTBGXX" /* flowcell id */),
                                                 util::to_vector(channels) /*Image channels */,
                                                 model::run::image_dimensions(/*Width*/ 2592, /*Height*/ 1944),
                                                 reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       101,         false),
                    model::run::read_info(2,         102,     107,         true),
                    model::run::read_info(3,         108,     208,         false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };


    /** Provide an example NovaSeq RunInfo v5
     */
    struct novaseq_run_info
    {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string& data)
        {
            data= "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                    "<RunInfo>\n"
                    "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
                    "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
                    "\t\t<Instrument>XXXXXX</Instrument>\n"
                    "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
                    "\t\t<Reads>\n"
                    "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
                    "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
                    "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
                    "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
                    "\t\t</Reads>\n"
                    "\t\t<FlowcellLayout LaneCount=\"2\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" FlowcellSide=\"1\">\n"
                    "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
                    "\t\t\t\t<Tiles>\n"
                    "\t\t\t\t\t<Tile>1_2101</Tile>\n"
                    "\t\t\t\t\t<Tile>1_2102</Tile>\n"
                    "\t\t\t\t</Tiles>\n"
                    "\t\t\t</TileSet>\n"
                    "\t\t</FlowcellLayout>\n"
                    "\t\t<AlignToPhiX/>\n"
                    "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
                    "\t\t<ImageChannels>\n"
                    "\t\t\t<Name>RED</Name>\n"
                    "\t\t\t<Name>GREEN</Name>\n"
                    "\t\t</ImageChannels>\n"
                    "\t</Run>\n"
                    "</RunInfo>";
        }
        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void create_expected(model::run::info& expected_run_info, const std::vector<model::run::read_info>& reads)
        {
            std::string tiles[] = {
                    "1_2101",
                    "1_2102"
            };
            const std::string channels[] = {"red", "green"};
            expected_run_info = model::run::info("170208_XXXXXX_0021_YYYYYYYY" /* run id */,
                                                 "2/8/2017 4:25:41 PM" /* date */ ,
                                                 "XXXXXX" /* instrument name*/,
                                                 21 /* run number */,
                                                 0 /** RunInfo version */,
                                                 model::run::flowcell_layout(2 /*lane count*/,
                                                                             2 /*surface count*/,
                                                                             4 /*swath count */,
                                                                             88 /*tile count */,
                                                                             1 /* sections per lane */,
                                                                             1 /* lanes per section*/,
                                                                             util::to_vector(tiles) /* Tiles */,
                                                                             constants::FourDigit,
                                                                             "YYYYYYYY" /* flowcell id */),
                                                 util::to_vector(channels) /*Image channels */,
                                                 model::run::image_dimensions(/*Width*/ 3200, /*Height*/ 3607),
                                                 reads);
        }
        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info& expected_run_info)
        {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1,         1,       151,         false),
                    model::run::read_info(2,         152,     159,         true),
                    model::run::read_info(3,         160,     167,         true),
                    model::run::read_info(4,         168,     318,         false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };

    /** Provide an example NextSeq 1k2k RunInfo v6
     */
    struct nextseq1k2k_run_info {
        /** Generate a file representation of the RunInfo in a string
         *
         * @param data destination string
         */
        static void create_string(std::string &data) {
            data = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                   "<RunInfo Version=\"6\">\n"
                   "\t<Run Id=\"170208_XXXXXX_0021_YYYYYYYY\" Number=\"21\">\n"
                   "\t\t<Flowcell>YYYYYYYY</Flowcell>\n"
                   "\t\t<Instrument>XXXXXX</Instrument>\n"
                   "\t\t<Date>2/8/2017 4:25:41 PM</Date>\n"
                   "\t\t<Reads>\n"
                   "\t\t\t<Read Number=\"1\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
                   "\t\t\t<Read Number=\"2\" NumCycles=\"8\" IsIndexedRead=\"Y\"/>\n"
                   "\t\t\t<Read Number=\"3\" NumCycles=\"8\" IsIndexedRead=\"Y\" IsReverseComplement=\"Y\" />\n"
                   "\t\t\t<Read Number=\"4\" NumCycles=\"151\" IsIndexedRead=\"N\"/>\n"
                   "\t\t</Reads>\n"
                   "\t\t<FlowcellLayout LaneCount=\"2\" SurfaceCount=\"2\" SwathCount=\"4\" TileCount=\"88\" FlowcellSide=\"1\">\n"
                   "\t\t\t<TileSet TileNamingConvention=\"FourDigit\">\n"
                   "\t\t\t\t<Tiles>\n"
                   "\t\t\t\t\t<Tile>1_2101</Tile>\n"
                   "\t\t\t\t\t<Tile>1_2102</Tile>\n"
                   "\t\t\t\t</Tiles>\n"
                   "\t\t\t</TileSet>\n"
                   "\t\t</FlowcellLayout>\n"
                   "\t\t<AlignToPhiX/>\n"
                   "\t\t<ImageDimensions Width=\"3200\" Height=\"3607\"/>\n"
                   "\t\t<ImageChannels>\n"
                   "\t\t\t<Name>RED</Name>\n"
                   "\t\t\t<Name>GREEN</Name>\n"
                   "\t\t</ImageChannels>\n"
                   "\t</Run>\n"
                   "</RunInfo>";
        }

        /** Generate a standard runinfo with an arbitrary read structure
         *
         * @param expected_run_info destination run info
         * @param reads arbitrary read structure
         */
        static void
        create_expected(model::run::info &expected_run_info, const std::vector<model::run::read_info> &reads) {
            std::string tiles[] = {
                    "1_2101",
                    "1_2102"
            };
            const std::string channels[] = {"red", "green"};
            expected_run_info = model::run::info("170208_XXXXXX_0021_YYYYYYYY" /* run id */,
                                                 "2/8/2017 4:25:41 PM" /* date */ ,
                                                 "XXXXXX" /* instrument name*/,
                                                 21 /* run number */,
                                                 6 /** RunInfo version */,
                                                 model::run::flowcell_layout(2 /*lane count*/,
                                                                             2 /*surface count*/,
                                                                             4 /*swath count */,
                                                                             88 /*tile count */,
                                                                             1 /* sections per lane */,
                                                                             1 /* lanes per section*/,
                                                                             util::to_vector(tiles) /* Tiles */,
                                                                             constants::FourDigit,
                                                                             "YYYYYYYY" /* flowcell id */),
                                                 util::to_vector(channels) /*Image channels */,
                                                 model::run::image_dimensions(/*Width*/ 3200, /*Height*/ 3607),
                                                 reads);
        }

        /** Generate expected run info relating to file representation
         *
         * @param expected_run_info destination run info
         */
        static void create_expected(model::run::info &expected_run_info) {
            const model::run::read_info reads[] = {
                    /*Number,    IsIndexedRead, CycleStart, CycleEnd*/
                    model::run::read_info(1, 1, 151, false),
                    model::run::read_info(2, 152, 159, true),
                    model::run::read_info(3, 160, 167, true),
                    model::run::read_info(4, 168, 318, false)
            };
            create_expected(expected_run_info, util::to_vector(reads));
        }
    };
}}}


