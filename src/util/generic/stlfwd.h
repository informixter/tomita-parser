#pragma once

#ifdef USE_INTERNAL_STL
    #include <stlport/stlfwd>

    typedef NStl::basic_istream<char, NStl::char_traits<char> > TIStream;
    typedef NStl::basic_ostream<char, NStl::char_traits<char> > TOStream;
#else
    #include <istream>
    #include <ostream>

    typedef std::basic_istream<char, std::char_traits<char> > TIStream;
    typedef std::basic_ostream<char, std::char_traits<char> > TOStream;
#endif
