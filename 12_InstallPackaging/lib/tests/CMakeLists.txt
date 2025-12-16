find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
    pkg_check_modules(PC_CHECK QUIET check)
endif()

if(NOT PC_CHECK_FOUND)
    message(FATAL_ERROR "Check library not found. Install check: sudo apt-get install check")
endif()

set(CHECK_INCLUDE_DIRS ${PC_CHECK_INCLUDE_DIRS})
set(CHECK_LIBRARIES ${PC_CHECK_LIBRARIES})

find_program(CHECKMK checkmk)
if(NOT CHECKMK)
    message(FATAL_ERROR "checkmk not found. Install checkmk: sudo apt-get install check")
endif()

set(CK_VERBOSITY "normal" CACHE STRING "Check verbosity: silent, minimal, normal, verbose")
set_property(CACHE CK_VERBOSITY PROPERTY STRINGS silent minimal normal verbose)

function(add_check_test TEST_NAME)
    set(TS_FILE ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.ts)
    set(C_FILE ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.c)
    
    add_custom_command(
        OUTPUT ${C_FILE}
        COMMAND ${CHECKMK} ${TS_FILE} > ${C_FILE}
        DEPENDS ${TS_FILE}
        COMMENT "Generating ${TEST_NAME}.c from ${TEST_NAME}.ts"
    )
    
    add_executable(${TEST_NAME} ${C_FILE})
    
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CHECK_INCLUDE_DIRS}
    )
    
    target_link_libraries(${TEST_NAME} PRIVATE roman ${CHECK_LIBRARIES})
    
    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
    
    set_tests_properties(${TEST_NAME} PROPERTIES
        ENVIRONMENT "CK_VERBOSITY=${CK_VERBOSITY}"
    )
endfunction()

add_check_test(roman_test)
add_check_test(basic)
add_check_test(result)

add_custom_target(build_tests DEPENDS roman_test basic result)

if(ENABLE_GCOV)
    add_custom_target(coverage
        COMMAND ctest --output-on-failure
        DEPENDS build_tests
        COMMENT "Running tests with coverage instrumentation"
    )
    
    add_custom_target(coverage-report
        COMMAND echo "Generating coverage report..."
        COMMAND cd ${CMAKE_BINARY_DIR} && gcov src/CMakeFiles/roman.dir/*.c.gcno || true
        COMMAND echo "Coverage report files:"
        COMMAND cd ${CMAKE_BINARY_DIR} && find . -name "*.gcov" 2>/dev/null || true
        DEPENDS coverage
        COMMENT "Generating coverage report"
    )
    
    add_custom_target(coverage-full
        DEPENDS coverage coverage-report
        COMMENT "Run tests and generate coverage report"
    )
endif()