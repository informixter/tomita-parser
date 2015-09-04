#include <library/archive/yarchive.h>
#include <util/stream/input.h>
#include <util/generic/singleton.h>
#include "dicteng.h"

static const unsigned char dictEngArchive[] = {
    #include "dicteng.inc"
};

class TDictEngReader {
public:
    TDictEngReader()
        : Reader(TBlob::NoCopy(dictEngArchive, sizeof(dictEngArchive)))
    {
    }

    Stroka GetFileByName(const Stroka& fileName) {
        return Reader.ObjectByKey("/" + fileName).Get()->ReadAll();
    }

    size_t GetSizeByName(const Stroka& fileName) {
        return Reader.ObjectBlobByKey("/" + fileName).Length();
    }

    TInputStream* GetObject(const Stroka& fileName) {
        TAutoPtr<TInputStream> ptr = Reader.ObjectByKey("/" + fileName);
        TInputStream *pInput = ptr.Get();
        ptr.Release();
        return pInput;
    }

private:
    TArchiveReader Reader;
};

size_t GetObjectSizeByName(const Stroka& fileName) {
  return Singleton<TDictEngReader>()->GetSizeByName(fileName);
}

TInputStream* GetObjectByName(const Stroka& fileName) {
  return Singleton<TDictEngReader>()->GetObject(fileName);
}

Stroka GetMorphFileByName(const Stroka& fileName) {
    return Singleton<TDictEngReader>()->GetFileByName(fileName);
}

