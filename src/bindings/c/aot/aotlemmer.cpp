
#include <util/charset/wide.h>
#include <util/stream/output.h>
#include <util/generic/map.h>


#include "LemmatizerLib/Lemmatizers.h"
#include "AgramtabLib/EngGramTab.h"

#include "aotlemmer.h"

static CLemmatizer* g_Lemmatizer = NULL;
static CEngGramTab* g_GramTab = NULL;

struct Ancode {
    char ancode[3];
    char stemGram[8];
    char flexGram[8];
};

// egramtab.tab
Ancode g_Ancodes[] = {
  { "aa", { gAdjective, 0 }, { 0 } },
  { "ab", { gAdjective, 0 }, { gComparative, 0 } },
  { "ac", { gAdjective, 0 }, { gSuperlative, 0 } },
  { "ad", { gAdjective, 0 }, { gProper , 0 } },

  { "ba", { gAdverb, 0 }, { 0 } },
  { "ba", { gAdverb, 0 }, { gComparative, 0 } },
  { "ba", { gAdverb, 0 }, { gSuperlative, 0 } },

  { "xi", { gNumeral, 0 }, { 0 } },
  { "cb", { gNumeral, 0 }, { gComparative, 0 } },
  { "cc", { gNumeral, 0 }, { gSuperlative, 0 } },

  { "va", { gVerb, 0 }, { gInfinitive, 0 } },
  { "vb", { gVerb, 0 }, { gPresent, gSingular, gPerson3, 0 } },
  { "vc", { gVerb, 0 }, { gPast, 0 } },
  { "vd", { gVerb, 0 }, { gPraedic, gParticiple, 0 } },
  { "ve", { gVerb, 0 }, { gGerund, 0 } },

  // Модальные глаголы
  { "vf", { gVerb, 0 }, { gInfinitive, 0 } },
  { "vh", { gVerb, 0 }, { gPast, 0 } },

  // Глагол to be
  { "ta", { gVerb, 0 }, { gInfinitive, 0 } },
  { "tb", { gVerb, 0 }, { gPresent, gSingular, gPerson1, 0 } },
  { "td", { gVerb, 0 }, { gPresent, gSingular, gPerson3, 0 } },
  { "te", { gVerb, 0 }, { gPresent, gPlural, 0 } },
  { "tf", { gVerb, 0 }, { gGerund, 0 } },
  { "tg", { gVerb, 0 }, { gPast, gSingular, 0 } },
  { "ti", { gVerb, 0 }, { gPast, gPlural, 0 } },
  { "tj", { gVerb, 0 }, { gPraedic, gParticiple, 0 } },
  { "tk", { gVerb, 0 }, { gFuture, gSingular, gPerson1, 0 } },
  { "tl", { gVerb, 0 }, { gFuture, gSingular, gPlural, gPerson1, gPerson2, gPerson3, 0 } },
  { "tm", { gVerb, 0 }, { gConditional, gSingular, gPerson1, gPerson2, 0 } },
  { "tn", { gVerb, 0 }, { gConditional, gSingular, gPerson3, 0 } },
  { "to", { gVerb, 0 }, { gConditional, gPlural, 0 } },

  //
  { "pa", { gSubstPronoun, 0 }, { gNominative, 0 } }, // "who"  // граммемы "pers" нет в Яндексе?
  { "pb", { gSubstPronoun, 0 }, { /*obj,*/ 0 } },            // "whom"
  { "pc", { gSubstPronoun, 0 }, { gNominative, gSingular, gPerson1, 0 } },
  { "pd", { gSubstPronoun, 0 }, { /*obj,*/ gSingular, gPerson1, 0 } },
  { "pe", { gSubstPronoun, 0 }, { gNominative, gPerson2, 0 } },
  { "pf", { gSubstPronoun, 0 }, { /*obj,*/ gPerson2, 0 } },
  { "pg", { gSubstPronoun, 0 }, { gNominative, gSingular, gPerson3, 0 } }, // "whoever"
  { "ph", { gSubstPronoun, 0 }, { /*obj,*/ gSingular, gPerson3, 0 } },            // "whomever"

  { "pi", { gSubstPronoun, 0 }, { gNominative, gPlural, gPerson1, 0 } },
  { "pk", { gSubstPronoun, 0 }, { /*obj,*/ gPlural, gPerson1, 0 } },
  { "pl", { gSubstPronoun, 0 }, { gNominative, gPlural, gPerson3, 0 } },
  { "pm", { gSubstPronoun, 0 }, { /*obj,*/ gPlural, gPerson3, 0 } },

  { "da", { gSubstPronoun, 0 }, { gReflexive, gSingular, 0 } }, // myself
  { "db", { gSubstPronoun, 0 }, { gReflexive, gPlural, 0 } }, // ourselves

  { "ea", { gAdjPronoun, 0 }, { gPossessive, 0 } }, // her
  { "eb", { gAdjPronoun, 0 }, { gPossessive, gPredic /*pred*/, 0 } }, // hers, mine
  { "ec", { gAdjPronoun, 0 }, { /*dem,*/ gSingular, 0 } }, // this
  { "ed", { gAdjPronoun, 0 }, { /*dem,*/ gPlural, 0 } }, // these
  { "ee", { gAdjPronoun, 0 }, { 0 } }, // any, another, such, what, which, whose
  { "ef", { gSubstPronoun, 0 }, { 0 } }, // all, anybody, anyone, AS!, BUT!

  { "na", { gSubstantive, 0 }, { /*narr,*/ gSingular, 0 } },
  { "nb", { gSubstantive, 0 }, { /*narr,*/ gPlural, 0 } },

  // analytical possessive
  // fa 1 NOUN narr,poss

  { "nc", { gSubstantive, 0 }, { /*narr, mass, uncount,*/ gSingular, 0 } }, // silk, clay

  //  analytical possessive
  // fb 1 NOUN narr,mass,uncount,poss

  { "ne", { gSubstantive, 0 }, { /*narr, mass,*/ gSingular, 0 } }, // water, butter
  { "ng", { gSubstantive, 0 }, { /*narr, mass,*/ gPlural, 0 } },

  //  analytical possessive
  // fc 1 NOUN narr,mass,poss

  { "ni", { gSubstantive, 0 }, { /*narr, uncount,*/ gSingular, 0 } },

  // "John", "James"
  { "oa", { gSubstantive, 0 }, { gProper, gFirstName, gMasculine, gSingular, 0 } },
  { "ob", { gSubstantive, 0 }, { gProper, gFirstName, gMasculine, gPlural, 0 } },

  //  analytical possessive
  // fd 1 NOUN prop,m,poss

  // "Mary", "Jane"
  { "oc", { gSubstantive, 0 }, { gProper, gFirstName, gFeminine, gSingular, 0 } },
  { "od", { gSubstantive, 0 }, { gProper, gFirstName, gFeminine, gPlural, 0 } },

  //  analytical possessive
  // fe 1 NOUN prop,f,poss

  // "Glen", "Lee", "Jerry"
  { "oe", { gSubstantive, 0 }, { gProper, gFirstName, gMasFem, gSingular, 0 } },
  { "of", { gSubstantive, 0 }, { gProper, gFirstName, gMasFem, gPlural, 0 } },

  //  analytical possessive
  // ff 1 NOUN prop,m,f,poss

  { "ga", { gSubstantive, 0 }, { gProper, gGeo, 0 } },

  //  analytical possessive
  // fg 1 NOUN prop,poss

  { "xa", { gConjunction, 0 }, { 0 } },
  { "xb", { gInterjunction, 0 }, { 0 } },
  { "xc", { gPreposition, 0 }, { 0 } },
  { "xd", { gParticle, 0 }, { 0 } },
  { "xf", { gArticle, 0 }, { 0 } },
  { "xi", { gNumeral, 0 }, { 0 } },
  { "xp", { gAdjNumeral, 0 }, { 0 } },

  { "yc", { gPostposition, 0 }, { /*plsq,*/ gSingular, 0 } },
  { "yd", { gPostposition, 0 }, { /*plsgs,*/ gPlural, 0 } },

  // в egramtab.tab тут написано что-то непонятное
  // в morph.mrd это слово DURNOVO
  { "xx", { gSubstantive, 0 }, { gProper, gSingular, gPlural, 0 } },

  // type ancodes
  { "za", { 0 }, { gGeo, 0 } },
  { "zb", { 0 }, { gFirstName, 0 } },
  { "zc", { 0 }, { /*org,*/ 0 } },

  { { 0 }, { 0 }, { 0 } }
};

