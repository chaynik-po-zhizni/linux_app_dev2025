set(ROMAN_SOURCES roman.c)
set(ROMAN_HEADERS roman.h)

add_library(roman SHARED ${ROMAN_SOURCES})

add_library(roman::roman ALIAS roman)

set(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib")
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_MODULE_PATH_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/roman")

target_include_directories(roman
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
)

set_target_properties(roman PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    OUTPUT_NAME "roman"
)

target_compile_definitions(roman
    PRIVATE ROMAN_BUILDING_DLL
)

install(FILES ${ROMAN_HEADERS} 
    DESTINATION ${INCLUDE_INSTALL_DIR}
    COMPONENT roman-development
)

install(TARGETS roman
    EXPORT roman-targets
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT roman-runtime
    INCLUDES DESTINATION ${INCLUDE_INSTALL_DIR}
)

install(EXPORT roman-targets
    FILE roman-config.cmake
    NAMESPACE roman::
    DESTINATION ${CMAKE_MODULE_PATH_INSTALL_DIR}
    COMPONENT roman-development
)