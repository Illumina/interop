/** Test parsing of the run parameters XML file
 *
 *  @file
 *  @date 11/9/2015
 *  @version 1.0
 *  @copyright GNU Public License.
 */


#include <vector>
#include <gtest/gtest.h>
#include "interop/constants/enums.h"
#include "interop/model/run/parameters.h"

using namespace illumina::interop::model;
using namespace illumina::interop;

static void test_helper_run_parameter_parse_xml(const run::parameters& expected_param,
                                                const run::parameters& actual_param)
{
    EXPECT_NE(actual_param.instrument_type(), constants::UnknownInstrument);
    EXPECT_EQ(actual_param.instrument_type(), expected_param.instrument_type());
    EXPECT_EQ(actual_param.version(), expected_param.version());
}

static void test_helper_run_parameter_parse_xml(const run::parameters& expected_param,
                                                std::string& xml_file)
{
    run::parameters actual_param;
    actual_param.parse(&xml_file[0]);
    test_helper_run_parameter_parse_xml(expected_param, actual_param);
}


/**120705_11618Unbin1R2I
 * @class illumina::interop::model::run::parameters
 * @test Confirm MiSeq version of the XML format is properly
 *
 */
TEST(RunParameters, ParseXML_MiSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
        "<RunParameters xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
        "  <EnableCloud>false</EnableCloud>\n"
        "  <Barcode>000000000-A12V4</Barcode>\n"
        "  <CopyManifests>true</CopyManifests>\n"
        "  <FlowcellRFIDTag>\n"
        "    <SerialNumber>000000000-A12V4</SerialNumber>\n"
        "    <PartNumber>15028382</PartNumber>\n"
        "    <ExpirationDate>2012-10-09T00:00:00</ExpirationDate>\n"
        "  </FlowcellRFIDTag>\n"
        "  <PR2BottleRFIDTag>\n"
        "    <SerialNumber>MS2106444-00PR2</SerialNumber>\n"
        "    <PartNumber>15023445</PartNumber>\n"
        "    <ExpirationDate>2013-06-18T00:00:00</ExpirationDate>\n"
        "  </PR2BottleRFIDTag>\n"
        "  <ReagentKitRFIDTag>\n"
        "    <ExpirationDate>0001-01-01T00:00:00</ExpirationDate>\n"
        "  </ReagentKitRFIDTag>\n"
        "  <Resumable>true</Resumable>\n"
        "  <ManifestFiles />\n"
        "  <AfterRunWashMethod>Post-Run Wash</AfterRunWashMethod>\n"
        "  <Setup>\n"
        "    <SupportMultipleSurfacesInUI>false</SupportMultipleSurfacesInUI>\n"
        "    <ApplicationVersion>90.0.1.44</ApplicationVersion>\n"
        "    <NumTilesPerSwath>14</NumTilesPerSwath>\n"
        "    <NumSwaths>1</NumSwaths>\n"
        "    <NumLanes>1</NumLanes>\n"
        "    <Read1>0</Read1>\n"
        "    <Index1Read>0</Index1Read>\n"
        "    <Index2Read>0</Index2Read>\n"
        "    <Read2>0</Read2>\n"
        "    <ApplicationName>MiSeq Control Software</ApplicationName>\n"
        "  </Setup>\n"
        "  <RunID>120705_M00903_0009_A000000000-A12V4</RunID>\n"
        "  <ScannerID>M00903</ScannerID>\n"
        "  <RunNumber>8</RunNumber>\n"
        "  <FPGAVersion>9.5.10</FPGAVersion>\n"
        "  <CPLD>Not Specified</CPLD>\n"
        "  <CameraModel>Not Specified</CameraModel>\n"
        "  <CameraDriverVersion>Not Specified</CameraDriverVersion>\n"
        "  <CameraFirmwareVersion>Not Specified</CameraFirmwareVersion>\n"
        "  <MCSVersion>90.0.1.44</MCSVersion>\n"
        "  <RTAVersion>1.16.18</RTAVersion>\n"
        "  <PR2BottleBarcode>MS2106444-00PR2</PR2BottleBarcode>\n"
        "  <ReagentKitPartNumberEntered />\n"
        "  <ReagentKitVersion>Version2</ReagentKitVersion>\n"
        "  <ReagentKitBarcode>AA1234567-12345</ReagentKitBarcode>\n"
        "  <PreviousPR2BottleBarcode />\n"
        "  <PreviousReagentKitBarcode />\n"
        "  <ExperimentName />\n"
        "  <Chemistry>Default</Chemistry>\n"
        "  <Username>XXXXX</Username>\n"
        "  <SelectedTiles>\n"
        "    <int>0</int>\n"
        "  </SelectedTiles>\n"
        "  <Workflow>\n"
        "    <Analysis>Metagenomics</Analysis>\n"
        "  </Workflow>\n"
        "  <EnableAnalysis>false</EnableAnalysis>\n"
        "  <Reads>\n"
        "    <RunInfoRead Number=\"1\" NumCycles=\"251\" IsIndexedRead=\"N\" />\n"
        "    <RunInfoRead Number=\"2\" NumCycles=\"12\" IsIndexedRead=\"Y\" />\n"
        "    <RunInfoRead Number=\"3\" NumCycles=\"251\" IsIndexedRead=\"N\" />\n"
        "  </Reads>\n"
        "  <TempFolder>D:\\Illumina\\MiSeqTemp\\120705_M00903_0009_A000000000-A12V4</TempFolder>\n"
        "  <AnalysisFolder>D:\\Illumina\\MiSeqAnalysis\\120705_M00903_0009_A000000000-A12V4</AnalysisFolder>\n"
        "  <RunStartDate>120705</RunStartDate>\n"
        "  <MostRecentWashType>PostRun</MostRecentWashType>\n"
        "  <RecipeFolder>D:\\Illumina\\MiSeq Control Software\\CustomRecipe</RecipeFolder>\n"
        "  <ILMNOnlyRecipeFolder>C:\\Illumina\\MiSeq Control Software\\Recipe</ILMNOnlyRecipeFolder>\n"
        "  <SampleSheetName>SampleSheet_Miseq</SampleSheetName>\n"
        "  <SampleSheetFolder>C:\\Illumina\\MiSeq Control Software\\SampleSheets</SampleSheetFolder>\n"
        "  <ManifestFolder>D:\\Illumina\\MiSeq Control Software\\Manifests</ManifestFolder>\n"
        "  <OutputFolder>Z:\\120705_M00903_0009_A000000000-A12V4</OutputFolder>\n"
        "  <FlowcellName>A</FlowcellName>\n"
        "  <FocusMethod>AutoFocus</FocusMethod>\n"
        "  <SurfaceToScan>Both</SurfaceToScan>\n"
        "  <SaveFocusImages>true</SaveFocusImages>\n"
        "  <SaveScanImages>true</SaveScanImages>\n"
        "  <CloudUsername />\n"
        "  <CloudRunId>398407</CloudRunId>\n"
        "  <SendInstrumentHealthToILMN>true</SendInstrumentHealthToILMN>\n"
        "  <AmpliconWorkflowType />\n"
        "</RunParameters>";


    const run::parameters::uint_t Version = 0;
    const run::parameters::instrument_type_t ApplicationName_and_SupportMultipleSurfacesInUI=constants::MiSeq;
    const run::parameters expected_param(Version,
                                   ApplicationName_and_SupportMultipleSurfacesInUI);

    test_helper_run_parameter_parse_xml(expected_param, xml_file);
}

