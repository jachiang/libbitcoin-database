###############################################################################
#  Copyright (c) 2014-2018 libbitcoin-database developers (see COPYING).
#
#         GENERATED SOURCE CODE, DO NOT EDIT EXCEPT EXPERIMENTALLY
#
###############################################################################

# libbitcoin-database project configuration.
#------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(libbitcoin-database LANGUAGES C CXX)

enable_testing()

list( APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/modules" )
include(CheckIncludeFiles)
include(CheckSymbolExists)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
  set( CANONICAL_LIB_NAME libbitcoin-database )
else ()
  set( CANONICAL_LIB_NAME bitcoin-database )
  find_package( PkgConfig REQUIRED )

  set( prefix "${CMAKE_PREFIX_PATH}" )
  set( exec_prefix "\${prefix}" )
  set( libdir "\${exec_prefix}/lib" )
  set( includedir "\${exec_prefix}/include" )

  set( PACKAGE_VERSION "4.0.0" )
  set( VERSION "${PACKAGE_VERSION}" )
endif ()

set( CMAKE_CXX_STANDARD 11 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )

# Add compiler options
#------------------------------------------------------------------------------
# Warn on all stuff.
add_compile_options( "-Wall" )

# Warn on extra stuff.
add_compile_options( "-Wextra" )

# Be really annoying.
add_compile_options( "-Wpedantic" )

# Disallow warning on style order of declarations.
add_compile_options( "-Wno-reorder" )

# Suppress warning for incomplete field initialization.
add_compile_options( "-Wno-missing-field-initializers" )

# Conform to style.
add_compile_options( "-Wno-missing-braces" )

# Ignore comments within comments or commenting of backslash extended lines.
add_compile_options( "-Wno-comment" )

# Conflict in stdlib under clang.
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options( "-Wno-mismatched-tags" )
endif()

# Implement -Dpkgconfigdir and output ${pkgconfigdir}.
#------------------------------------------------------------------------------
set( pkgconfigdir "${libdir}/pkgconfig" CACHE PATH "Path to pkgconfig directory." )

# Implement -Dwith-tests and declare with-tests.
#------------------------------------------------------------------------------
set( with-tests "yes" CACHE BOOL "Compile with unit tests." )

# Implement -Dwith-tools and declare with-tools.
#------------------------------------------------------------------------------
set( with-tools "yes" CACHE BOOL "Compile with tools." )

# Implement -Denable-ndebug and define NDEBUG.
#------------------------------------------------------------------------------
set( enable-ndebug "yes" CACHE BOOL "Compile without debug assertions." )

if (enable-ndebug)
    add_definitions( -DNDEBUG )
endif()

# Inherit -Denable-shared and define BOOST_ALL_DYN_LINK.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    add_definitions( -DBOOST_ALL_DYN_LINK )
endif()

# Find boost
#------------------------------------------------------------------------------
find_package( Boost 1.62.0 REQUIRED COMPONENTS
    unit_test_framework )

set( boost_unit_test_framework_LIBS "-lboost_unit_test_framework" )

if (enable-ndebug)
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_DEBUG}" )
else ()
    set( Boost_LIBRARY_DIR "${Boost_LIBRARY_DIR_RELEASE}" )
endif()

set( boost_CPPFLAGS "-I${Boost_INCLUDE_DIR}" )
set( boost_LDFLAGS "-L${Boost_LIBRARY_DIR}" )

# Find bitcoin
#------------------------------------------------------------------------------
find_package( Bitcoin 4.0.0 REQUIRED )

# Define project common includes directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    include_directories( SYSTEM
        ${bitcoin_INCLUDE_DIRS} )
else()
    include_directories( SYSTEM
        ${bitcoin_STATIC_INCLUDE_DIRS} )
endif()

# Define project common library directories
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_directories( 
        ${bitcoin_LIBRARY_DIRS} )
else()
    link_directories( 
        ${bitcoin_STATIC_LIBRARY_DIRS} )
endif()

# Define project common libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    link_libraries( 
        "-fstack-protector"
        "-fstack-protector-all"
        ${bitcoin_LIBRARIES} )
else()
    link_libraries( 
        "-fstack-protector"
        "-fstack-protector-all"
        ${bitcoin_STATIC_LIBRARIES} )
endif()

