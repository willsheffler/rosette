#include <gtest/gtest.h>

#include <rosette/score/score.hpp>

namespace rosette {
namespace score {
namespace gtest {

TEST(score, dummy) { ASSERT_EQ(dummy_func<double>(), 12345.0); }

TEST(score, rosette_score_safe) { ASSERT_TRUE(true); }
}
}
}