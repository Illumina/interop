/** @page imaging_table Generate the SAV Imaging Table
 *
 *
 * This application writes out a CSV file containing the same data as the SAV Imaging tab.
 *
 * ### Running the Program
 *
 * The program runs as follows:
 *
 *      $ imaging_table 9166157_221Bin2R0I
 *
 * In this sample, 9166157_221Bin2R0I is a run folder and the summary is written to the standard output.
 *
 *      # Run Folder: 9166157_221Bin2R0I
 *      Lane,Tile,Cycle,Read,Cycle Within Read,Density(k/mm2),Density Pf(k/mm2),Cluster Count (k),Cluster Count Pf (k),% Pass Filter,% Aligned,% Phasing,% Prephasing,Error Rate,%>= Q20,%>= Q30,P90|A,P90|C,P90|G,P90|T,% No Calls,% Base|A,% Base|C,% Base|G,% Base|T,Fwhm|A,Fwhm|C,Fwhm|G,Fwhm|T,Corrected|A,Corrected|C,Corrected|G,Corrected|T,Called|A,Called|C,Called|G,Called|T,Signal To Noise,Time,Surface,Swath,Tile Number
 *      1,1101,1,1,1,2353.8,864,6470,2375,36.7,97.6,0,0.131,0.29,95.2,5.07,246,419,274,587,63.3,12.6,25,21.1,41.3,2.2,2.38,2.23,2.26,117,95,103,117,386,369,382,394,0,9.85889e+18,1,1,1
 *      2,1101,1,1,1,2353.8,1060.2,6470,2914.2,45,97.5,0.217,0.128,0.327,95.87,75.77,264,451,305,709,55,13,25.5,21.5,40,2.12,2.34,2.08,2.09,102,84,90,98,366,342,346,377,0,9.85889e+18,1,1,1
 *      3,1101,1,1,1,2353.8,855.2,6470,2350.7,36.3,98.3,0,0.063,0.363,95.07,77.02,219,373,244,580,63.7,12.5,24.4,21.2,41.9,2.02,2.31,2.04,2.05,108,82,98,103,364,338,354,362,0,9.85889e+18,1,1,1
 *      ....
 */

#include <iostream>
#include <iomanip>
#include "interop/io/metric_file_stream.h"
#include "interop/model/run_metrics.h"
#include "interop/logic/table/create_imaging_table.h"
#include "interop/io/table/imaging_table_csv.h"
#include "interop/version.h"
#include "inc/application.h"

using namespace illumina::interop::model::metrics;
using namespace illumina::interop;

int main(int argc, char** argv)
{
    if (argc == 0)
    {
        std::cerr << "No arguments specified!" << std::endl;
        return INVALID_ARGUMENTS;
    }

    std::cout << "# Version: " << INTEROP_VERSION << std::endl;

    std::vector<unsigned char> valid_to_load;

    logic::table::list_imaging_table_metrics_to_load(valid_to_load);
    for (int i = 1; i < argc; i++)
    {
        run_metrics run;
        std::cout << "# Run Folder: " << io::basename(argv[i]) << std::endl;
        int ret = read_run_metrics(argv[i], run, valid_to_load);
        if (ret != SUCCESS) return ret;
        model::table::imaging_table table;

        try
        {
            logic::table::create_imaging_table(run, table);
        }
        catch(const std::exception& ex)
        {
            std::cerr << ex.what() << std::endl;
            return UNEXPECTED_EXCEPTION;
        }
        std::cout << table << std::endl;
    }
    return SUCCESS;
}

