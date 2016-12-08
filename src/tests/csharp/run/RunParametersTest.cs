using System;
using NUnit.Framework;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using Illumina.InterOp.Run;

namespace illumina.interop.csharp.unittest
{
	/// <summary>
	/// Confirm that the run info XML parsing works properly in C#
	/// </summary>
	///
	[TestFixture]
	public abstract class AbstractRunParametersTest
	{
		/// <summary>
		/// The expected run parameters
		/// </summary>
		protected parameters expected_param;
		/// <summary>
		/// The actual run parameters
		/// </summary>
		protected parameters actual_param = new parameters();
		/// <summary>
		/// The setup should be overridden by the specific configuration file
		/// </summary>
	    [SetUp]
		protected abstract void SetUp();

		/// <summary>
		/// Confirms that reading the hard coded binary data matches the expected values of the metric
		/// </summary>
	    [Test]
	    public void CompareRunInfo()
	    {
            Assert.AreNotEqual(actual_param.instrument_type(), instrument_type.UnknownInstrument);
            Assert.AreEqual(actual_param.instrument_type(), expected_param.instrument_type());

            Assert.AreEqual(actual_param.version(), expected_param.version());
        }
	}
	/// <summary>
	/// Confirm that the run info XML parsing for MiSeq works properly in C#
	/// </summary>
	///
	[TestFixture]
	public class RunParametersMiSeqTest : AbstractRunParametersTest
	{
		/// <summary>
		/// The setup should be overridden by the specific version of the format
		/// </summary>
		[SetUp]
		protected override void SetUp()
		{
            string xml_file = "<?xml version=\"1.0\"?>\n"+
                "<RunParameters xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">\n"+
                "  <EnableCloud>false</EnableCloud>\n"+
                "  <Barcode>000000000-A12V4</Barcode>\n"+
                "  <CopyManifests>true</CopyManifests>\n"+
                "  <FlowcellRFIDTag>\n"+
                "    <SerialNumber>000000000-A12V4</SerialNumber>\n"+
                "    <PartNumber>15028382</PartNumber>\n"+
                "    <ExpirationDate>2012-10-09T00:00:00</ExpirationDate>\n"+
                "  </FlowcellRFIDTag>\n"+
                "  <PR2BottleRFIDTag>\n"+
                "    <SerialNumber>MS2106444-00PR2</SerialNumber>\n"+
                "    <PartNumber>15023445</PartNumber>\n"+
                "    <ExpirationDate>2013-06-18T00:00:00</ExpirationDate>\n"+
                "  </PR2BottleRFIDTag>\n"+
                "  <ReagentKitRFIDTag>\n"+
                "    <ExpirationDate>0001-01-01T00:00:00</ExpirationDate>\n"+
                "  </ReagentKitRFIDTag>\n"+
                "  <Resumable>true</Resumable>\n"+
                "  <ManifestFiles />\n"+
                "  <AfterRunWashMethod>Post-Run Wash</AfterRunWashMethod>\n"+
                "  <Setup>\n"+
                "    <SupportMultipleSurfacesInUI>false</SupportMultipleSurfacesInUI>\n"+
                "    <ApplicationVersion>90.0.1.44</ApplicationVersion>\n"+
                "    <NumTilesPerSwath>14</NumTilesPerSwath>\n"+
                "    <NumSwaths>1</NumSwaths>\n"+
                "    <NumLanes>1</NumLanes>\n"+
                "    <Read1>0</Read1>\n"+
                "    <Index1Read>0</Index1Read>\n"+
                "    <Index2Read>0</Index2Read>\n"+
                "    <Read2>0</Read2>\n"+
                "    <ApplicationName>MiSeq Control Software</ApplicationName>\n"+
                "  </Setup>\n"+
                "  <RunID>120705_M00903_0009_A000000000-A12V4</RunID>\n"+
                "  <ScannerID>M00903</ScannerID>\n"+
                "  <RunNumber>8</RunNumber>\n"+
                "  <FPGAVersion>9.5.10</FPGAVersion>\n"+
                "  <CPLD>Not Specified</CPLD>\n"+
                "  <CameraModel>Not Specified</CameraModel>\n"+
                "  <CameraDriverVersion>Not Specified</CameraDriverVersion>\n"+
                "  <CameraFirmwareVersion>Not Specified</CameraFirmwareVersion>\n"+
                "  <MCSVersion>90.0.1.44</MCSVersion>\n"+
                "  <RTAVersion>1.16.18</RTAVersion>\n"+
                "  <PR2BottleBarcode>MS2106444-00PR2</PR2BottleBarcode>\n"+
                "  <ReagentKitPartNumberEntered />\n"+
                "  <ReagentKitVersion>Version2</ReagentKitVersion>\n"+
                "  <ReagentKitBarcode>AA1234567-12345</ReagentKitBarcode>\n"+
                "  <PreviousPR2BottleBarcode />\n"+
                "  <PreviousReagentKitBarcode />\n"+
                "  <ExperimentName />\n"+
                "  <Chemistry>Default</Chemistry>\n"+
                "  <Username>XXXXX</Username>\n"+
                "  <SelectedTiles>\n"+
                "    <int>0</int>\n"+
                "  </SelectedTiles>\n"+
                "  <Workflow>\n"+
                "    <Analysis>Metagenomics</Analysis>\n"+
                "  </Workflow>\n"+
                "  <EnableAnalysis>false</EnableAnalysis>\n"+
                "  <Reads>\n"+
                "    <RunInfoRead Number=\"1\" NumCycles=\"251\" IsIndexedRead=\"N\" />\n"+
                "    <RunInfoRead Number=\"2\" NumCycles=\"12\" IsIndexedRead=\"Y\" />\n"+
                "    <RunInfoRead Number=\"3\" NumCycles=\"251\" IsIndexedRead=\"N\" />\n"+
                "  </Reads>\n"+
                "  <TempFolder>D:\\Illumina\\MiSeqTemp\\120705_M00903_0009_A000000000-A12V4</TempFolder>\n"+
                "  <AnalysisFolder>D:\\Illumina\\MiSeqAnalysis\\120705_M00903_0009_A000000000-A12V4</AnalysisFolder>\n"+
                "  <RunStartDate>120705</RunStartDate>\n"+
                "  <MostRecentWashType>PostRun</MostRecentWashType>\n"+
                "  <RecipeFolder>D:\\Illumina\\MiSeq Control Software\\CustomRecipe</RecipeFolder>\n"+
                "  <ILMNOnlyRecipeFolder>C:\\Illumina\\MiSeq Control Software\\Recipe</ILMNOnlyRecipeFolder>\n"+
                "  <SampleSheetName>SampleSheet_Miseq</SampleSheetName>\n"+
                "  <SampleSheetFolder>C:\\Illumina\\MiSeq Control Software\\SampleSheets</SampleSheetFolder>\n"+
                "  <ManifestFolder>D:\\Illumina\\MiSeq Control Software\\Manifests</ManifestFolder>\n"+
                "  <OutputFolder>Z:\\120705_M00903_0009_A000000000-A12V4</OutputFolder>\n"+
                "  <FlowcellName>A</FlowcellName>\n"+
                "  <FocusMethod>AutoFocus</FocusMethod>\n"+
                "  <SurfaceToScan>Both</SurfaceToScan>\n"+
                "  <SaveFocusImages>true</SaveFocusImages>\n"+
                "  <SaveScanImages>true</SaveScanImages>\n"+
                "  <CloudUsername />\n"+
                "  <CloudRunId>398407</CloudRunId>\n"+
                "  <SendInstrumentHealthToILMN>true</SendInstrumentHealthToILMN>\n"+
                "  <AmpliconWorkflowType />\n"+
                "</RunParameters>";


            uint Version = 0;
            instrument_type ApplicationName_and_SupportMultipleSurfacesInUI=instrument_type.MiSeq;
            expected_param = new parameters(Version,
                                           ApplicationName_and_SupportMultipleSurfacesInUI);
            actual_param.parse(xml_file);

		}
	}
}