/** 131212_221Bin1R0I
 * @class illumina::interop::model::run::parameters
 * @test Confirm HiSeq version of the XML format is properly
 *
 */
TEST(RunParameters, ParseXML_HiSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
            "<RunParameters xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
            "  <Setup>\n"
            "    <ExperimentName>xxxxx with HCS 288.1 no reverse image flip, xxxx=1.391</ExperimentName>\n"
            "    <ScanID>-999</ScanID>\n"
            "    <FCPosition>A</FCPosition>\n"
            "    <WorkFlowType>NOINDEX</WorkFlowType>\n"
            "    <PairEndFC>false</PairEndFC>\n"
            "    <Read1>-1</Read1>\n"
            "    <IndexRead1>-1</IndexRead1>\n"
            "    <IndexRead2>-1</IndexRead2>\n"
            "    <Read2>-1</Read2>\n"
            "    <OutputFolder>\\\\ussd-prd-isi01\\sapphire\\system</OutputFolder>\n"
            "    <CompressBcls>false</CompressBcls>\n"
            "    <RemapQScores />\n"
            "    <NearNeighborCorrection>true</NearNeighborCorrection>\n"
            "    <PeriodicSave>RTA2 Save Images and Thumbnails Lanes 1-4</PeriodicSave>\n"
            "    <Flowcell>Sapphire TSMC v3</Flowcell>\n"
            "    <FirstBaseConfirmation>false</FirstBaseConfirmation>\n"
            "    <SampleSheet />\n"
            "    <ControlLane>0</ControlLane>\n"
            "    <KeepIntensityFiles>false</KeepIntensityFiles>\n"
            "    <AlignToPhiX>\n"
            "      <Lane>1</Lane>\n"
            "      <Lane>2</Lane>\n"
            "      <Lane>3</Lane>\n"
            "      <Lane>4</Lane>\n"
            "      <Lane>5</Lane>\n"
            "      <Lane>6</Lane>\n"
            "      <Lane>7</Lane>\n"
            "      <Lane>8</Lane>\n"
            "    </AlignToPhiX>\n"
            "    <ClusteringChoice>None</ClusteringChoice>\n"
            "    <RunMode>HighOutput</RunMode>\n"
            "    <Rehyb>None</Rehyb>\n"
            "    <PerformPreRunFluidicsCheck>false</PerformPreRunFluidicsCheck>\n"
            "    <ServiceRun>false</ServiceRun>\n"
            "    <ApplicationName>HiSeq Control Software</ApplicationName>\n"
            "    <ApplicationVersion>0.0.287.2</ApplicationVersion>\n"
            "    <RunID>131212_1270_0870_A02D224</RunID>\n"
            "    <RunStartDate>131212</RunStartDate>\n"
            "    <BaseSpaceSettings>\n"
            "      <Username />\n"
            "      <RunId />\n"
            "      <TempFolder />\n"
            "      <UseBaseSpace>false</UseBaseSpace>\n"
            "      <SendInstrumentHealthToILMN>false</SendInstrumentHealthToILMN>\n"
            "      <RunMonitoringOnly>false</RunMonitoringOnly>\n"
            "    </BaseSpaceSettings>\n"
            "    <ScannerID>1270</ScannerID>\n"
            "    <ScanNumber>870</ScanNumber>\n"
            "    <ComputerName>HS3000-1270</ComputerName>\n"
            "    <FPGAVersion>10.32.11</FPGAVersion>\n"
            "    <CPLDVersion>3.0.0</CPLDVersion>\n"
            "    <RTAVersion>2.0.33</RTAVersion>\n"
            "    <ChemistryVersion>Illumina,xxxx Fluidics Controller,0,v2.0340</ChemistryVersion>\n"
            "    <CameraFirmware>2.10-B00-R01</CameraFirmware>\n"
            "    <CameraDriver>7.0.120.4385</CameraDriver>\n"
            "    <FocusCameraFirmware />\n"
            "    <Barcode>02D224</Barcode>\n"
            "    <Username>XXXX</Username>\n"
            "    <SelectedSections>\n"
            "      <Section Name=\"A_1\" />\n"
            "      <Section Name=\"B_1\" />\n"
            "      <Section Name=\"C_1\" />\n"
            "      <Section Name=\"D_1\" />\n"
            "      <Section Name=\"E_1\" />\n"
            "      <Section Name=\"F_1\" />\n"
            "      <Section Name=\"G_1\" />\n"
            "      <Section Name=\"H_1\" />\n"
            "    </SelectedSections>\n"
            "    <FocusMethod>DynamicITF</FocusMethod>\n"
            "    <SelectedSurface>BothLaneSurfaces</SelectedSurface>\n"
            "    <SwathScanMode>DualSwathFC</SwathScanMode>\n"
            "    <EnableLft>true</EnableLft>\n"
            "    <AutoTiltOnce>true</AutoTiltOnce>\n"
            "    <EnableAutoCenter>true</EnableAutoCenter>\n"
            "    <EnableAnalysis>true</EnableAnalysis>\n"
            "    <EnableBasecalling>true</EnableBasecalling>\n"
            "    <EnableCameraLogging>false</EnableCameraLogging>\n"
            "    <AdapterPlate>HiSeq Adapter Plate</AdapterPlate>\n"
            "    <SlideHolder>HiSeq Flow Cell Holder</SlideHolder>\n"
            "    <TemplateCycleCount>0</TemplateCycleCount>\n"
            "    <SerialTemplateBuilding>true</SerialTemplateBuilding>\n"
            "    <NumAnalysisThreads>8</NumAnalysisThreads>\n"
            "    <FPGADynamicFocusSettings>\n"
            "      <MaxInitialZJumpHalfUm>3</MaxInitialZJumpHalfUm>\n"
            "      <MaxSubsequentZJumpHalfUm>7</MaxSubsequentZJumpHalfUm>\n"
            "      <NumberOfInitialZJumps>0</NumberOfInitialZJumps>\n"
            "      <CVGainStart>350</CVGainStart>\n"
            "      <CVGainPosLocked>350</CVGainPosLocked>\n"
            "      <Offset>150</Offset>\n"
            "      <HotPixel>200</HotPixel>\n"
            "      <MotorDelayFrames>21</MotorDelayFrames>\n"
            "      <SoftwareLaserLag>218</SoftwareLaserLag>\n"
            "      <DitherSize>100</DitherSize>\n"
            "      <GroupSize>50</GroupSize>\n"
            "      <DitherShift>20</DitherShift>\n"
            "      <IntensityCeiling>65535</IntensityCeiling>\n"
            "      <IGain>50</IGain>\n"
            "      <IHistory>4</IHistory>\n"
            "    </FPGADynamicFocusSettings>\n"
            "    <TileWidth>3200</TileWidth>\n"
            "    <TileHeight>7244</TileHeight>\n"
            "    <ImageWidth>3200</ImageWidth>\n"
            "    <ImageHeight>174420</ImageHeight>\n"
            "    <LaneLength>60.175</LaneLength>\n"
            "    <NumTilesPerSwath>24</NumTilesPerSwath>\n"
            "    <NumSwaths>2</NumSwaths>\n"
            "    <UseExistingRecipe>true</UseExistingRecipe>\n"
            "    <Reads>\n"
            "      <Read Number=\"1\" NumCycles=\"27\" IsIndexedRead=\"N\" />\n"
            "      <Read Number=\"2\" NumCycles=\"9\" IsIndexedRead=\"Y\" />\n"
            "    </Reads>\n"
            "    <EnableNotifications>false</EnableNotifications>\n"
            "    <ReagentKits>\n"
            "      <Sbs>\n"
            "        <SbsReagentKit>\n"
            "          <ID>1</ID>\n"
            "          <Prime>false</Prime>\n"
            "          <NumberCyclesRemaining>209</NumberCyclesRemaining>\n"
            "          <IsNew50Cycle>false</IsNew50Cycle>\n"
            "          <IsNew200Cycle>true</IsNew200Cycle>\n"
            "        </SbsReagentKit>\n"
            "      </Sbs>\n"
            "      <Index>\n"
            "        <ReagentKit>\n"
            "          <ID>1</ID>\n"
            "        </ReagentKit>\n"
            "      </Index>\n"
            "      <Pe />\n"
            "      <Rehyb />\n"
            "    </ReagentKits>\n"
            "    <ReagentBottles>\n"
            "      <Sbs />\n"
            "    </ReagentBottles>\n"
            "    <Resume>false</Resume>\n"
            "    <ResumeCycle>0</ResumeCycle>\n"
            "    <RecipeFilePath>C:\\Users\\XXXX\\Desktop\\SF Recipes\\1x26_XXXXX_xxxminSBS_noILpump_v2_single indexed.xml</RecipeFilePath>\n"
            "    <SupportMultipleSurfacesInUI>true</SupportMultipleSurfacesInUI>\n"
            "    <TempFolder>D:\\Illumina\\HiSeqTemp\\131212_1270_0870_A02D224</TempFolder>\n"
            "    <RecipeFragmentVersion>1.4.9</RecipeFragmentVersion>\n"
            "    <PromptForPeReagents>false</PromptForPeReagents>\n"
            "    <MockRun>false</MockRun>\n"
            "    <ImageRegistrationConfiguration>\n"
            "      <BorderPaddingX>-1</BorderPaddingX>\n"
            "      <BorderPaddingY>-1</BorderPaddingY>\n"
            "      <RegistrationLowFrequencyFilter>true</RegistrationLowFrequencyFilter>\n"
            "      <TapWidth>200</TapWidth>\n"
            "      <EnableFixTapBoundaries>true</EnableFixTapBoundaries>\n"
            "      <AreaPerPixelmm2>1.19025E-07</AreaPerPixelmm2>\n"
            "    </ImageRegistrationConfiguration>\n"
            "    <RigidRegistrationConfiguration>\n"
            "      <UseRigidRegistration>true</UseRigidRegistration>\n"
            "      <UpperLeftFiducialCenterXInPixels>1085</UpperLeftFiducialCenterXInPixels>\n"
            "      <UpperLeftFiducialCenterYInPixels>1810</UpperLeftFiducialCenterYInPixels>\n"
            "      <RigidRegistrationPitchInPixels>2.03</RigidRegistrationPitchInPixels>\n"
            "      <RigidRegistrationNumberOfColumns>507</RigidRegistrationNumberOfColumns>\n"
            "      <RigidRegistrationNumberOfRows>2063</RigidRegistrationNumberOfRows>\n"
            "      <RigidRegistrationFiducialVersion>v3_26X</RigidRegistrationFiducialVersion>\n"
            "      <TileHeight>7244</TileHeight>\n"
            "      <NumberOfTile>24</NumberOfTile>\n"
            "      <RigidRegistrationThirdOrderCoefficients>\n"
            "        <double>1.285592E-09</double>\n"
            "        <double>-1.942569E-07</double>\n"
            "        <double>-3.762933E-05</double>\n"
            "        <double>1.238717E-09</double>\n"
            "        <double>1.369931E-07</double>\n"
            "        <double>-6.325432E-05</double>\n"
            "        <double>1.613717E-09</double>\n"
            "        <double>-1.426944E-07</double>\n"
            "        <double>-0.0001773168</double>\n"
            "        <double>1.410592E-09</double>\n"
            "        <double>8.855557E-08</double>\n"
            "        <double>-4.169183E-05</double>\n"
            "      </RigidRegistrationThirdOrderCoefficients>\n"
            "      <RigidRegistrationSecondOrderCoefficients>\n"
            "        <double>-3.6875E-07</double>\n"
            "        <double>5.4325E-07</double>\n"
            "        <double>1.3125</double>\n"
            "        <double>-5.485E-07</double>\n"
            "        <double>-1.0925E-07</double>\n"
            "        <double>0.4375</double>\n"
            "        <double>3.74E-07</double>\n"
            "        <double>5.312E-08</double>\n"
            "        <double>0</double>\n"
            "        <double>2.25E-07</double>\n"
            "        <double>2.5575E-07</double>\n"
            "        <double>0</double>\n"
            "      </RigidRegistrationSecondOrderCoefficients>\n"
            "    </RigidRegistrationConfiguration>\n"
            "    <ScannedBarcode />\n"
            "  </Setup>\n"
            "  <Version>1</Version>\n"
            "</RunParameters>";


    const run::parameters::uint_t Version = 1;
    const run::parameters::instrument_type_t ApplicationName_and_SupportMultipleSurfacesInUI=constants::HiSeq;
    const run::parameters expected_param(Version,
                                   ApplicationName_and_SupportMultipleSurfacesInUI);

    test_helper_run_parameter_parse_xml(expected_param, xml_file);
}

