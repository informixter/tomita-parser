LIBRARY()

ARCHIVE(
    NAME dicteng.inc
    egramtab.tab
    morph.annot
    morph.bases
    morph.forms_autom
    morph.options
    npredict.bin
)

PEERDIR(
    library/archive
)

SRCS(
    dicteng.cpp
)

END()
