/** Model for the imaging table
 *
 *  @file
 *  @date  6/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <string>
#include <iostream>

namespace illumina { namespace interop { namespace model { namespace table {


    /** Column header for the imaging table
     */
    struct column_header
    {
        /** Constructor */
        column_header(){}
        /** Constructor
         *
         * @param title tile of the column
         */
        column_header(const std::string& title) : m_subtitle(title) {}
        /** Constructor
         *
         * @param title tile of the column
         * @param subtitle subtile of the column
         */
        column_header(const std::string& title, const std::string& subtitle) : m_title(title), m_subtitle(subtitle){}

    public:
        /** Get the group title
         *
         * @return group title
         */
        const std::string& title()const
        {
            return m_title;
        }
        /** Get the subtitle
         *
         * @return subtitle
         */
        const std::string& subtitle()const
        {
            return m_subtitle;
        }
        /** Get the identifer title
         *
         * @return identifier title
         */
        const std::string& id()const
        {
            return m_title != "" ? m_title : m_subtitle;
        }
        /** Compare the column header to another
         *
         * @param other other column header
         * @return true if both titles are the same
         */
        bool operator==(const column_header& other)const
        {
            return m_title == other.m_title && m_subtitle == other.m_subtitle;
        }
        /** Read a column header from an input stream
         *
         * @param in input stream
         * @param header column header
         * @return input stream
         */
        friend std::istream& operator>>(std::istream& in, column_header& header)
        {
            std::getline(in, header.m_subtitle, ',');
            std::string::size_type n;
            if((n=header.m_subtitle.find("|")) != std::string::npos)
            {
                header.m_title = header.m_subtitle.substr(0, n);
                header.m_subtitle = header.m_subtitle.substr(n+1);
            }
            return in;
        }

        /** Write a column header to an output stream
         *
         * @param out output stream
         * @param header column header
         * @return output stream
         */
        friend std::ostream& operator<<(std::ostream& out, const column_header& header)
        {
            if(header.m_title != "") out << header.m_title << "|";
            out << header.m_subtitle;
            return out;
        }

    private:
        /** Title of a group of columns */
        std::string m_title;
        /** Title of the column */
        std::string  m_subtitle;
    };

}}}}