Ancode* find_ancode(const std::string &s) {
    Ancode *p = g_Ancodes;
    while (0 != p->ancode[0]) {
        if (s == std::string(p->ancode))
            return p;
        p++;
    }
    return p;
}


void copy_from_aot(TAotLemma& aotLemma, const CFormInfo &fi, const char* word);
void init_ancodes2yxgrammems();

CAgramtab* GetGramTab () {
  if (NULL == g_GramTab) {
    g_GramTab = new CEngGramTab;
    if (!g_GramTab->Load(/*"/home/victor/soft/seman-svn/trunk/Dicts/Morph/*/ "egramtab.tab")) {
//    if (!g_GramTab->Load("/tmp/Morph/egramtab.tab")) {
      Cerr << "Can't load EngGramTab" << Endl;
      delete g_GramTab;
      g_GramTab = NULL;
    }
  }

  return g_GramTab;
}

CLemmatizer* GetLemmatizer() {
  if (NULL == g_Lemmatizer) {
    if (!GetGramTab())
        return NULL;
    g_Lemmatizer = new CLemmatizerEnglish;
    std::string sError;
    if (!g_Lemmatizer->LoadDictionaries("", sError)) {
//    if (!g_Lemmatizer->LoadDictionaries("/tmp/Morph/Eng", sError)) {
        Cerr << "g_Lemmatizer->LoadDictionaries failed: " << sError.c_str() << Endl;
        delete g_Lemmatizer;
        g_Lemmatizer = NULL;
    }
  }

  return g_Lemmatizer;
}

