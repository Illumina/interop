// Suppress warnings and expose operators to client languages
// Most languages do no support operator overloading like C++, so these need to be renamed or ignored.

#if defined(SWIGCSHARP) || defined(SWIGJAVA)

    %rename(equals) operator==;

    %rename(subtract) operator-;
    %rename(add) *::operator+=;
    %rename(is_less) operator< const;
    %rename(copy) operator=;


#else
   // Do nothing
#endif


%ignore operator<<;
%ignore operator>>;
%ignore *::operator() const;

// TODO: Remove at in C++ and rename operator[] to at
%ignore *::operator[];
