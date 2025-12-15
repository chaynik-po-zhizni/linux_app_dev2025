cmake_minimum_required(VERSION 3.10)
project(stupid_guessor VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckFunctionExists)

set(PACKAGE_NAME "stupid_guessor")
set(PACKAGE_VERSION "2.0")
set(PACKAGE_BUGREPORT "blabla@mail.ru")

check_include_file(argp.h HAVE_ARGP_H)
if(HAVE_ARGP_H)
    check_symbol_exists(argp_parse "argp.h" HAVE_ARGP_PARSE)
endif()

if(HAVE_ARGP_H AND HAVE_ARGP_PARSE)
    message(STATUS "Found argp (part of standard C library)")
    set(ARGP_LIBRARIES "") 
else()
    message(WARNING "System argp not found. Using bundled version.")
    file(WRITE ${CMAKE_BINARY_DIR}/argp_stub.c "
        #include <stdio.h>
        #include <string.h>
        
        struct argp_state { void *input; };
        struct argp { 
            const void *options;
            int (*parser)(int key, char *arg, struct argp_state *state);
            const char *args_doc;
            const char *doc;
        };
        
        int argp_parse(const struct argp *argp, int argc, char **argv,
                      unsigned flags, int *arg_index, void *input) {
            for (int i = 1; i < argc; i++) {
                if (strcmp(argv[i], \"-r\") == 0 || strcmp(argv[i], \"--roman\") == 0) {
                    *((int*)input) = 1;
                } else if (strcmp(argv[i], \"-x\") == 0 || strcmp(argv[i], \"--extended\") == 0) {
                    *((int*)input + 1) = 1;
                }
            }
            return 0;
        }
    ")
    add_library(argp_stub STATIC ${CMAKE_BINARY_DIR}/argp_stub.c)
    set(ARGP_LIBRARIES argp_stub)
    add_definitions(-DHAVE_ARGP_BUNDLED)
endif()

find_package(Intl REQUIRED)
find_package(Gettext REQUIRED)

find_program(XGETTEXT_EXECUTABLE xgettext)
find_program(MSGMERGE_EXECUTABLE msgmerge)
find_program(MSGFMT_EXECUTABLE msgfmt)

if(NOT XGETTEXT_EXECUTABLE)
    message(FATAL_ERROR "xgettext not found!")
endif()
if(NOT MSGMERGE_EXECUTABLE)
    message(FATAL_ERROR "msgmerge not found!")
endif()
if(NOT MSGFMT_EXECUTABLE)
    message(FATAL_ERROR "msgfmt not found!")
endif()


add_executable(${PROJECT_NAME} src/stupid_guessor.c)

if(ARGP_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES} ${ARGP_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} ${Intl_LIBRARIES})
endif()

set(LOCALE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    MAX_NUMBER=100
    MIN_NUMBER=1
    PACKAGE="${PACKAGE_NAME}"
    LOCALE_PATH="${LOCALE_INSTALL_DIR}"
    PACKAGE_VERSION="${PACKAGE_VERSION}"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${Intl_INCLUDE_DIRS}
)

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/po)

set(POT_FILE "${CMAKE_BINARY_DIR}/po/${PROJECT_NAME}.pot")

add_custom_target(update-po-ru
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/po
    COMMAND ${XGETTEXT_EXECUTABLE} --package-name=${PROJECT_NAME} --package-version=${PROJECT_VERSION} -k_ -kN_ -kP_:1,2 --from-code=UTF-8 -o ${CMAKE_BINARY_DIR}/po/${PROJECT_NAME}.pot ${CMAKE_SOURCE_DIR}/src/stupid_guessor.c
    COMMAND ${MSGMERGE_EXECUTABLE} --update --backup=none ${CMAKE_SOURCE_DIR}/po/ru.po ${CMAKE_BINARY_DIR}/po/${PROJECT_NAME}.pot
    COMMENT "Updating Russian translation"
)

add_custom_target(update-po)
add_dependencies(update-po update-po-ru)


set(MO_FILE "${CMAKE_BINARY_DIR}/po/ru.mo")

add_custom_command(
    OUTPUT ${MO_FILE}
    COMMAND ${MSGFMT_EXECUTABLE}
        -o ${MO_FILE}
        ${CMAKE_SOURCE_DIR}/po/ru.po
    DEPENDS ${CMAKE_SOURCE_DIR}/po/ru.po
    COMMENT "Compiling Russian translation"
)

add_custom_target(po-ru DEPENDS ${MO_FILE})

install(
    FILES ${MO_FILE}
    DESTINATION share/locale/ru/LC_MESSAGES
    RENAME ${PROJECT_NAME}.mo
)

add_custom_target(po-all DEPENDS po-ru)

find_program(HELP2MAN_EXECUTABLE help2man)

if(HELP2MAN_EXECUTABLE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
        COMMAND ${HELP2MAN_EXECUTABLE}
                ./stupid_guessor
                -o ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
        DEPENDS stupid_guessor
        COMMENT "Generating man page"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )
    add_custom_target(man ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/man/man1
            COMPONENT documentation)
endif()
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

find_package(Doxygen REQUIRED dot)

if(DOXYGEN_FOUND)
    message(STATUS "Doxygen найден: ${DOXYGEN_EXECUTABLE}")
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    
    file(GLOB_RECURSE DOXYGEN_SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/*.md
    )
    
    add_custom_target(doxygen-docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Генерация Doxygen документации"
        DEPENDS ${DOXYGEN_SOURCE_FILES} ${CMAKE_BINARY_DIR}/Doxyfile
        VERBATIM
    )
    
    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/html/
        DESTINATION share/doc/${PROJECT_NAME}/html
        COMPONENT documentation
    )
    
else()
    message(WARNING "Doxygen не найден. Установите: sudo apt-get install doxygen graphviz")
endif()

add_custom_target(documentation)
add_dependencies(documentation doxygen-docs)
if(HELP2MAN_EXECUTABLE)
    add_dependencies(documentation man)
endif()
add_dependencies(documentation po-all)

add_custom_target(with-docs)
add_dependencies(with-docs ${PROJECT_NAME} update-po po-all documentation)


add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Full clean including build directory"
)