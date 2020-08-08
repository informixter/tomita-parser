macro(EXEC_CHECK CMD OPTS GOLD TMP IN_FILE)
    string(LENGTH "${IN_FILE}" IN_FILE_NAME_LEN)

    if (${IN_FILE_NAME_LEN} GREATER 0)
        execute_process(COMMAND ${CMD} "--binary-dir=${TMP}" ${OPTS}
                        INPUT_FILE ${IN_FILE}
                        RESULT_VARIABLE CMD_RESULT
                        OUTPUT_VARIABLE TOMITA_OUT)
    else()
        execute_process(COMMAND ${CMD} "--binary-dir=${TMP}" ${OPTS}
                        RESULT_VARIABLE CMD_RESULT
                        OUTPUT_VARIABLE TOMITA_OUT)
    endif()

    if(CMD_RESULT)
        message(FATAL_ERROR "Error running ${CMD} ${OPTS}. Result is \"${CMD_RESULT}\" \"${TOMITA_OUT}\"")
    endif()
    file(READ ${GOLD} CORRECT_RESULT)
    if(NOT TOMITA_OUT STREQUAL CORRECT_RESULT)
        message(FATAL_ERROR "Tomita output is different from expected. Expected output: \"${CORRECT_RESULT}\". Actual output: \"${TOMITA_OUT}\"")
    endif()
endmacro()

exec_check(${CMD} ${OPT} ${GOLD} ${TEMP} "${IFILE}")
