cmake_minimum_required(VERSION 2.8)
project(libCGeo)

set(CMAKE_BUILD_TARGET "Shared")
set(CMAKE_BUILD_TYPE "Debug")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "Did not select build type, will build Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
endif()

if(NOT CMAKE_BUILD_TARGET)
    message(STATUS "No build target selected, will build both static and shared library")
    set(CMAKE_BUILD_TARGET "Both" CACHE STRING "" FORCE)
endif()

option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_UTESTS "Build library unit tests" ON)

set(libCGeo_VERSION_MAJOR 0)
set(libCGeo_VERSION_MINOR 0)
set(libCGeo_VERSION_PATCH 0)
set(libCGeo_VERSION ${libCGeo_VERSION_MAJOR}.${libCGeo_VERSION_MINOR}.${libCGeo_VERSION_PATCH})

set(libCGeo_DESCRIPTION "A fast cross platform Computational Geometry Library written in C")
set(libCGeo_URL "https://github.com/jwlodek/libCGeo")


include(GNUInstallDirs)

set(INSTALL_CMAKE_DIR "${libCGeo_SOURCE_DIR}/lib/cmake/libCGeo" CACHE PATH
    "Installation dir for CMake files")

set(SOURCES src/libCGeo.c src/diagnostics.c)

include_directories(${libCGeo_SOURCE_DIR}/include ${libCGeo_BINARY_DIR}/include)

if(${CMAKE_BUILD_TARGET} MATCHES "Shared")
    set(BUILD_CGEO_SHARED TRUE)
elseif(${CMAKE_BUILD_TARGET} MATCHES "Static")
    set(BUILD_CGEO_STATIC TRUE)
elseif(${CMAKE_BUILD_TARGET} MATCHES "Both")
    set(BUILD_CGEO_SHARED TRUE)
    set(BUILD_CGEO_STATIC TRUE)
else()
    message(FATAL_ERROR "Invalid build type ${CMAKE_BUILD_TARGET}")
endif()

if(BUILD_CGEO_SHARED)
    add_library(CGeo SHARED ${SOURCES})
    set_property(TARGET CGeo PROPERTY VERSION ${libCGeo_VERSION})
    set_property(TARGET CGeo PROPERTY SOVERSION ${libCGeo_VERSION_MAJOR})
    list(APPEND CGEO_TARGETS CGeo)
endif()

if(BUILD_CGEO_STATIC)
    add_library(CGeo_static STATIC ${SOURCES})
    set_target_properties(CGeo_static PROPERTIES OUTPUT_NAME CGeo)
    list(APPEND CGEO_TARGETS CGeo_static)
endif()

configure_file(include/libCGeo/libCGeo_config.h.in ${PROJECT_BINARY_DIR}/include/libCGeo/libCGeo_config.h @ONLY)

foreach(target_name ${CGEO_TARGETS})
    set_target_properties(${target_name} PROPERTIES
        PUBLIC_HEADER "include/libCGeo/libCGeo.h;${libCGeo_BINARY_DIR}/include/libCGeo/libCGeo_config.h")
endforeach()


if(BUILD_EXAMPLES)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
    add_executable(diagnostics_example example/diagnostics_example.c)
    target_link_libraries(diagnostics_example m CGeo)

    add_executable(csv_read_example example/csv_reading_example.c)
    target_link_libraries(csv_read_example m CGeo)
endif()

find_package(PkgConfig)

if(BUILD_UTESTS)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/test)
    find_package(criterion QUIET)
    if(CRITERION_FOUND)
        set(CRITERION_LINK_FLAGS ${CRITERION_LIBRARIES})
    else()
        pkg_check_modules(CRITERION QUIET criterion)
        if(CRITERION_FOUND)
            set(CRITERION_INCLUDE_DIR ${CRITERION_INCLUDE_DIRS})
            set(CRITERION_LINK_FLAGS ${CRITERION_LDFLAGS})
        endif()
    endif()
    if(CRITERION_FOUND)
        add_executable(CGeo_test test/libCGeo_tests.c)
        target_link_libraries(CGeo_test CGeo m criterion)
    else()
        message("Criterion library not found, not building unit tests")
    endif()
endif()

if(BUILD_CGEO_SHARED)
    target_link_libraries(CGeo m)
endif()


install(TARGETS ${CGEO_TARGETS}
        EXPORT libCGeoTargets
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}"
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include/libCGeo")

export(TARGETS ${CGEO_TARGETS} FILE "${PROJECT_BINARY_DIR}/libCGeoTargets.cmake")
export(PACKAGE libCGeo)

set(CONF_INCLUDE_DIR "${CMAKE_INSTALL_PREFIX}/include")
set(CONF_LIBRARY_DIR "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}")
set(CONF_LIBRARY "${CMAKE_INSTALL_PREFIX}/lib/${CMAKE_LIBRARY_ARCHITECTURE}/${CMAKE_SHARED_LIBRARY_PREFIX}CGeo${CMAKE_SHARED_LIBRARY_SUFFIX}")

configure_file(libCGeoConfig.cmake.in ${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libCGeoConfig.cmake)

configure_file(libCGeoConfigVersion.cmake.in ${PROJECT_BINARY_DIR}/libCGeoConfigVersion.cmake @ONLY)

configure_file(libCGeo.pc.in ${PROJECT_BINARY_DIR}/libCGeo.pc @ONLY)

install(FILES
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/libCGeoConfig.cmake"
    "{$PROJECT_BINARY_DIR}/libCGeoConfigVersion.cmake"
    DESTINATION "${INSTALL_CMAKE_DIR}")

install(EXPORT libCGeoTargets DESTINATION "${INSTALL_CMAKE_DIR}")

install(FILES "${PROJECT_BINARY_DIR}/libCGeo.pc" DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")
