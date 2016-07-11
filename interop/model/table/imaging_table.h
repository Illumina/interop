/** Model for the imaging table
 *
 *  @file
 *  @date  5/12/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once
#include <math.h>
#include <iostream>
#include "interop/model/table/column_header.h"
#include "interop/model/table/imaging_table_entry.h"


namespace illumina { namespace interop { namespace model { namespace table {
    /** Describes an imaging table, row data, column headers and boolean filled */
    class imaging_table
    {
    public:
        /** Define a row vector */
        typedef std::vector< table_entry> row_vector_t;
        /** Define a flag vector */
        typedef std::vector< bool > filled_vector_t;
        /** Define a header vector */
        typedef std::vector< column_header > header_vector_t;
    public:
        /** Define a constant iterator to a row */
        typedef row_vector_t::const_iterator const_iterator;
        /** Define an iterator to a row */
        typedef row_vector_t::iterator iterator;
        /** Define a constant reference to a row */
        typedef row_vector_t::const_reference const_reference;

    public:
        /** Reserve space for the rows
         *
         * @param n number of rows
         */
        void reserve(const size_t n)
        {
            m_rows.reserve(n);
        }
        /** Get a reference to a row
         *
         * @param n index of the row
         * @return row
         */
        const_reference operator[](const size_t n)const
        {
            if(n >=m_rows.size()) throw model::index_out_of_bounds_exception("Row out of bounds");
            return m_rows[n];
        }
        /** Get a reference to a row
         *
         * @param n index of the row
         * @return row
         */
        const table_entry& at(const size_t n)const
        {
            if(n >=m_rows.size()) throw model::index_out_of_bounds_exception("Row out of bounds");
            return m_rows[n];
        }
        /** Add a row to the table
         *
         * @param val table row
         */
        void push_back(const table_entry& val)
        {
            m_rows.push_back(val);
        }
        /** Get a vector of column headers
         *
         * @return column headers
         */
        const header_vector_t& headers()const
        {
            return m_columns_header;
        }
        /** Set a vector of column headers
         *
         * @param headers column headers
         */
        void headers(const header_vector_t& headers)
        {
            m_columns_header = headers;
        }
        /** Get a vector of filled entries
         *
         * @return bool vector
         */
        const filled_vector_t& filled_columns()const
        {
            return m_columns_filled;
        }
        /** Set a vector of filled entries
         *
         * @param vec filled vector
         */
        void filled_columns(const filled_vector_t& vec)
        {
            m_columns_filled = vec;
        }
        /** Get iterator to first row
         *
         * @return iterator to first row
         */
        const_iterator begin()const
        {
            return m_rows.begin();
        }
        /** Get iterator to end of row collection
         *
         * @return iterator
         */
        const_iterator end()const
        {
            return m_rows.end();
        }
        /** Get iterator to first row
         *
         * @return iterator to first row
         */
        iterator begin()
        {
            return m_rows.begin();
        }
        /** Get iterator to end of row collection
         *
         * @return iterator
         */
        iterator end()
        {
            return m_rows.end();
        }
        /** Test if the table is empty
         *
         * @return true if empty
         */
        bool empty()const
        {
            return m_rows.empty();
        }
        /** Resize the number of rows
         *
         * @param n number of rows
         */
        void resize(const size_t n)
        {
            m_rows.resize(n);
        }
        /** Clear the contents of the table
         */
        void clear()
        {
            m_rows.clear();
            m_columns_filled.clear();
            m_columns_header.clear();
        }

    public:
        /** Number of columns in the table
         *
         * @return column count
         */
        size_t column_count()const
        {
            return m_columns_header.size();
        }
        /** Number of rows in the table
         *
         * @return row count
         */
        size_t row_count()const
        {
            return m_rows.size();
        }

        friend std::istream& operator>>(std::istream& in, imaging_table& table);
        friend std::ostream& operator<<(std::ostream& out, const imaging_table& table);
    private:
        row_vector_t m_rows;
        filled_vector_t m_columns_filled;
        header_vector_t m_columns_header;
    };

}}}}