void aotAnalyzeWord(const char* word, std::vector<TAotLemma>& aotLemmata) {
  std::vector<CFormInfo> results;
  std::string w(word);
  GetLemmatizer()->CreateParadigmCollection(false, w, true, true, results);

  aotLemmata.clear();
  aotLemmata.resize(results.size());

  for (size_t i = 0; i < results.size(); i++)
    copy_from_aot(aotLemmata[i], results[i], word);
}

inline unsigned short int s2i(const char* s) {
    char x[2];
    x[0] = s[0];
    x[1] = s[1];
    return (unsigned short int)(*x);
}

void copy_from_aot(TAotLemma& aotLemma, const CFormInfo &fi, const char* word) {
//  if (0 == g_Ancodes2YxGrammems.size())
//    init_ancodes2yxgrammems();

  //StemFlexGrm sf = g_Ancodes2YxGrammems[s2i(fi.GetSrcAncode().c_str())];
  Ancode *pA = find_ancode(fi.GetSrcAncode());
  std::vector<const char*> vFlexGrm;
  vFlexGrm.push_back(pA->flexGram);
  aotLemma.Init(fi.m_bFound?UTF8ToWide(fi.GetSrcNorm()):UTF8ToWide(Stroka(word)), UTF8ToWide(Stroka(word)), pA->stemGram, vFlexGrm, fi.GetParadigmId());


//  string ancode = fi.GetSrcAncode();
//  QWORD qw = GetGramTab()->GetAllGrammems(ancode.c_str());
//  for (size_t i = 0; i < 32; i++) {
//      DWORD p = 1 << i;
//      if (qw & p) {
//        Cerr << "1[" << word << "] grm = " << i << " " << p << " " << GetGramTab()->GetGrammemStr(i) << Endl;
//      }
//  }

//  qw = GetGramTab()->GetAllGrammems(fi.GetCommonAncode().c_str());
//  for (size_t i = 0; i < 32; i++) {
//      DWORD p = 1 << i;
//      if (qw & p) {
//        Cerr << "2[" << word << "] grm = " << i << " " << p << " " << GetGramTab()->GetGrammemStr(i) << Endl;
//      }
//  }

//  Cerr << "POS [" << word << "] = " << GetGramTab()->GetPartOfSpeechStr(GetGramTab()->GetPartOfSpeech(ancode.c_str())) << Endl;

}

bool TAotLemma::GenerateAllForm(TWordformArray &array) {
  std::vector<CFormInfo> results;
  std::string word = WideToUTF8(strLemma);
  GetLemmatizer()->CreateParadigmCollection(false, word, true, true, results);

  size_t iResult = 0;
  for (size_t i = 0; i < results.size(); i++) {
    if (iParadigmId == results[i].GetParadigmId()) {
        iResult = i;
        break;
    }
  }

  CFormInfo &fi = results[iResult];

  for (size_t i = 0; i < fi.GetCount(); i++) {
    std::string strForm = fi.GetWordForm(i);
    Ancode *pA = find_ancode(fi.GetAncode(i));
    std::vector<const char*> vFlexGrm;
    vFlexGrm.push_back(pA->flexGram);
    TAotForm form;
    form.Init(UTF8ToWide(strForm), pA->stemGram, vFlexGrm);
    array.push_back(form);
  }


  return true;
}

/*void init_ancodes2yxgrammems() {
  g_Ancodes2YxGrammems[s2i("na")] = StemFlexGrm(gSubstantive, gSingular);

  g_Ancodes2YxGrammems[s2i("nb")] = StemFlexGrm(gSubstantive, gPlural);

}*/
