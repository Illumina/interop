// Suppress warnings and expose operators to client languages
// Most languages do no support operator overloading like C++, so these need to be renamed or ignored.

%ignore *::operator[](size_t const)const;
%ignore *::operator()const;
%ignore *::operator()(size_t const,size_t const)const;
%ignore *::operator()(size_t const,size_t const,size_t const)const;
%ignore *::operator()(size_t const,size_t const,size_t const,size_t const)const;
%ignore *::at; // .at is deprecated in the C++ code
%ignore operator enum_t;
%ignore operator<<;
%ignore operator>>;
%ignore *::iterator;
%ignore *::begin;
%ignore *::end;



#if defined(SWIGCSHARP) || defined(SWIGJAVA) || defined(SWIGPYTHON)

// TODO: Add python section and rename to appropriate python operator
    %rename(equals) operator==;
    %rename(subtract) operator-;
    %rename(add) *::operator+=;
    %rename(is_less) operator< const;
    %rename(copy) operator=;
    %rename(to_int) operator uint64_t;
    %rename(at) *::operator[];
    %rename(at) *::operator();
//#elif defined(SWIGPYTHON)
 /*%rename(__add__)              *::operator+;
%rename(__pos__)              *::operator+();
%rename(__pos__)              *::operator+() const;
%rename(__sub__)              *::operator-;
%rename(__neg__)              *::operator-() const;
%rename(__neg__)              *::operator-();
%rename(__mul__)              *::operator*;
%rename(__deref__)            *::operator*();
%rename(__deref__)            *::operator*() const;
%rename(__div__)              *::operator/;
%rename(__mod__)              *::operator%;
%rename(__logxor__)           *::operator^;
%rename(__logand__)           *::operator&;
%rename(__logior__)           *::operator|;
%rename(__lognot__)           *::operator~();
%rename(__lognot__)           *::operator~() const;
%rename(__not__)              *::operator!();
%rename(__not__)              *::operator!() const;
%rename(__assign__)           *::operator=;
%rename(__add_assign__)       *::operator+=;
%rename(__sub_assign__)       *::operator-=;
%rename(__mul_assign__)       *::operator*=;
%rename(__div_assign__)       *::operator/=;
%rename(__mod_assign__)       *::operator%=;
%rename(__logxor_assign__)    *::operator^=;
%rename(__logand_assign__)    *::operator&=;
%rename(__logior_assign__)    *::operator|=;
%rename(__lshift__)           *::operator<<;
%rename(__lshift_assign__)    *::operator<<=;
%rename(__rshift__)           *::operator>>;
%rename(__rshift_assign__)    *::operator>>=;
%rename(__eq__)               *::operator==;
%rename(__ne__)               *::operator!=;
%rename(__lt__)               *::operator<;
%rename(__gt__)               *::operator>;
%rename(__lte__)              *::operator<=;
%rename(__gte__)              *::operator>=;
%rename(__and__)              *::operator&&;
%rename(__or__)               *::operator||;
%rename(__preincr__)          *::operator++();
%rename(__postincr__)         *::operator++(int);
%rename(__predecr__)          *::operator--();
%rename(__postdecr__)         *::operator--(int);
%rename(__comma__)            *::operator,();
%rename(__comma__)            *::operator,() const;
%rename(__member_ref__)       *::operator->;
%rename(__member_func_ref__)  *::operator->*;
%rename(__funcall__)          *::operator();
%rename(__aref__)             *::operator[];*/
#else
   // Do nothing
#endif

%define WRAP_VECTOR(vector_t)
%apply size_t { vector_t::size_type };
%enddef

%define RENAME_TEMPLATE_OPERATOR_CONST(vector_t) // Prefer constant over the default, non-const
%ignore vector_t::at;
%ignore vector_t::operator[](size_t const);
%ignore vector_t::operator();
%ignore vector_t::operator()(size_t const,size_t const);
%ignore vector_t::operator()(size_t const,size_t const,size_t const,size_t const);
%rename(at) vector_t::operator[]const;
%rename(at) vector_t::operator()const;
%enddef


