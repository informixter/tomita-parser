#pragma once

#include "deque.h"

#ifdef USE_INTERNAL_STL
    #include <stlport/stack>
#else
    #include <stack>
    #define NStl std
#endif

template <class T, class S = ydeque<T> >
class ystack: public NStl::stack<T, S> {
    typedef NStl::stack<T, S> TBase;
public:
    inline ystack()
        : TBase()
    {
    }

    explicit ystack(const S& s)
        : TBase(s)
    {
    }
};
