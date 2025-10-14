#include <gtest/gtest.h>
#include <bernoulli/rate_span.hpp>

TEST(RateSpanTest, Construction) {
    // Default construction
    rate_span r1;
    EXPECT_EQ(r1.min, 0.0f);
    EXPECT_EQ(r1.max, 1.0f);
    
    // Single value construction
    rate_span r2(0.5f);
    EXPECT_EQ(r2.min, 0.5f);
    EXPECT_EQ(r2.max, 0.5f);
    
    // Range construction
    rate_span r3(0.2f, 0.8f);
    EXPECT_EQ(r3.min, 0.2f);
    EXPECT_EQ(r3.max, 0.8f);
    
    // Clamping to [0,1]
    rate_span r4(-0.5f, 1.5f);
    EXPECT_EQ(r4.min, 0.0f);
    EXPECT_EQ(r4.max, 1.0f);
}

TEST(RateSpanTest, Addition) {
    rate_span r1(0.1f, 0.3f);
    rate_span r2(0.2f, 0.4f);
    
    auto r3 = r1 + r2;
    EXPECT_FLOAT_EQ(r3.min, 0.3f);
    EXPECT_FLOAT_EQ(r3.max, 0.7f);
}

TEST(RateSpanTest, Subtraction) {
    rate_span r1(0.5f, 0.8f);
    rate_span r2(0.1f, 0.3f);
    
    auto r3 = r1 - r2;
    EXPECT_FLOAT_EQ(r3.min, 0.2f);  // 0.5 - 0.3
    EXPECT_FLOAT_EQ(r3.max, 0.7f);  // 0.8 - 0.1
}

TEST(RateSpanTest, Multiplication) {
    rate_span r1(0.2f, 0.5f);
    rate_span r2(0.4f, 0.6f);
    
    auto r3 = r1 * r2;
    // All products: 0.08, 0.12, 0.20, 0.30
    EXPECT_FLOAT_EQ(r3.min, 0.08f);
    EXPECT_FLOAT_EQ(r3.max, 0.30f);
}

TEST(RateSpanTest, Intersection) {
    rate_span r1(0.2f, 0.7f);
    rate_span r2(0.4f, 0.9f);
    
    auto r3 = r1 & r2;
    EXPECT_FLOAT_EQ(r3.min, 0.4f);  // max of mins
    EXPECT_FLOAT_EQ(r3.max, 0.7f);  // min of maxs
}

TEST(RateSpanTest, Union) {
    rate_span r1(0.2f, 0.7f);
    rate_span r2(0.4f, 0.9f);
    
    auto r3 = r1 | r2;
    EXPECT_FLOAT_EQ(r3.min, 0.2f);  // min of mins
    EXPECT_FLOAT_EQ(r3.max, 0.9f);  // max of maxs
}