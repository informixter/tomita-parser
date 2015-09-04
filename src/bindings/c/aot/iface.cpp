#include "iface.h"

#include <util/charset/doccodes.h>
#include <util/generic/ptr.h>
#include <util/generic/vector.h>
#include <util/charset/wide.h>

#include "aotlemmer.h"

#define ANALYSES(x) ((TLemmaArray*)(x))
#define LEMMA(x) ((TAotLemma*)(x))
#define FORMS(x) ((TWordformArray*)(x))
#define FORM(x) ((TAotForm*)(x))

extern "C" {
    EXPORT MystemAnalysesHandle* MystemAnalyze(TSymbol* word, int len) {
        try {
            THolder<TLemmaArray> out(new TLemmaArray);
            //NLemmer::AnalyzeWord(word, len, *out.Get(), TLangMask(LANG_RUS));
            Stroka s = WideToUTF8(word, len);
            aotAnalyzeWord(s.c_str(), *out.Get());
            return out.Release();
        } catch (...) {
        }
        return 0;
    }

    EXPORT void MystemDeleteAnalyses(MystemAnalysesHandle* analyses) {
        delete ANALYSES(analyses);
    }

    EXPORT int MystemAnalysesCount(MystemAnalysesHandle* analyses) {
        return ANALYSES(analyses)->size();
    }

    EXPORT MystemLemmaHandle* MystemLemma(MystemAnalysesHandle* analyses, int i) {
        try {
            return &ANALYSES(analyses)->at(i);
        } catch (...) {
        }
        return 0;
    }

    EXPORT TSymbol* MystemLemmaText(MystemLemmaHandle* lemma) {
        return const_cast<TSymbol*>(LEMMA(lemma)->GetLemmaText());
    }

    EXPORT int MystemLemmaTextLen(MystemLemmaHandle* lemma) {
        return LEMMA(lemma)->GetLemmaTextLen();
    }

    EXPORT TSymbol* MystemLemmaForm(MystemLemmaHandle* lemma) {
        return const_cast<TSymbol*>(LEMMA(lemma)->GetFormText());
    }

    EXPORT int MystemLemmaFormLen(MystemLemmaHandle* lemma) {
        return LEMMA(lemma)->GetFormTextLen();
    }

    EXPORT int MystemLemmaQuality(MystemLemmaHandle* lemma) {
        return 0;
    }

    EXPORT char* MystemLemmaStemGram(MystemLemmaHandle* lemma) {
        return const_cast<char*>(LEMMA(lemma)->GetStemGram());
    }
    EXPORT char** MystemLemmaFlexGram(MystemLemmaHandle* lemma) {
        return const_cast<char**>(LEMMA(lemma)->GetFlexGram());
    }

    EXPORT int MystemLemmaFlexGramNum(MystemLemmaHandle* lemma) {
        return LEMMA(lemma)->GetFlexGramNum();
    }

    EXPORT int MystemLemmaFlexLen(MystemLemmaHandle* lemma) {
        return 0;
    }

    EXPORT int MystemLemmaRuleId(MystemLemmaHandle* lemma) {
        return LEMMA(lemma)->GetParadigmId();
    }

    EXPORT MystemFormsHandle* MystemGenerate(MystemLemmaHandle* lemma) {
        try {
            THolder<TWordformArray> out(new TWordformArray);
            //NLemmer::Generate(*LEMMA(lemma), *out.Get());
            LEMMA(lemma)->GenerateAllForm(*out.Get());
            return out.Release();
        } catch (...) {
        }
        return 0;
    }

    EXPORT void MystemDeleteForms(MystemFormsHandle* forms) {
        delete FORMS(forms);
    }

    EXPORT int MystemFormsCount(MystemFormsHandle* forms) {
        return FORMS(forms)->size();
    }

    EXPORT MystemFormHandle* MystemForm(MystemFormsHandle* forms, int i) {
        try {
            return &FORMS(forms)->at(i);
        } catch (...) {
        }
        return 0;
    }

    EXPORT TSymbol* MystemFormText(MystemFormHandle* form) {
        return const_cast<TSymbol*>(FORM(form)->GetFormText());
    }

    EXPORT int MystemFormTextLen(MystemFormHandle* form) {
        return FORM(form)->GetFormTextLen();
    }

    EXPORT char* MystemFormStemGram(MystemFormHandle* form) {
        return const_cast<char*>(FORM(form)->GetStemGram());
    }

    EXPORT char** MystemFormFlexGram(MystemFormHandle* form) {
        return const_cast<char**>(FORM(form)->GetFlexGram());
    }

    EXPORT int MystemFormFlexGramNum(MystemFormHandle* form) {
        return FORM(form)->GetFlexGramNum();
    }
 }