/**6471489_213Bin1R0I
 * @class illumina::interop::model::run::parameters
 * @test Confirm NexSeq version of the XML format is properly
 *
 */
TEST(RunParameters, ParseXML_NextSeq)
{
    std::string xml_file = "<?xml version=\"1.0\"?>\n"
            "<RunParameters xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"
            "  <Setup>\n"
            "    <SupportMultipleSurfacesInUI>true</SupportMultipleSurfacesInUI>\n"
            "    <ApplicationVersion>1.2.1.7</ApplicationVersion>\n"
            "    <ApplicationName>NextSeq Control Software</ApplicationName>\n"
            "    <NumTilesPerSwath>12</NumTilesPerSwath>\n"
            "    <NumSwaths>3</NumSwaths>\n"
            "    <NumLanes>4</NumLanes>\n"
            "    <Read1>75</Read1>\n"
            "    <Read2>0</Read2>\n"
            "    <Index1Read>0</Index1Read>\n"
            "    <Index2Read>0</Index2Read>\n"
            "    <SectionPerLane>3</SectionPerLane>\n"
            "    <LanePerSection>2</LanePerSection>\n"
            "  </Setup>\n"
            "  <RunID>140502_NS500128_0072_AH0MNBAGXX_xxxx_1.6pM</RunID>\n"
            "  <InstrumentID>NS500128</InstrumentID>\n"
            "  <RunNumber>72</RunNumber>\n"
            "  <RTAVersion>2.1.3</RTAVersion>\n"
            "  <FlowCellSerial>XXXXXXXXX</FlowCellSerial>\n"
            "  <PR2BottleSerial>BU0123456-12345</PR2BottleSerial>\n"
            "  <ReagentKitSerial>RN0123456-12345</ReagentKitSerial>\n"
            "  <ReagentKitSerialWasEnteredInBaseSpace>false</ReagentKitSerialWasEnteredInBaseSpace>\n"
            "  <ExperimentName>H0MNBAGXX_xxxxx_1.6pM</ExperimentName>\n"
            "  <LibraryID />\n"
            "  <Chemistry>NextSeq High</Chemistry>\n"
            "  <SelectedTiles>\n"
            "    <int>0</int>\n"
            "  </SelectedTiles>\n"
            "  <RunFolder>D:\\Illumina\\NextSeq Control Software Temp\\140502_NS500128_0072_AH0MNBAGXX_xxxx_1.6pM\\</RunFolder>\n"
            "  <OutputFolder>\\\\sd-isilon\\XXXX\\XXXX\\SeqRuns\\140502_NS500128_0072_AH0MNBAGXX_xxxx_1.6pM\\</OutputFolder>\n"
            "  <RecipeFolder>C:\\Illumina\\NextSeq Control Software\\Recipe\\Custom\\High</RecipeFolder>\n"
            "  <SimulationFolder />\n"
            "  <RunStartDate>140502</RunStartDate>\n"
            "  <FocusMethod>IXFocus</FocusMethod>\n"
            "  <SurfaceToScan>Both</SurfaceToScan>\n"
            "  <SaveFocusImages>false</SaveFocusImages>\n"
            "  <SaveScanImages>false</SaveScanImages>\n"
            "  <SelectiveSave>true</SelectiveSave>\n"
            "  <IsPairedEnd>false</IsPairedEnd>\n"
            "  <AnalysisWorkflowType />\n"
            "  <CustomReadOnePrimer>BP10</CustomReadOnePrimer>\n"
            "  <CustomReadTwoPrimer>BP11</CustomReadTwoPrimer>\n"
            "  <CustomIndexPrimer>BP12</CustomIndexPrimer>\n"
            "  <CustomIndexTwoPrimer>EXT</CustomIndexTwoPrimer>\n"
            "  <UsesCustomPrimer1>false</UsesCustomPrimer1>\n"
            "  <UsesCustomPrimer2>false</UsesCustomPrimer2>\n"
            "  <UsesCustomIndexPrimer>false</UsesCustomIndexPrimer>\n"
            "  <RunManagementType>Standalone</RunManagementType>\n"
            "  <BaseSpaceRunId>6471489</BaseSpaceRunId>\n"
            "  <BaseSpaceRunMode>IlluminaHealthOnly</BaseSpaceRunMode>\n"
            "  <ComputerName>NS500-128</ComputerName>\n"
            "  <SequencingStarted>true</SequencingStarted>\n"
            "  <SequencingCompletedOrAborted>false</SequencingCompletedOrAborted>\n"
            "  <SequencingCompletedWithoutErrors>false</SequencingCompletedWithoutErrors>\n"
            "  <PlannedRead1Cycles>75</PlannedRead1Cycles>\n"
            "  <PlannedRead2Cycles>0</PlannedRead2Cycles>\n"
            "  <PlannedIndex1ReadCycles>0</PlannedIndex1ReadCycles>\n"
            "  <PlannedIndex2ReadCycles>0</PlannedIndex2ReadCycles>\n"
            "</RunParameters>";


    const run::parameters::uint_t Version = 0;
    const run::parameters::instrument_type_t ApplicationName_and_SupportMultipleSurfacesInUI=constants::NextSeq;
    const run::parameters expected_param(Version,
                                   ApplicationName_and_SupportMultipleSurfacesInUI);

    test_helper_run_parameter_parse_xml(expected_param, xml_file);
}

