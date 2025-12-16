cmake_minimum_required(VERSION 3.10)
project(stupid_guessor VERSION 3.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ============================================
# Настройка поиска библиотеки roman
# ============================================
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CMAKE_INSTALL_PREFIX}/lib/cmake")


if(EXISTS ${CMAKE_SOURCE_DIR}/lib/CMakeLists.txt)
    add_subdirectory(${CMAKE_SOURCE_DIR}/lib lib)
    set(ROMAN_TARGET roman)  
endif()

if(NOT TARGET ${ROMAN_TARGET})
    message(FATAL_ERROR "Roman library target '${ROMAN_TARGET}' not found.")
endif()

# ============================================
# Настройки программы
# ============================================
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckFunctionExists)

set(PACKAGE_NAME "stupid_guessor")
set(PACKAGE_VERSION "${PROJECT_VERSION}")
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
    set(HAVE_ARGP_BUNDLED 1)
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

# ============================================
# Настройка локализации
# ============================================
set(BUILD_LOCALE_DIR "${CMAKE_BINARY_DIR}/locale")
set(INSTALL_LOCALE_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")

# ============================================
# Основной исполняемый файл
# ============================================
add_executable(${PROJECT_NAME} src/stupid_guessor.c)

# Линковка с библиотекой roman
target_link_libraries(${PROJECT_NAME} PRIVATE ${ROMAN_TARGET})

if(ARGP_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Intl_LIBRARIES} ${ARGP_LIBRARIES})
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${Intl_LIBRARIES})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}
    ${Intl_INCLUDE_DIRS}
)

set(LOCALE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/locale")

target_compile_definitions(${PROJECT_NAME} PRIVATE
    PACKAGE="${PACKAGE_NAME}"
    LOCALE_PATH="${LOCALE_INSTALL_DIR}"
    PACKAGE_VERSION="${PACKAGE_VERSION}"
)

# ============================================
# Переводы и локализация
# ============================================

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
add_dependencies(${PROJECT_NAME} po-all)

# ============================================
# man-страницы
# ============================================
find_program(HELP2MAN_EXECUTABLE help2man)

if(HELP2MAN_EXECUTABLE)
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
        COMMAND ${HELP2MAN_EXECUTABLE}
                ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor
                -o ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
                --no-info
                --version-string=${PROJECT_VERSION}
        DEPENDS ${PROJECT_NAME}
        COMMENT "Generating man page"
    )
    add_custom_target(stupid_guessor-man ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/stupid_guessor.1
            DESTINATION share/man/man1
            COMPONENT documentation)
endif()

# ============================================
# Установка программы
# ============================================
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# ============================================
# Документация программы
# ============================================
find_package(Doxygen)
if(DOXYGEN_FOUND)
    message(STATUS "Doxygen found: ${DOXYGEN_EXECUTABLE}")
    
    set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/doc/stupid_guessor)
    
    configure_file(
        ${CMAKE_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile.stupid_guessor
        @ONLY
    )
    
    add_custom_target(stupid_guessor-docs
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIRECTORY}
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile.stupid_guessor
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating Doxygen documentation for Stupid Guessor"
        VERBATIM
    )
    
    install(DIRECTORY ${DOXYGEN_OUTPUT_DIRECTORY}/html/
            DESTINATION ${CMAKE_INSTALL_PREFIX}/share/doc/stupid_guessor
            COMPONENT documentation)
else()
    message(WARNING "Doxygen not found. Install: sudo apt-get install doxygen graphviz")
endif()


# ============================================
# Установка библиотеки roman
# ============================================
add_custom_target(install-roman-lib
    COMMAND ${CMAKE_COMMAND} --install . --component roman-runtime
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    COMMENT "Installing Roman library"
)
    
install(TARGETS roman
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    COMPONENT roman-runtime
)

if(EXISTS ${CMAKE_SOURCE_DIR}/lib/src/roman.h)
    install(FILES ${CMAKE_SOURCE_DIR}/lib/src/roman.h
            DESTINATION include
            COMPONENT roman-development)
endif()
    
install(
    EXPORT roman-targets
    FILE roman-config.cmake
    NAMESPACE roman::
    DESTINATION lib/cmake/roman
    COMPONENT roman-development
)
    
add_dependencies(${PROJECT_NAME} roman)
    
set(CPACK_COMPONENTS_ALL ${PROJECT_NAME} roman-runtime roman-development roman-documentation)

# ============================================
# Цели сборки
# ============================================
add_custom_target(stupid-guessor-documentation)
if(DOXYGEN_FOUND)
    add_dependencies(stupid-guessor-documentation stupid_guessor-docs)
endif()
if(HELP2MAN_EXECUTABLE)
    add_dependencies(stupid-guessor-documentation stupid_guessor-man)
endif()

# ============================================
# Деинсталляция с поддержкой библиотеки roman
# ============================================
configure_file(
    ${CMAKE_SOURCE_DIR}/cmake_uninstall.cmake.in
    ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    @ONLY
)

add_custom_target(uninstall
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake_uninstall.cmake
    COMMENT "Uninstalling ${PROJECT_NAME} and Roman library (if installed)"
)

# ============================================
# Общая цель для полной сборки
# ============================================

    add_custom_target(with-deps-all
        DEPENDS ${PROJECT_NAME} stupid-guessor-documentation roman-documentation
        COMMENT "Building stupid_guessor with all dependencies"
    )


# ============================================
# Очистка
# ============================================
add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
    COMMENT "Cleaning build directory (standard clean)"
)

add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E echo "Removing build directory: ${CMAKE_BINARY_DIR}"
    COMMAND cd ${CMAKE_SOURCE_DIR} && ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Full clean: removing entire build directory"
)


# ============================================
# Информация
# ============================================
message(STATUS "")
message(STATUS "Build configuration for ${PROJECT_NAME} ${PROJECT_VERSION}")
message(STATUS "===========================================")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Roman library: ${ROMAN_TARGET}")
message(STATUS "Documentation: ${DOXYGEN_FOUND}")
message(STATUS "===========================================")
