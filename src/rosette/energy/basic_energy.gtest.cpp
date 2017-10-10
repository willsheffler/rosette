#include <gtest/gtest.h>

#include <rosette/energy/basic_energy.hpp>

namespace rosette {
namespace energy {
namespace basic_energy_gtest {

TEST(basic_energy, dummy) { ASSERT_EQ(dummy_func<double>(), 12345.0); }

TEST(basic_energy, basic_energy_safe_1) {
  int N = 1000;
  MatX4d atoms(N, 4);  // even though 2nd dim is static 4, need 2nd arg
  VecXi4 atypes(N);
  VecXi4 exatypes(N);
  MatX4i4 bond(N, 4);
  MatXd scores = basic_energy_safe<double>(atoms, atypes, exatypes, bond);
  ASSERT_EQ(scores.rows(), N + 0);
  ASSERT_EQ(scores.cols(), Etype::Ntypes);
}

TEST(basic_energy, basic_energy_safe_2) { ASSERT_TRUE(true); }
TEST(basic_energy, basic_energy_safe_3) { ASSERT_TRUE(true); }
TEST(basic_energy, basic_energy_safe_4) { ASSERT_TRUE(true); }
TEST(basic_energy, basic_energy_safe_5) { ASSERT_TRUE(true); }
}
}
}