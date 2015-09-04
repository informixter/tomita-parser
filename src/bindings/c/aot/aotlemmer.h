
#include <util/generic/stroka.h>
//#include <util/generic/vector.h>
#include <vector>

#include <library/lemmer/dictlib/yx_gram_enum.h>

class TAotForm {
  Wtroka strForm;
  const char* stemGrm;
  const char** flexGrm;
  size_t flexGrmNum;

public:
  TAotForm()
    : stemGrm(NULL)
    , flexGrm(NULL)
    , flexGrmNum(0) { }

  ~TAotForm() {
    if (NULL != flexGrm)
      delete[] flexGrm;
  }

  TAotForm(const TAotForm& other) {
    copy_from_other(other);
  }

  void copy_from_other(const TAotForm& other) {
    if (other.flexGrmNum > 0 && NULL != other.flexGrm) {
      flexGrm = new const char* [flexGrmNum = other.flexGrmNum];
      for (size_t i = 0; i < flexGrmNum; i++)
        flexGrm[i] = other.flexGrm[i];
    }

    strForm = other.strForm;
    stemGrm = other.stemGrm;
  }

  void Init(const Wtroka& _strForm,
            const char* _stemGrm, const std::vector<const char*> _vflexGrm) {
    strForm = _strForm;
    stemGrm = _stemGrm;

    if (_vflexGrm.size() > 0) {
      flexGrm = new const char* [flexGrmNum = _vflexGrm.size()];
      for (size_t i = 0; i < flexGrmNum; i++)
        flexGrm[i] = _vflexGrm[i];
    }
  }

  const unsigned short int* GetFormText() {
    return strForm.c_str();
  }

  size_t GetFormTextLen() {
    return strForm.size();
  }

  const char* GetStemGram() {
    return stemGrm;
  }

  const char** GetFlexGram() {
    return flexGrm;
  }

  int GetFlexGramNum() {
    return flexGrmNum;
  }
}; // в Яндексе это TYandexWordform;
typedef std::vector<TAotForm> TWordformArray;

class TAotLemma : public TAotForm {
    Wtroka strLemma;
    int iParadigmId;

public:
    TAotLemma()
        : TAotForm()
        , iParadigmId(0) { }

    void Init(const Wtroka& _strLemma, const Wtroka& _strForm,
              const char* _stemGrm, const std::vector<const char*> _vflexGrm,
              int _paradigmId) {
      TAotForm::Init(_strForm, _stemGrm, _vflexGrm);
      strLemma = _strLemma;
      iParadigmId = _paradigmId;
    }

    const unsigned short int* GetLemmaText() {
        return strLemma.c_str();
    }

    size_t GetLemmaTextLen() {
        return strLemma.size();
    }

    int GetParadigmId() {
      return iParadigmId;
    }

    bool GenerateAllForm(TWordformArray &array);
}; // в Яндексе это TYandexLemma, в АОТ это CFormInfo
typedef std::vector<TAotLemma> TLemmaArray; // TWLemmaArray

void aotAnalyzeWord(const char* word, std::vector<TAotLemma>& aotLemmata);
