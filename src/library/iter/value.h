#pragma once

#include "vector.h"

#include <util/generic/typetraits.h>

namespace NIter {

// same as TVectorIterator, but can also be initialized on non-ptr single value
// (it makes a copy of this value and refers to it with IterCur.
// TODO: think of more appropriate class name!
template <class T>
class TValueIterator: public TVectorIterator<T>
{
    typedef TVectorIterator<T> TBase;
public:

    inline TValueIterator()
        : TBase()
    {
    }

    inline TValueIterator(typename TBase::TStlIter it_begin, typename TBase::TStlIter it_end)
        : TBase(it_begin, it_end)
    {
    }

#ifdef USE_INTERNAL_STL
    inline explicit TValueIterator(const T& value)
        : Value(value)
    {
        Reset(&Value, &Value + 1);
    }
#else
    inline explicit TValueIterator(const T& value)
        : ValueVector(1, value)
    {
        Reset(ValueVector);
    }

#endif

#ifdef USE_INTERNAL_STL
    inline TValueIterator(const TValueIterator& src)
        : TBase(src), Value(src.Value)
    {
        if (src.Cur == &src.Value)
            Reset(&Value, &Value + 1);
    }
#else
    inline TValueIterator(const TValueIterator& src)
        : TBase(src), ValueVector(src.ValueVector)
    {
        if (src.Cur == src.ValueVector.begin())
            Reset(ValueVector);
    }
#endif

#ifdef USE_INTERNAL_STL
    inline TValueIterator& operator=(const TValueIterator& src) {
        *(TBase*)this = src;
        Value = src.Value;
        if (src.Cur == &src.Value)
            Reset(&Value, &Value + 1);
        return *this;
    }
#else
    inline TValueIterator& operator=(const TValueIterator& src) {
        *(TBase*)this = src;
        ValueVector = src.ValueVector;
        if (src.Cur == src.ValueVector.begin())
            Reset(ValueVector);
        return *this;
    }
#endif

    using TBase::Reset;

#ifdef USE_INTERNAL_STL
    inline void Reset(const T& value) {
        Value = value;
        Reset(&Value, &Value + 1);
    }
#else
    inline void Reset(const T& value) {
        ValueVector = TVector(1, value);
        Reset(ValueVector);
    }
#endif

private:
#ifdef USE_INTERNAL_STL
    typename TTypeTraits<T>::TNonConst Value;
#else
    typedef typename TTypeTraits<T>::TNonConst TNonConst;
    typename TStlContainerSelector<TNonConst>::TVector ValueVector;
#endif
};

} // namespace NGzt
