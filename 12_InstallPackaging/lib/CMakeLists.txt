cmake_minimum_required(VERSION 3.10)
project(roman VERSION 1.0.0 LANGUAGES C)

set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME "roman-runtime")

option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BUILD_TESTS "Build tests" ON)
option(ENABLE_GCOV "Enable code coverage" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(ENABLE_GCOV)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lgcov")
endif()

add_subdirectory(src)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

find_package(Doxygen)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen найден: ${DOXYGEN_EXECUTABLE}")

    if(EXISTS ${CMAKE_SOURCE_DIR}/lib) 
        set(INPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib/src)
    else()
        set(INPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/src)
    endif()

    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc/roman)
    
    configure_file(
        ${INPUT_DIRECTORY}/../Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile.roman
        @ONLY
    )

    # doxygen_add_docs(
    #     stupid_guessor-docs 
    #     ${CMAKE_SOURCE_DIR}/src  
    #     COMMENT "Generating Doxygen documentation for Stupid Guessor"
    # )
    
    add_custom_target(doxygen-docs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIRECTORY}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile.roman
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation for Roman"
        VERBATIM
    )
    
    # set(DOXYGEN_PROJECT_NAME "Roman Numerals Library")
    # set(DOXYGEN_PROJECT_NUMBER ${PROJECT_VERSION})
    # set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc/roman)
    # set(DOXYGEN_GENERATE_HTML YES)
    # set(DOXYGEN_GENERATE_MAN NO)
    # set(DOXYGEN_GENERATE_LATEX NO)
    # set(DOXYGEN_EXTRACT_ALL YES)
    # set(DOXYGEN_EXTRACT_PRIVATE YES)
    # set(DOXYGEN_CALLER_GRAPH YES)
    # set(DOXYGEN_CALL_GRAPH YES)
    # set(DOXYGEN_HIDE_UNDOC_MEMBERS YES)
    # set(DOXYGEN_HIDE_UNDOC_CLASSES YES)

    # doxygen_add_docs(
    #     doxygen-docs  # Имя цели
    #     ${INPUT_DIRECTORY}/  # Исходные файлы
    #     COMMENT "Doxygen generation documentation"
    # )
    
    # Создаем общую цель documentation
    add_custom_target(roman-documentation)
    add_dependencies(roman-documentation doxygen-docs)
    
    # Установка документации
    install(
        DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/html/
        DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/roman
        COMPONENT roman-documentation
    )
    
else()
    message(WARNING "Doxygen not found")
endif()