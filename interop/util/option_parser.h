/** Simple command line option parser
 *
 * This parser handles options of the form `--option-name=option-value` and skips any option not prefixed with `--`. Any
 * option it does find will be removed from the list of command line arguments.
 *
 *  @file
 *  @date 7/3/16
 *  @version 1.0
 *  @copyright GNU Public License.
 */

#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "interop/util/exception.h"
#include "interop/util/unique_ptr.h"

namespace illumina { namespace interop { namespace util
{

    /** Abstract option
     */
    class abstract_option
    {
    public:
        /** Constructor
         *
         * @param flag name of the flag without `--` prefix
         * @param help message describing the function of the flag
         */
        abstract_option(const std::string &flag, const std::string &help) : m_flag(flag), m_help(help)
        { }
        /** Destructor
         */
        virtual ~abstract_option(){}

    public:
        /** Parses value from given string
         * @param str value as a string
         */
        virtual void set_value(const std::string& str) = 0;
        /** Get the current value of the option
         * @return value as a string
         */
        virtual std::string get_value()const = 0;

    public:
        /** Get name of the flag
         *
         * @return flag name
         */
        const std::string &flag() const
        {
            return m_flag;
        }
        /** Get help description of flag
         *
         * @return description
         */
        const std::string &help() const
        {
            return m_help;
        }

    private:
        std::string m_flag;
        std::string m_help;
    };
    /** Base option exception
     */
    class option_exception : public std::runtime_error
    {
    public:
        /** Constructor
         *
         * @param msg error message
         */
        option_exception(const std::string& msg) : std::runtime_error(msg){}
    };
    /** Exception thrown when an error occurs when parsing an option value
     */
    class invalid_option_value_exception : public option_exception
    {
    public:
        /** Constructor
         *
         * @param msg error message
         */
        invalid_option_value_exception(const std::string& msg) : option_exception(msg){}
    };
    /** Exception thrown when an error occurs parsing the option in total
     */
    class invalid_option_exception : public option_exception
    {
    public:
        /** Constructor
         *
         * @param msg error message
         */
        invalid_option_exception(const std::string& msg) : option_exception(msg){}
    };
    /** Proxy for references to values
     */
    template<typename T>
    class value_container
    {
    public:
        /** Type of value to parse */
        typedef T value_type;
        /** Constructor
         *
         * @param ptr pointer to value
         */
        value_container(T* ptr) : m_ptr(ptr)
        {
        }

        /** Set the value
         *
         * @param val parsed value
         */
        void set_value(const value_type& val)const
        {
            *m_ptr = val;
        }
        /** Get the value pointed to by the container
         *
         * @return value
         */
        const T& get()const
        {
            return *m_ptr;
        }

    private:
        T* m_ptr;
    };

    /** Proxy for references to values
     */
    template<typename T, typename R, typename P1>
    class value_container< R (T::* )(P1) >
    {
        typedef R (T::* function_pointer )(P1);
    public:
        /** Type of value to parse */
        typedef P1 value_type;
        /** Constructor
         *
         * @param ptr_obj pointer to object
         * @param ptr_func pointer to object method
         */
        value_container(T* ptr_obj, function_pointer ptr_func) : m_obj(ptr_obj), m_func(ptr_func){}

        /** Set the value
         *
         * @param val parsed value
         */
        void set_value(const value_type& val)const
        {
            ((*m_obj).*m_func)(val);
        }
        /** Get an empty value
         *
         * @return empty string
         */
        std::string get()const
        {
            return "";
        }

    private:
        T* m_obj;
        function_pointer m_func;
    };
    /** Wrapper memeber function setter
     *
     * @param obj reference to object
     * @param func pointer ot member function setting
     * @return value_container
     */
    template<typename T, typename R, typename P1>
    inline value_container< R (T::* )(P1) > wrap_setter(T& obj, R (T::* func)(P1))
    {
        return value_container< R (T::* )(P1) >(&obj, func);
    }



    /** Option template
     *
     * This performs the parsing of the value from the string
     */
    template<typename T>
    class option : public abstract_option
    {
    public:
        /** Constructor
         *
         * @param value pointer to value
         * @param flag option name
         * @param help descriptoin of the option
         */
        option(const value_container<T>& value, const std::string &flag, const std::string &help) : abstract_option(flag, help), m_value(
                value)
        { }
        /** Destructor
         */
        virtual ~option(){}

