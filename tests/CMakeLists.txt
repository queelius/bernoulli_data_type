# Find or fetch Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Enable testing
enable_testing()

# Test executable
add_executable(bernoulli_tests
    test_main.cpp
    test_rate_span.cpp
    test_observed_bool.cpp
    test_observed_set.cpp
    test_observed_map.cpp
    test_bloom_filter.cpp
)

# Link against Google Test and our library
target_link_libraries(bernoulli_tests
    PRIVATE
        Bernoulli::Types
        gtest
        gtest_main
)

# Add include directories for tests
target_include_directories(bernoulli_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

# Register tests with CTest
include(GoogleTest)
gtest_discover_tests(bernoulli_tests)

# Add a custom target to run tests
add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS bernoulli_tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running unit tests"
)