LIBRARY()

NOUTIL()

IF (NOT WIN32)
    WERROR()
ENDIF ()

SRCS(
    bitutils.cpp                  # comptrie
    format.cpp                    # kernel/gazetteer
)

IF (USE_INTERNAL_STL)
    SRCS(
        delim_file_iter.cpp       # util/draft
    )
ENDIF()

PEERDIR(
    library/svnversion
)

END()
