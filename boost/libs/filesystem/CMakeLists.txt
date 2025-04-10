# Copyright 2019 Mike Dev
# Copyright 2020-2022 Andrey Semashev
#
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt
#
# NOTE: CMake support for Boost.Filesystem is currently experimental at best
#       and the interface is likely to change in the future

cmake_minimum_required(VERSION 3.5...3.16)
project(BoostFilesystem VERSION "${BOOST_SUPERPROJECT_VERSION}" LANGUAGES CXX)

include(CheckCXXSourceCompiles)

set(BOOST_FILESYSTEM_NO_DEPRECATED OFF CACHE BOOL "Disable deprecated functionality of Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_SENDFILE OFF CACHE BOOL "Disable usage of sendfile API in Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_COPY_FILE_RANGE OFF CACHE BOOL "Disable usage of copy_file_range API in Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_STATX OFF CACHE BOOL "Disable usage of statx API in Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_GETRANDOM OFF CACHE BOOL "Disable usage of getrandom API in Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_ARC4RANDOM OFF CACHE BOOL "Disable usage of arc4random API in Boost.Filesystem")
set(BOOST_FILESYSTEM_DISABLE_BCRYPT OFF CACHE BOOL "Disable usage of BCrypt API in Boost.Filesystem")
set(BOOST_FILESYSTEM_EMSCRIPTEN_USE_WASI OFF CACHE BOOL "Use WASI under emscripten in Boost.Filesystem")

# Note: We can't use the Boost::library targets in the configure checks as they may not yet be included
# by the superproject when this CMakeLists.txt is included. We also don't want to hardcode include paths
# of the needed libraries and their dependencies, recursively, as this is too fragile and requires maintenance.
# Instead, we collect include paths of all libraries and use them in the configure checks. This works faster
# if there is a unified Boost include tree in the filesystem (i.e. if `b2 headers` was run or we're in the
# official monolithic Boost distribution tree).
include(cmake/BoostLibraryIncludes.cmake)

set(CMAKE_REQUIRED_INCLUDES "${CMAKE_CURRENT_SOURCE_DIR}/src" ${BOOST_LIBRARY_INCLUDES})

check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_attribute_init_priority.cpp>" BOOST_FILESYSTEM_HAS_INIT_PRIORITY)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_cxx20_atomic_ref.cpp>" BOOST_FILESYSTEM_HAS_CXX20_ATOMIC_REF)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_blksize.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_BLKSIZE)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_mtim.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_MTIM)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_mtimensec.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_MTIMENSEC)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_mtimespec.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_MTIMESPEC)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_birthtimensec.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMENSEC)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_stat_st_birthtimespec.cpp>" BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMESPEC)
if(NOT BOOST_FILESYSTEM_DISABLE_STATX)
    check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_statx.cpp>" BOOST_FILESYSTEM_HAS_STATX)
    if(NOT BOOST_FILESYSTEM_HAS_STATX)
        check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_statx_syscall.cpp>" BOOST_FILESYSTEM_HAS_STATX_SYSCALL)
    endif()
endif()
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_fdopendir_nofollow.cpp>" BOOST_FILESYSTEM_HAS_FDOPENDIR_NOFOLLOW)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_dirent_d_type.cpp>" BOOST_FILESYSTEM_HAS_DIRENT_D_TYPE)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_posix_at_apis.cpp>" BOOST_FILESYSTEM_HAS_POSIX_AT_APIS)
check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_fallocate.cpp>" BOOST_FILESYSTEM_HAS_FALLOCATE)
if(WIN32 AND NOT BOOST_FILESYSTEM_DISABLE_BCRYPT)
    set(CMAKE_REQUIRED_LIBRARIES bcrypt)
    check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_bcrypt.cpp>" BOOST_FILESYSTEM_HAS_BCRYPT)
    unset(CMAKE_REQUIRED_LIBRARIES)
endif()

unset(CMAKE_REQUIRED_INCLUDES)