    public:
        /** Parse the value from a string
         *
         * @param str value as a string
         */
        void set_value(const std::string& str)
        {
            std::istringstream iss(str);
            typename value_container<T>::value_type val;
            iss >> val;
            m_value.set_value(val);
            int ch;
            if(!iss.eof() && (ch=iss.get()) != ' ')
            {
                INTEROP_THROW(invalid_option_value_exception, "Failed to parse character: " << static_cast<char>(ch));
            }
        }
        /** Get the current value of the option
         * @return value as a string
         */
        std::string get_value()const
        {
            std::ostringstream oss;
            oss << m_value.get();
            return oss.str();
        }

    private:
        value_container<T> m_value;
    };

    /** Container for options
     *
     * Performs:
     *  1. Option parsing from the command line
     *  2. Display help message
     */
    class option_parser
    {
        typedef stdbp::unique_ptr<abstract_option> option_pointer;
        typedef std::vector <option_pointer> option_vector;
    public:
        /** Add option with corresponding value to the container
         *
         * @param value reference to value to update (most be persistent!)
         * @param flag name of the option
         * @param help option description
         * @return reference to self
         */
        template<typename T>
        option_parser &operator()(T &value, const std::string &flag, const std::string &help)
        {
            m_options.push_back(option_pointer(new option<T>(&value, flag, help)));
            return *this;
        }
        /** Add option with corresponding value to the container
         *
         * @param value reference to value to update (most be persistent!)
         * @param flag name of the option
         * @param help option description
         * @return reference to self
         */
        template<typename T>
        option_parser &operator()(const value_container<T> &value, const std::string &flag, const std::string &help)
        {
            m_options.push_back(option_pointer(new option<T>(value, flag, help)));
            return *this;
        }
        /** Check if help was requested
         *
         * @param argc number of arguments
         * @param argv list of string arguments
         * @return true if help was requested
         */
        bool is_help_requested(int argc, const char **argv)
        {
            const std::string help1 = "-help";
            const std::string help2 = "--help";
            const std::string help3 = "-h";
            for(int i=1;i<argc;++i)
            {
                if(help1 == argv[i]) return true;
                if(help2 == argv[i]) return true;
                if(help3 == argv[i]) return true;
            }
            return false;
        }
        /** Parse a command line
         *
         * @param argc number of arguments
         * @param argv list of string arguments
         */
        void parse(int &argc, const char **argv)
        {
            for(int i=1;i<argc;++i)
            {
                if(!is_option(argv[i])) continue;
                if(parse_option(argv[i]))
                {
                    // Remove argument from command line
                    for (int j = i; j != argc; ++j) {
                        argv[j] = argv[j + 1];
                    }

                    // Decrements the argument count.
                    --argc;
                    --i;
                }
            }
        }
        /** Check for unknown options
         *
         * If any exist, throw an exception
         */
        void check_for_unknown_options(const int argc, const char **argv)
        {
            for(int i=1;i<argc;++i)
            {
                if(is_option(argv[i]))
                {
                    INTEROP_THROW(invalid_option_exception, "Unexpected option: " << argv[i]);
                }
            }
        }
        /** Write option description to the given output stream
         *
         * @param out output stream
         * @param prefix prefix to option description
         * @param sep separator between option name and description
         * @param postfix trailing text
         */
        void display_help(std::ostream &out, const char* prefix="\t", const char* sep=": ", const char* postfix="\n")
        {
            for(size_t i=0;i<m_options.size();++i)
                out << prefix << "--"<< m_options[i]->flag() << "[" << m_options[i]->get_value() << "]" << sep << m_options[i]->help() << postfix;
        }

    private:
        bool is_option(const char* argv)
        {
            if(argv[0] == '\0' || argv[1] == '\0' ) return false;
            if(argv[0] != '-' || argv[1] != '-') return false;
            return true;
        }
        bool parse_option(const std::string& option)
        {
            std::string::size_type n;
            if((n=option.find('=')) == std::string::npos)
                INTEROP_THROW(invalid_option_exception, "Option must be of the form --option=value");
            for(size_t i=0;i<m_options.size();++i)
            {
                if(m_options[i]->flag() == option.substr(2, n-2) )
                {
                    m_options[i]->set_value(option.substr(n+1));
                    return true;
                }
            }
            return false;
        }

    private:
        option_vector m_options;
    };


}}}

