/** Iterator over a range of numbers
 *
 * This header provides facilities to manipulate files, directories and the paths that identify them.
 *
 *  @file
 *  @date 10/27/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */
#pragma once

namespace illumina { namespace interop { namespace util
{
    /** Abstract range iterator */
    class abstract_range_iterator
    {
    public:
        /** Destructor */
        virtual ~abstract_range_iterator(){}
        /** Increment the iterator to the next position in the range
         *
         * @return reference to iterator
         */
        abstract_range_iterator& operator++()
        {
            advance();
            return *this;
        }
        /** Reset the iterator to the start of the range
         */
        void operator()()
        {
            reset();
        }
        /** Test if you can continue to iterate
         *
         * @return true if not done
         */
        operator bool() const
        {
            return !is_done();
        }

    public:
        /** Reset the iterator to the start of the range
         */
        virtual void reset()=0;
        /** Advance the iterator to the next element in the range
         */
        virtual void advance()=0;
        /** Test if you are at the end of the range
         *
         * @return true if you are done iterating, reached the end of the range
         */
        virtual bool is_done()const=0;
    };

    /** Chain iterators into a recursive loop
     *
     * @note this class cannot be copied, data is moved!
     */
    class chain_range_iterator : public abstract_range_iterator
    {
    public:
        /** Constructor
         *
         * @param it1 iterator to first range
         * @param it2 iterator to second range
         */
        chain_range_iterator(abstract_range_iterator* it1=0, abstract_range_iterator* it2=0) :
                m_iterator_1(it1), m_iterator_2(it2)
        {
        }
        /** Constructor
         *
         * @param iterators array of iterators
         * @param current current index to iterator
         * @param dummy not used
         * @return
         */
        template<int N>
        chain_range_iterator(abstract_range_iterator* (&iterators)[N], size_t current=0, size_t dummy=0)
        {
            (void)dummy;
            m_iterator_1 = iterators[current];
            if( (current+2) < N )
                m_iterator_2 = new chain_range_iterator(iterators, current+1, 0);
            else m_iterator_2 = iterators[current+1];
        }
        /** Destructor */
        virtual ~chain_range_iterator()
        {
            delete m_iterator_1;
            delete m_iterator_2;
        }

    public:
        /** Move constructor
         *
         * @note No copy is performed! Data is moved!
         * @param other chain iterator to move data from
         */
        chain_range_iterator(const chain_range_iterator& other) : m_iterator_1(other.m_iterator_1), m_iterator_2(other.m_iterator_2)
        {
            other.m_iterator_1 = 0;
            other.m_iterator_2 = 0;
        }
        /** Move assignment operator
         *
         * @note No copy is performed! Data is moved!
         * @param other chain iterator to move data from
         * @return reference to this object
         */
        chain_range_iterator& operator=(const chain_range_iterator& other)
        {
            delete m_iterator_1;
            delete m_iterator_2;
            m_iterator_1 = other.m_iterator_1;
            m_iterator_2 = other.m_iterator_2;
            other.m_iterator_1 = 0;
            other.m_iterator_2 = 0;
            return *this;
        }

    public:
        /** Reset the iterator to the start of the range
         */
        void reset()
        {
            INTEROP_ASSERT(m_iterator_1 != 0);
            INTEROP_ASSERT(m_iterator_2 != 0);
            if(m_iterator_2 == 0 || m_iterator_1 == 0) return;
            m_iterator_1->reset();
            m_iterator_2->reset();
        }
        /** Advance the iterator to the next element in the range
         */
        void advance()
        {
            INTEROP_ASSERT(m_iterator_1 != 0);
            INTEROP_ASSERT(m_iterator_2 != 0);
            if(m_iterator_2 == 0 || m_iterator_1 == 0) return;
            m_iterator_1->advance();
            if(m_iterator_1->is_done())
            {

                m_iterator_2->advance();
                if(!m_iterator_2->is_done()) m_iterator_1->reset();
            }
        }
        /** Test if you are at the end of the range
         *
         * @return true if you are done iterating, reached the end of the range
         */
        bool is_done()const
        {
            INTEROP_ASSERT(m_iterator_2 != 0);
            if(m_iterator_2 == 0 || m_iterator_1 == 0) return true;
            return m_iterator_1->is_done() && m_iterator_2->is_done();
        }

    private:
        mutable abstract_range_iterator* m_iterator_1;
        mutable abstract_range_iterator* m_iterator_2;
    };
    /** Iterate over a range and update a reference to another value
     */
    template<typename T, typename Storage=T>
    class indirect_range_iterator : public abstract_range_iterator
    {
    public:
        /** Constructor
         *
         * @param rval reference to value to update
         */
        indirect_range_iterator(T &rval) : m_value(&rval),
                                           m_begin(static_cast<Storage>(rval)),
                                           m_end(static_cast<Storage>(rval))
        {
            reset();
        }
        /** Constructor
         *
         * @param rval reference to value to update
         * @param beg value at start of range
         * @param end value at end of range
         * @param reset_state reset current state
         */
        indirect_range_iterator(T &rval, const T beg, const T end, const bool reset_state=true) : m_value(&rval),
                                                                     m_begin(static_cast<Storage>(beg)),
                                                                     m_end(static_cast<Storage>(end))
        {
            if(reset_state) reset();
        }
        /** Destructor */
        virtual ~indirect_range_iterator(){}

    public:
        /** Reset the iterator to the start of the range
         */
        void reset()
        {
            (*m_value) = static_cast<T>(m_begin);
        }
        /** Advance the iterator to the next element in the range
         */
        void advance()
        {
            const Storage sval = static_cast<Storage>(*m_value)+1;
            (*m_value) = static_cast<T>(sval);
        }

        /** Test if you are at the end of the range
         *
         * @return true if you are done iterating, reached the end of the range
         */
        bool is_done() const
        {
            return (*m_value) >= static_cast<T>(m_end);
        }

    private:
        T* m_value;
        Storage m_begin;
        Storage m_end;
    };

}}}
