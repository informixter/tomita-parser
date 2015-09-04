DLL(aot_lemmer 1 0)

OWNER(lazyfrog)

SRCS(
    aotlemmer.cpp
    iface.cpp

    AgramtabLib/agramtab_.cpp
    AgramtabLib/EngGramTab.cpp
    AgramtabLib/StdGramtab.cpp
    common/rus_numerals.cpp
    common/utilit.cpp
    LemmatizerLib/Lemmatizers.cpp
    LemmatizerLib/MorphAutomat.cpp
    LemmatizerLib/MorphDict.cpp
    LemmatizerLib/Paradigm.cpp
    LemmatizerLib/Predict.cpp
    LemmatizerLib/Statistic.cpp
    LemmatizerLib/StdMorph.cpp
    MorphWizardLib/wizard.cpp
)

PEERDIR(
    bindings/c/aot/dicteng
)

END()
