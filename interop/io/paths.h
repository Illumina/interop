/** Interface for common file paths
 *
 *
 *  @file
 *  @date 10/25/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include "interop/util/filesystem.h"

namespace illumina { namespace interop { namespace io
{

    /** Location of files written to an RTA run folder
     */
    class paths
    {
    public:
        /** Get the path to the RTA configuration file
         *
         * @param run_directory run directory
         * @param version version of RTA
         * @return config filename
         */
        static std::string rta_config(const std::string &run_directory, const int version=3)
        {
            return io::combine(run_directory, rta_config(version));
        }
        /** Get the path of the RunInfo
         *
         * @return path/to/RunInfo.xml
         */
        static std::string run_info(const std::string &run_directory)
        {
            return io::combine(run_directory, run_info());
        }
        /** Get the path of the RunParameters
         *
         * @param run_directory run directory
         * @param alternate return alternate filename
         * @return path/to/RunParameters.xml
         */
        static std::string run_parameters(const std::string &run_directory, const bool alternate=false)
        {
            return io::combine(run_directory, run_parameters(alternate));
        }
        /** Get the name of the RunInfo
         *
         * @return RunInfo.xml
         */
        static std::string run_info()
        {
            return "RunInfo.xml";
        }
        /** Get the name of the RunParameters
         *
         * @param alternate return alternate filename
         * @return RunParameters.xml (alternate: runParameters.xml)
         */
        static std::string run_parameters(const bool alternate=false)
        {
            if(alternate) return "runParameters.xml";
            return "RunParameters.xml";
        }
        /** Generate a file name from a run directory and the metric type
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param run_directory file path to the run directory
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        template<class MetricType>
        static std::string interop_filename(const std::string &run_directory, bool use_out = true)
        {
            return interop_filename(run_directory, MetricType::prefix(), MetricType::suffix(), use_out);
        }

        /** Generate a file name from a run directory and the metric type
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        template<class MetricType>
        static std::string interop_basename(bool use_out = true)
        {
            return interop_basename(MetricType::prefix(), MetricType::suffix(), use_out);
        }
        /** Get the path to the RTA configuration file
         *
         * @param version version of RTA
         * @return config filename
         */
        static std::string rta_config(const int version=3)
        {
            if(version == 3) return "RTA3.cfg";
            return "RTAConfiguration.xml";
        }

    private:
        /** Generate a file name from a run directory and the InterOp name
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param prefix prefix name of the interop file
         * @param suffix suffix name of the interop file
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        static std::string interop_basename(const std::string &prefix,
                                            const std::string &suffix,
                                            const bool use_out = true)
        {
            return prefix + "Metrics" + suffix + ((use_out) ? ("Out.bin") : (".bin"));
        }

        /** Generate a file name from a run directory and the InterOp name
         *
         * @note The 'Out' suffix is appended when we read the file. We excluded the Out in certain conditions
         * when writing the file.
         *
         * @param run_directory file path to the run directory
         * @param prefix prefix name of the interop file
         * @param suffix suffix name of the interop file
         * @param use_out if true, append "Out" to the end of the filename
         * @return file path to the InterOp directory
         */
        static std::string interop_filename(const std::string &run_directory,
                                            const std::string &prefix,
                                            const std::string &suffix,
                                            const bool use_out = true)
        {
            if (io::basename(run_directory) == interop_basename(prefix, suffix, use_out))
                return run_directory;
            if (io::basename(run_directory) == "InterOp")
                return io::combine(run_directory, interop_basename(prefix, suffix, use_out));
            return io::combine(interop_directory_name(run_directory), interop_basename(prefix, suffix, use_out));
        }
        /** Append the InterOp directory to the run directory file path
         *
         * @param run_directory file path to the run directory
         * @return file path to the InterOp directory
         */
        static std::string interop_directory_name(const std::string &run_directory)
        {
            return io::combine(run_directory, "InterOp");
        }
        /** Generate the proper name of a cycle folder from a given cycle
         *
         * @param cycle cycle number
         * @return cycle folder name
         */
        static std::string cycle_folder(const size_t cycle)
        {
            return "C" + util::lexical_cast<std::string>(cycle)+".1";
        }
    };

}}}