set(BOOST_FILESYSTEM_SOURCES
    src/codecvt_error_category.cpp
    src/exception.cpp
    src/operations.cpp
    src/directory.cpp
    src/path.cpp
    src/path_traits.cpp
    src/portability.cpp
    src/unique_path.cpp
    src/utf8_codecvt_facet.cpp
)
if(WIN32 OR CYGWIN)
    list(APPEND BOOST_FILESYSTEM_SOURCES src/windows_file_codecvt.cpp)
endif()

add_library(boost_filesystem ${BOOST_FILESYSTEM_SOURCES})
add_library(Boost::filesystem ALIAS boost_filesystem)

target_compile_features(boost_filesystem PUBLIC
    cxx_rvalue_references
    cxx_strong_enums
    cxx_noexcept
    cxx_nullptr
    cxx_defaulted_functions
    cxx_defaulted_move_initializers
    cxx_deleted_functions
    cxx_default_function_template_args
    cxx_final
    cxx_override
)

get_target_property(BOOST_FILESYSTEM_TARGET_TYPE boost_filesystem TYPE)
if(BOOST_FILESYSTEM_TARGET_TYPE STREQUAL "SHARED_LIBRARY")
    set(CMAKE_REQUIRED_LIBRARIES "-Wl,--no-undefined")
    check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_linkflag_no_undefined.cpp>" BOOST_FILESYSTEM_HAS_LINKFLAG_NO_UNDEFINED)
    unset(CMAKE_REQUIRED_LIBRARIES)
    if(NOT BOOST_FILESYSTEM_HAS_LINKFLAG_NO_UNDEFINED)
        set(CMAKE_REQUIRED_LIBRARIES "-Wl,-undefined,error")
        check_cxx_source_compiles("#include <${CMAKE_CURRENT_SOURCE_DIR}/config/has_linkflag_no_undefined.cpp>" BOOST_FILESYSTEM_HAS_LINKFLAG_UNDEFINED_ERROR)
        unset(CMAKE_REQUIRED_LIBRARIES)
    endif()
    if(BOOST_FILESYSTEM_HAS_LINKFLAG_NO_UNDEFINED)
        if(NOT CMAKE_VERSION VERSION_LESS 3.13)
            target_link_options(boost_filesystem PRIVATE "-Wl,--no-undefined")
        else()
            target_link_libraries(boost_filesystem PRIVATE "-Wl,--no-undefined")
        endif()
    elseif(BOOST_FILESYSTEM_HAS_LINKFLAG_UNDEFINED_ERROR)
        if(NOT CMAKE_VERSION VERSION_LESS 3.13)
            target_link_options(boost_filesystem PRIVATE "-Wl,-undefined,error")
        else()
            target_link_libraries(boost_filesystem PRIVATE "-Wl,-undefined,error")
        endif()
    endif()
endif()

target_include_directories(boost_filesystem PUBLIC include)
target_include_directories(boost_filesystem PRIVATE src)

target_compile_definitions(boost_filesystem
    PUBLIC
        # NOTE:
        # We deactivate autolinking, because cmake based builds don't need it
        # and we don't implement name mangling for the library file anyway.
        # Ususally the parent CMakeLists.txt file should already have globally defined BOOST_ALL_NO_LIB
        BOOST_FILESYSTEM_NO_LIB
        $<$<STREQUAL:$<TARGET_PROPERTY:boost_filesystem,TYPE>,SHARED_LIBRARY>:BOOST_FILESYSTEM_DYN_LINK=1>
        $<$<STREQUAL:$<TARGET_PROPERTY:boost_filesystem,TYPE>,STATIC_LIBRARY>:BOOST_FILESYSTEM_STATIC_LINK=1>

    PRIVATE
        BOOST_FILESYSTEM_SOURCE
)

if(WIN32)
    target_compile_definitions(boost_filesystem PRIVATE
        _SCL_SECURE_NO_WARNINGS
        _SCL_SECURE_NO_DEPRECATE
        _CRT_SECURE_NO_WARNINGS
        _CRT_SECURE_NO_DEPRECATE
    )
endif()

