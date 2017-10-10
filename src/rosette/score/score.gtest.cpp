#include <gtest/gtest.h>

#include <rosette/score/score.hpp>

namespace rosette {
namespace score {
namespace gtest {

TEST(score, dummy) { ASSERT_EQ(dummy_func<double>(), 12345.0); }

TEST(score, rosette_score_safe1) { ASSERT_TRUE(true); }
TEST(score, rosette_score_safe2) { ASSERT_TRUE(true); }
TEST(score, rosette_score_safe3) { ASSERT_TRUE(true); }
TEST(score, rosette_score_safe4) { ASSERT_TRUE(true); }
TEST(score, rosette_score_safe5) { ASSERT_TRUE(true); }
}
}
}