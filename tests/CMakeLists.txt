project(tests)


# Config option: bastard mode

set(bastard_mode no;always;outofdict)
foreach(m ${bastard_mode})
add_test(NAME bastardmode-${m}
         COMMAND ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:tomita-parser>
                                  -DOPT=config_bastard_mode_${m}.proto
                                  -DGOLD=correct/config_bastard_mode_${m}.proto.out
                                  -DTEMP=${CMAKE_BINARY_DIR}/tests/tmp/
                                  -P run_tomita_and_compare.cmake
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        )
endforeach()


# Gramar: gnc agreement

set(tomita_agr gnc;nc;c;gn;gc)
file(GLOB files "${CMAKE_CURRENT_SOURCE_DIR}/gram_features/texts/gnc-agr-*.txt")
foreach(agrtype ${tomita_agr})
    foreach(file ${files})
        get_filename_component(fn ${file} NAME_WE)

        add_test(NAME ${agrtype}-agr-${fn}
                 COMMAND ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:tomita-parser>
                                          -DOPT=config_${agrtype}agr.proto
                                          -DGOLD=gram_features/correct/${fn}-${agrtype}.out
                                          -DTEMP=${CMAKE_BINARY_DIR}/tests/tmp/
                                          -DIFILE=${file}
                                          -P run_tomita_and_compare.cmake
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )

        add_test(NAME ${agrtype}-agr-neg-${fn}
                 COMMAND ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:tomita-parser>
                                          -DOPT=config_${agrtype}agr_neg.proto
                                          -DGOLD=gram_features/correct/${fn}-${agrtype}-neg.out
                                          -DTEMP=${CMAKE_BINARY_DIR}/tests/tmp/
                                          -DIFILE=${file}
                                          -P run_tomita_and_compare.cmake
                 WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
    endforeach()
endforeach()


# Grammar: alg fio

file(GLOB opencorpora_files "${CMAKE_CURRENT_SOURCE_DIR}/texts/opencorpora/*.txt")
foreach(file ${opencorpora_files})
    get_filename_component(fn ${file} NAME)
    add_test(NAME algfio-${fn}
             COMMAND ${CMAKE_COMMAND} -DCMD=$<TARGET_FILE:tomita-parser>
                                      -DOPT=config_algfio.proto
                                      -DGOLD=algorithms/fio/correct/${fn}.out
                                      -DTEMP=${CMAKE_BINARY_DIR}/tests/tmp/
                                      -DIFILE=${file}
                                      -P run_tomita_and_compare.cmake
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
endforeach()