if(WIN32 OR CYGWIN)
    target_compile_definitions(boost_filesystem PRIVATE
        BOOST_USE_WINDOWS_H
        _WIN32_WINNT=0x0A00
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
    (CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 12 OR CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 12) AND
    CMAKE_CXX_COMPILER_VERSION VERSION_LESS 13)
    # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105329
    # https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105651
    target_compile_options(boost_filesystem PRIVATE
        -Wno-restrict
    )
endif()

if(BOOST_FILESYSTEM_NO_DEPRECATED)
    target_compile_definitions(boost_filesystem PUBLIC BOOST_FILESYSTEM_NO_DEPRECATED)
endif()

if(BOOST_FILESYSTEM_HAS_INIT_PRIORITY)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_INIT_PRIORITY)
endif()

if(BOOST_FILESYSTEM_DISABLE_SENDFILE)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_SENDFILE)
endif()
if(BOOST_FILESYSTEM_DISABLE_COPY_FILE_RANGE)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_COPY_FILE_RANGE)
endif()
if(BOOST_FILESYSTEM_DISABLE_STATX)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_STATX)
endif()
if(BOOST_FILESYSTEM_DISABLE_GETRANDOM)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_GETRANDOM)
endif()
if(BOOST_FILESYSTEM_DISABLE_ARC4RANDOM)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_ARC4RANDOM)
endif()
if(BOOST_FILESYSTEM_DISABLE_BCRYPT)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_BCRYPT)
endif()
if(BOOST_FILESYSTEM_DISABLE_EMSCRIPTEN_WASI)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_DISABLE_EMSCRIPTEN_WASI)
endif()

if(BOOST_FILESYSTEM_HAS_STAT_ST_BLKSIZE)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_BLKSIZE)
endif()
if(BOOST_FILESYSTEM_HAS_STAT_ST_MTIM)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_MTIM)
endif()
if(BOOST_FILESYSTEM_HAS_STAT_ST_MTIMENSEC)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_MTIMENSEC)
endif()
if(BOOST_FILESYSTEM_HAS_STAT_ST_MTIMESPEC)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_MTIMESPEC)
endif()
if(BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMENSEC)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMENSEC)
endif()
if(BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMESPEC)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STAT_ST_BIRTHTIMESPEC)
endif()
if(BOOST_FILESYSTEM_HAS_STATX)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STATX)
endif()
if(BOOST_FILESYSTEM_HAS_STATX_SYSCALL)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_STATX_SYSCALL)
endif()
if(BOOST_FILESYSTEM_HAS_FDOPENDIR_NOFOLLOW)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_FDOPENDIR_NOFOLLOW)
endif()
if(BOOST_FILESYSTEM_HAS_DIRENT_D_TYPE)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_DIRENT_D_TYPE)
endif()
if(BOOST_FILESYSTEM_HAS_POSIX_AT_APIS)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_POSIX_AT_APIS)
endif()
if(BOOST_FILESYSTEM_HAS_FALLOCATE)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_FALLOCATE)
endif()

target_link_libraries(boost_filesystem
    PUBLIC
        Boost::assert
        Boost::config
        Boost::container_hash
        Boost::detail
        Boost::io
        Boost::iterator
        Boost::smart_ptr
        Boost::system
        Boost::type_traits

    PRIVATE
        Boost::core
        Boost::predef
        Boost::scope
)

if(NOT BOOST_FILESYSTEM_HAS_CXX20_ATOMIC_REF)
    target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_NO_CXX20_ATOMIC_REF)
    target_link_libraries(boost_filesystem
        PRIVATE
            Boost::atomic
    )
endif()

if(WIN32)
    if(BOOST_FILESYSTEM_HAS_BCRYPT)
        target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_BCRYPT)
        target_link_libraries(boost_filesystem PRIVATE bcrypt)
    else()
        target_compile_definitions(boost_filesystem PRIVATE BOOST_FILESYSTEM_HAS_WINCRYPT)
        if(NOT WINCE)
            target_link_libraries(boost_filesystem PRIVATE advapi32)
        else()
            target_link_libraries(boost_filesystem PRIVATE coredll)
        endif()
    endif()

    target_link_libraries(boost_filesystem
        PRIVATE
            Boost::winapi
    )
endif()