# Define ${CANONICAL_LIB_NAME} project.
#------------------------------------------------------------------------------
add_library( ${CANONICAL_LIB_NAME}
    "../../src/data_base.cpp"
    "../../src/settings.cpp"
    "../../src/store.cpp"
    "../../src/unspent_outputs.cpp"
    "../../src/unspent_transaction.cpp"
    "../../src/verify.cpp"
    "../../src/databases/address_database.cpp"
    "../../src/databases/block_database.cpp"
    "../../src/databases/transaction_database.cpp"
    "../../src/memory/accessor.cpp"
    "../../src/memory/file_storage.cpp"
    "../../src/mman-win32/mman.c"
    "../../src/mman-win32/mman.h"
    "../../src/result/address_iterator.cpp"
    "../../src/result/address_result.cpp"
    "../../src/result/block_result.cpp"
    "../../src/result/inpoint_iterator.cpp"
    "../../src/result/transaction_iterator.cpp"
    "../../src/result/transaction_result.cpp" )

# ${CANONICAL_LIB_NAME} project specific include directories.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${bitcoin_INCLUDE_DIRS} )
else()
    target_include_directories( ${CANONICAL_LIB_NAME} PRIVATE
        "../../include"
        ${bitcoin_STATIC_INCLUDE_DIRS} )
endif()

target_include_directories( ${CANONICAL_LIB_NAME} PUBLIC
"../../include" )

# ${CANONICAL_LIB_NAME} project specific libraries/linker flags.
#------------------------------------------------------------------------------
if (BUILD_SHARED_LIBS)
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${bitcoin_LIBRARIES} )
else()
    target_link_libraries( ${CANONICAL_LIB_NAME}
        ${bitcoin_STATIC_LIBRARIES} )
endif()

# Define libbitcoin-database-test project.
#------------------------------------------------------------------------------
if (with-tests)
    add_executable( libbitcoin-database-test
        "../../test/block_state.cpp"
        "../../test/data_base.cpp"
        "../../test/main.cpp"
        "../../test/settings.cpp"
        "../../test/store.cpp"
        "../../test/unspent_outputs.cpp"
        "../../test/unspent_transaction.cpp"
        "../../test/databases/address_database.cpp"
        "../../test/databases/block_database.cpp"
        "../../test/databases/transaction_database.cpp"
        "../../test/memory/accessor.cpp"
        "../../test/memory/file_storage.cpp"
        "../../test/primitives/hash_table.cpp"
        "../../test/primitives/hash_table_header.cpp"
        "../../test/primitives/hash_table_multimap.cpp"
        "../../test/primitives/list.cpp"
        "../../test/primitives/list_element.cpp"
        "../../test/primitives/list_iterator.cpp"
        "../../test/primitives/record_manager.cpp"
        "../../test/primitives/slab_manager.cpp"
        "../../test/result/address_iterator.cpp"
        "../../test/result/address_result.cpp"
        "../../test/result/block_result.cpp"
        "../../test/result/transaction_iterator.cpp"
        "../../test/result/transaction_result.cpp"
        "../../test/utility/storage.cpp"
        "../../test/utility/storage.hpp"
        "../../test/utility/utility.cpp"
        "../../test/utility/utility.hpp" )

    add_test( NAME libbitcoin-database-test COMMAND libbitcoin-database-test
            --run_test=*
            --show_progress=no
            --detect_memory_leak=0
            --report_level=no
            --build_info=yes )

#     libbitcoin-database-test project specific include directories.
#------------------------------------------------------------------------------
    target_include_directories( libbitcoin-database-test PRIVATE
        "../../include" )

#     libbitcoin-database-test project specific libraries/linker flags.
#------------------------------------------------------------------------------
    target_link_libraries( libbitcoin-database-test
        ${CANONICAL_LIB_NAME}
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} )

endif()

# Define initchain project.
#------------------------------------------------------------------------------
if (with-tools)
    add_executable( initchain
        "../../tools/initchain/initchain.cpp" )

#     initchain project specific include directories.
#------------------------------------------------------------------------------
    target_include_directories( initchain PRIVATE
        "../../include" )

#     initchain project specific libraries/linker flags.
#------------------------------------------------------------------------------
    target_link_libraries( initchain
        ${CANONICAL_LIB_NAME} )

endif()

# Manage pkgconfig installation.
#------------------------------------------------------------------------------
configure_file(
  "../../libbitcoin-database.pc.in"
  "libbitcoin-database.pc" @ONLY )

install( FILES
    "${CMAKE_CURRENT_BINARY_DIR}/libbitcoin-database.pc"
    DESTINATION "${pkgconfigdir}" )

# Manage installation of docs.
#------------------------------------------------------------------------------
install( FILES
    "../../AUTHORS"
    "../../COPYING"
    "../../ChangeLog"
    "../../INSTALL"
    "../../NEWS"
    "../../README"
    DESTINATION share/doc/libbitcoin-database )

# Manage ${CANONICAL_LIB_NAME} installation.
#------------------------------------------------------------------------------
install( TARGETS ${CANONICAL_LIB_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include )

# Manage include installation.
#------------------------------------------------------------------------------
install( DIRECTORY "../../include/bitcoin"
    DESTINATION include )

