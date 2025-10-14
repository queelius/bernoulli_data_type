cmake_minimum_required(VERSION 3.14)
project(BernoulliTypes 
    VERSION 0.1.0
    DESCRIPTION "A C++ library for probabilistic data structures using the Bernoulli framework"
    LANGUAGES CXX)

# Options
option(BERNOULLI_BUILD_TESTS "Build unit tests" ON)
option(BERNOULLI_BUILD_EXAMPLES "Build examples" ON)
option(BERNOULLI_BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BERNOULLI_INSTALL "Generate installation target" ON)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include modules
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Interface library (header-only)
add_library(bernoulli_types INTERFACE)
add_library(Bernoulli::Types ALIAS bernoulli_types)

# Specify include directories
target_include_directories(bernoulli_types
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Compile features
target_compile_features(bernoulli_types
    INTERFACE
        cxx_std_17
)

# Find dependencies (if any)
# find_package(Threads REQUIRED)
# target_link_libraries(bernoulli_types INTERFACE Threads::Threads)

# Tests
if(BERNOULLI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(BERNOULLI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Benchmarks
if(BERNOULLI_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks)
endif()

# Installation
if(BERNOULLI_INSTALL)
    # Install headers
    install(
        DIRECTORY include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    # Install targets
    install(
        TARGETS bernoulli_types
        EXPORT BernoulliTypesTargets
    )

    # Install export set
    install(
        EXPORT BernoulliTypesTargets
        FILE BernoulliTypesTargets.cmake
        NAMESPACE Bernoulli::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BernoulliTypes
    )

    # Create config file
    configure_package_config_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/BernoulliTypesConfig.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/BernoulliTypesConfig.cmake
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BernoulliTypes
    )

    # Create version file
    write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/BernoulliTypesConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    # Install config files
    install(
        FILES
            ${CMAKE_CURRENT_BINARY_DIR}/BernoulliTypesConfig.cmake
            ${CMAKE_CURRENT_BINARY_DIR}/BernoulliTypesConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BernoulliTypes
    )
endif()

# Print configuration summary
message(STATUS "")
message(STATUS "BernoulliTypes configuration summary:")
message(STATUS "  Version:          ${PROJECT_VERSION}")
message(STATUS "  Build tests:      ${BERNOULLI_BUILD_TESTS}")
message(STATUS "  Build examples:   ${BERNOULLI_BUILD_EXAMPLES}")
message(STATUS "  Build benchmarks: ${BERNOULLI_BUILD_BENCHMARKS}")
message(STATUS "  Install:          ${BERNOULLI_INSTALL}")
message(STATUS "  C++ standard:     ${CMAKE_CXX_STANDARD}")
message(STATUS "")