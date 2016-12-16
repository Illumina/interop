/** Write plot data in the GNUPlot format
 *
 *  @file
 *  @date 5/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <cctype>
#include "interop/model/plot/flowcell_data.h"
#include "interop/model/plot/bar_point.h"
#include "interop/model/plot/candle_stick_point.h"
#include "interop/model/plot/plot_data.h"
#include "interop/io/table/csv_format.h"

namespace illumina { namespace interop { namespace io { namespace plot
{

    /** Write a plot in the GNUPlot scripting language
     */
    class gnuplot_writer
    {
    public:
        /** Write the flowcell heat map of a specific metric to the output stream using the GNUPlot format
         *
         * @param out output stream
         * @param data flowcell heatmap data
         * @param output_image_path optional output image path for the script
         */
        void write_flowcell(std::ostream &out,
                            const model::plot::flowcell_data &data,
                            const std::string &output_image_path = "")
        {
            write_output_format(out, output_image_path);
            write_title(out, data);
            out << "set view map\n";
            out << "unset key\n";
            out << "unset tics\n";
            out << "unset border\n";
            out << "set cbtics border in scale 0,0 mirror norotate autojustify\n";
            out << "set cbrange [" << data.saxis().min() << ":" << data.saxis().max() << "]\n";
            out <<
            "set palette defined (0 \"#8A2BE2\", 0.143 \"blue\", 0.286 \"cyan\", 0.429 \"light-green\", 0.572 \"#32CD32\", 0.715 \"#00FF00\", 0.858 \"yellow\", 1 \"orange\")\n";
            out << "set autoscale fix\n";
            out << "set size ratio 2\n";
            out << "set yrange[:] reverse\n";
            out << "plot \"-\" matrix with image" << "\n";
            const size_t swath_count = data.column_count() / data.tile_count();
            for (size_t y = 0; y < data.tile_count(); ++y) // Rows
            {
                for (size_t x = 0; x < data.row_count(); ++x) // Column groups
                {
                    for (size_t s = 0; s < swath_count; ++s) // Columns
                    {
                        out << " " << table::handle_nan(data(x, y + s * data.tile_count()));
                    }
                    out << " nan";
                }
                out << "\n";
            }
        }

        /** Write the flowcell heat map to the output stream using the GNUPlot format
         *
         * @param out output stream
         * @param data flowcell heatmap data
         * @param output_image_path optional output image path for the script
         */
        void write_flowcell_tile_id(std::ostream &out,
                                    const model::plot::flowcell_data &data,
                                    const std::string &output_image_path = "")
        {
            write_output_format(out, output_image_path);
            write_title(out, data);
            out << "set view map\n";
            out << "unset key\n";
            out << "set cbrange [" << data.saxis().min() << ":" << data.saxis().max() << "]\n";
            out << "plot \"-\" matrix with image" << "\n";
            const size_t swath_count = data.column_count() / data.tile_count();
            for (size_t y = 0; y < data.tile_count(); ++y)
            {
                for (size_t x = 0; x < data.row_count(); ++x)
                {
                    for (size_t s = 0; s < swath_count; ++s)
                    {
                        out << " " << data.tile_id(x, y + s * data.tile_count());
                    }
                }
                out << "\n";
            }
        }

        /** Write a heat map to the output stream
         *
         * @param out output stream
         * @param data heat map data
         * @param output_image_path optional output image path for the script
         */
        void write_heatmap(std::ostream &out,
                           const model::plot::heatmap_data &data,
                           const std::string &output_image_path = "")
        {
            write_output_format(out, output_image_path);
            write_title(out, data);
            write_axes(out, data.xyaxes());
            out << "set view map\n";
            out << "set palette defined (0 \"white\", 0.333 \"green\", 0.667 \"yellow\", 1 \"red\")\n";
            out << "plot \"-\" matrix with image" << "\n";
            for (size_t y = 0; y < data.column_count(); ++y)
            {
                out << data(0, y);
                for (size_t x = 1; x < data.row_count(); ++x)
                {
                    out << " " << table::handle_nan(data(x, y));
                }
                out << "\n";
            }
        }

        /** Write a generic chart (line, candlestick or bar) to the output stream
         *
         * @param out output stream
         * @param data plot data for a line, candlestick or bar plot
         * @param output_image_path optional output image path for the script
         */
        template<typename Point>
        void write_chart(std::ostream &out,
                         const model::plot::plot_data<Point> &data,
                         const std::string &output_image_path = "")
        {
            write_output_format(out, output_image_path);
            write_title(out, data);
            write_axes(out, data.xyaxes());
            write_plot_description(out, data);
            write_plot_data(out, data);
        }

    protected:
        /** Write each series contained within plot_data to the output stram
         *
         * @param out output stream
         * @param data plot data for a line, candlestick or bar plot
         */
        template<typename Point>
        void write_plot_data(std::ostream &out, const model::plot::plot_data<Point> &data)
        {
            if (data.size() == 0) return;
            for (size_t series = 0; series < data.size(); ++series)
            {
                write_plot_series(out, data[series]);
                write_end_of_series(out); // Marker for the end of the series so GNUPlot knows when the next line starts
            }
        }

        /** Write a description of the plot data to the output stream
         *
         * @param out output stream
         * @param data plot data for a line, candlestick or bar plot
         */
        template<typename Point>
        void write_plot_description(std::ostream &out, const model::plot::plot_data<Point> &data)
        {
            if (data.size() == 0) return;

            switch (data[0].series_type())
            {
                case model::plot::series<Point>::Bar:
                    out << "set style fill solid border -1\n";
                    out << "unset key\n";
                    break;
                case model::plot::series<Point>::Candlestick:
                    out << "set boxwidth 0.3\n";
                    break;
                default:
                    break;
            };

            out << "plot ";
            for (size_t series = 0; series < data.size(); ++series)
            {
                out << "\"-\" ";
                write_type(out, data[series]);
                write_label(out, data[series]);
                write_color(out, data[series]);
                write_additional(out, data[series]);
                if ((series + 1) < data.size()) out << ",";
            }
            out << "\n";
        }

        /** Write the label and range for each axis to the output stream
         *
         * @param out output stream
         * @param axes pair of axis objects
         */
        void write_axes(std::ostream &out, const model::plot::axes &axes)
        {
            write_axis(out, axes.y(), 'y');
            write_axis(out, axes.x(), 'x');
        }

        /** Write the label and range for an axis to the output stream
         *
         * @param out output stream
         * @param axis range and label of the axis
         * @param axis_label 'x' or 'y' to denote either x- or y-axis
         */
        void write_axis(std::ostream &out, const model::plot::axis &axis, const char axis_label)
        {
            out << "set " << axis_label << "range ["
                << table::handle_nan(axis.min()) << " : " << table::handle_nan(axis.max()) << " ]\n";
            if (axis.label() != "") out << "set " << axis_label << "label \"" << axis.label() << "\"\n";
        }

        /** Write the title of the plot to the output stream
         *
         * @param out output stream
         * @param data plot data for a line, candlestick, bar plot, heat map or flowcell heat map
         */
        template<typename PlotData>
        void write_title(std::ostream &out, const PlotData &data)
        {
            if (data.title() != "") out << "set title \"" << data.title() << "\"\n";
        }

        /** Write the output format for GNUPlot
         *
         * This tells GNUPlot to write the plot as an image in PNG format
         *
         * @param out output stream
         * @param output_image_path output filename of the image
         */
        void write_output_format(std::ostream &out, const std::string &output_image_path)
        {
            if (output_image_path == "") return;
            out << "set terminal png crop" << std::endl;
            out << "set output \'" << output_image_path << "\'" << std::endl;
            //out << "set style fill solid noborder" << std::endl;
        }

    protected:
        /** Write a series of candlestick points to the output stream
         *
         * @param out output stream
         * @param series series of candlestick points
         */
        void write_plot_series(std::ostream &out, const model::plot::series<model::plot::candle_stick_point> &series)
        {
            if (series.series_type() == model::plot::series<model::plot::candle_stick_point>::Candlestick)
            {
                for (size_t i = 0; i < series.size(); ++i)
                {
                    out << table::handle_nan(series[i].x()) << "\t";
                    out << table::handle_nan(series[i].lower()) << "\t";
                    out << table::handle_nan(series[i].p25()) << "\t";
                    out << table::handle_nan(series[i].p50()) << "\t";
                    out << table::handle_nan(series[i].p75()) << "\t";
                    out << table::handle_nan(series[i].upper());
                    out << std::endl;
                }
            }
            else write_plot_line(out, series);
        }

        /** Write a series of line plot points to the output stream
         *
         * @param out output stream
         * @param series series of line plot points
         */
        template<typename P>
        void write_plot_line(std::ostream &out, const model::plot::series<P> &series)
        {
            for (size_t i = 0; i < series.size(); ++i)
            {
                out << table::handle_nan(series[i].x()) << "\t" << table::handle_nan(series[i].y()) << std::endl;
            }
        }

        /** Write a series of line plot points to the output stream
         *
         * @param out output stream
         * @param series series of line plot points
         */
        template<typename X, typename Y>
        void write_plot_series(std::ostream &out, const model::plot::series<model::plot::data_point<X, Y> > &series)
        {
            write_plot_line(out, series);
        }

        /** Write a series of bar plot points to the output stream
         *
         * @param out output stream
         * @param series series of bar plot points
         */
        void write_plot_series(std::ostream &out, const model::plot::series<model::plot::bar_point> &series)
        {
            for (size_t i = 0; i < series.size(); ++i)
            {
                out << table::handle_nan(series[i].x()) << "\t"
                    << table::handle_nan(series[i].y()) << "\t"
                    << table::handle_nan(series[i].width()) << std::endl;
            }
        }

    protected:
        /** Write the end of series indicator to the output stream
         *
         * @param out output stream
         */
        void write_end_of_series(std::ostream &out)
        {
            out << "e\n";
        }

        /** Write the plot type to the output stream
         *
         * This function determines the plot type from the series_type() function
         *
         * @param out output stream
         * @param series series data for a line, candlestick or bar plot
         * @param sep separator between features
         */
        template<typename P>
        void write_type(std::ostream &out,
                        const model::plot::series<P> &series,
                        const char sep = ' ')
        {
            switch (series.series_type())
            {
                case model::plot::series<P>::Bar:
                    out << "using 1:2:3 with boxes" << sep;
                    break;
                case model::plot::series<P>::Candlestick:
                    out << "using 1:3:2:6:5 with candlesticks" << sep;
                    break;
                case model::plot::series<P>::Line:
                    out << "with lines" << sep;
                    break;
            }
        }

        /** Write additional features of the plot based on the plot type
         *
         * This function determines the plot type from the series_type() function
         *
         * @param out output stream
         * @param series series data for a line, candlestick or bar plot
         * @param sep seperator between features
         */
        template<typename P>
        void write_additional(std::ostream &out, const model::plot::series<P> &series, const char sep = ' ')
        {
            switch (series.series_type())
            {
                case model::plot::series<P>::Candlestick:
                    out << "whiskerbars" << sep;//, \'\' using 1:4:4:4:4 with candlesticks" << sep;
                    break;
                default:
                    break;
            }
        }

        /** Write the label for the series
         *
         * If there is no label, notitle is written instead.
         *
         * @param out output stream
         * @param series series data for a line, candlestick or bar plot
         * @param sep seperator between features
         */
        template<typename P>
        void write_label(std::ostream &out, const model::plot::series<P> &series, const char sep = ' ')
        {
            if (series.title() == "") out << "notitle";
            else out << "title \"" << series.title() << "\"";
            out << sep;
        }

        /** Write the color of the series
         *
         *
         * @param out output stream
         * @param series series data for a line, candlestick or bar plot
         * @param sep separator between features
         */
        template<typename P>
        void write_color(std::ostream &out, const model::plot::series<P> &series, const char sep = ' ')
        {
            if (series.color() == "") return;
            out << "lt rgb \"" << normalize_color(series.color()) << "\"";
            out << sep;
        }

        /** Normalize the camel case color to lower case and seperated with a dash (`-`)
         *
         * @param color camel case color name
         * @return GNUPlot color name
         */
        std::string normalize_color(const std::string &color)
        {
            std::string color_normalized;
            for (size_t i = 0; i < color.length(); ++i)
            {
                if (std::isupper(color[i]))
                {
                    if (i > 0) color_normalized += '-';
                    color_normalized += static_cast<char>(::tolower(color[i]));
                }
                else color_normalized += color[i];
            }
            return color_normalized;
        }
    };


}}}}

