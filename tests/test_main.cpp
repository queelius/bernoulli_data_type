#include <gtest/gtest.h>

// Google Test main is provided by gtest_main library
// This file can contain global test environment setup if needed

// Example global test environment
class BernoulliTestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // Global setup code
    }
    
    void TearDown() override {
        // Global teardown code  
    }
};

// Register the environment (optional)
// testing::Environment* const env = testing::AddGlobalTestEnvironment(new BernoulliTestEnvironment);