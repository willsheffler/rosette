#include <Eigen/Dense>

namespace rosette {

using Eigen::Ref;

template <typename T>
using VecX = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template <typename T>
using RefVecX = Eigen::Ref<VecX<T>>;
template <typename T>
using CRefVecX = Eigen::Ref<VecX<T> const>;

template <typename T>
using MatX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
template <typename T>
using RefMatX = Eigen::Ref<MatX<T>>;
template <typename T>
using CRefMatX = Eigen::Ref<MatX<T> const>;

template <typename T>
using MatX4 = Eigen::Matrix<T, Eigen::Dynamic, 4, Eigen::RowMajor>;
template <typename T>
using RefMatX4 = Eigen::Ref<MatX4<T>>;
template <typename T>
using CRefMatX4 = Eigen::Ref<MatX4<T> const>;

using VecXi1 = VecX<int8_t>;
using VecXi2 = VecX<int16_t>;
using VecXi4 = VecX<int32_t>;
using VecXi8 = VecX<int64_t>;
// using VecXi = VecX<int>;
using VecXu1 = VecX<uint8_t>;
using VecXu2 = VecX<uint16_t>;
using VecXu4 = VecX<uint32_t>;
using VecXu8 = VecX<uint64_t>;
// using VecXu = VecX<uint>;
using VecXf = VecX<float>;
using VecXd = VecX<double>;

using RefVecXi1 = RefVecX<int8_t>;
using RefVecXi2 = RefVecX<int16_t>;
using RefVecXi4 = RefVecX<int32_t>;
using RefVecXi8 = RefVecX<int64_t>;
// using RefVecXi = RefVecX<int>;
using RefVecXu1 = RefVecX<uint8_t>;
using RefVecXu2 = RefVecX<uint16_t>;
using RefVecXu4 = RefVecX<uint32_t>;
using RefVecXu8 = RefVecX<uint64_t>;
// using RefVecXu = RefVecX<uint>;
using RefVecXf = RefVecX<float>;
using RefVecXd = RefVecX<double>;

using CRefVecXi1 = CRefVecX<int8_t>;
using CRefVecXi2 = CRefVecX<int16_t>;
using CRefVecXi4 = CRefVecX<int32_t>;
using CRefVecXi8 = CRefVecX<int64_t>;
// using CRefVecXi = CRefVecX<int>;
using CRefVecXu1 = CRefVecX<uint8_t>;
using CRefVecXu2 = CRefVecX<uint16_t>;
using CRefVecXu4 = CRefVecX<uint32_t>;
using CRefVecXu8 = CRefVecX<uint64_t>;
// using CRefVecXu = CRefVecX<uint>;
using CRefVecXf = CRefVecX<float>;
using CRefVecXd = CRefVecX<double>;

using MatXi1 = MatX<int8_t>;
using MatXi2 = MatX<int16_t>;
using MatXi4 = MatX<int32_t>;
using MatXi8 = MatX<int64_t>;
// using MatXi = MatX<int>;
using MatXu1 = MatX<uint8_t>;
using MatXu2 = MatX<uint16_t>;
using MatXu4 = MatX<uint32_t>;
using MatXu8 = MatX<uint64_t>;
// using MatXu = MatX<uint>;
using MatXf = MatX<float>;
using MatXd = MatX<double>;

using RefMatXi1 = RefMatX<int8_t>;
using RefMatXi2 = RefMatX<int16_t>;
using RefMatXi4 = RefMatX<int32_t>;
using RefMatXi8 = RefMatX<int64_t>;
// using RefMatXi = RefMatX<int>;
using RefMatXu1 = RefMatX<uint8_t>;
using RefMatXu2 = RefMatX<uint16_t>;
using RefMatXu4 = RefMatX<uint32_t>;
using RefMatXu8 = RefMatX<uint64_t>;
// using RefMatXu = RefMatX<uint>;
using RefMatXf = RefMatX<float>;
using RefMatXd = RefMatX<double>;

using CRefMatXi1 = CRefMatX<int8_t>;
using CRefMatXi2 = CRefMatX<int16_t>;
using CRefMatXi4 = CRefMatX<int32_t>;
using CRefMatXi8 = CRefMatX<int64_t>;
// using CRefMatXi = CRefMatX<int>;
using CRefMatXu1 = CRefMatX<uint8_t>;
using CRefMatXu2 = CRefMatX<uint16_t>;
using CRefMatXu4 = CRefMatX<uint32_t>;
using CRefMatXu8 = CRefMatX<uint64_t>;
// using CRefMatXu = CRefMatX<uint>;
using CRefMatXf = CRefMatX<float>;
using CRefMatXd = CRefMatX<double>;

using MatX4i1 = MatX4<int8_t>;
using MatX4i2 = MatX4<int16_t>;
using MatX4i4 = MatX4<int32_t>;
using MatX4i8 = MatX4<int64_t>;
// using MatX4i = MatX4<int>;
using MatX4u1 = MatX4<uint8_t>;
using MatX4u2 = MatX4<uint16_t>;
using MatX4u4 = MatX4<uint32_t>;
using MatX4u8 = MatX4<uint64_t>;
// using MatX4u = MatX4<uint>;
using MatX4f = MatX4<float>;
using MatX4d = MatX4<double>;

using RefMatX4i1 = RefMatX4<int8_t>;
using RefMatX4i2 = RefMatX4<int16_t>;
using RefMatX4i4 = RefMatX4<int32_t>;
using RefMatX4i8 = RefMatX4<int64_t>;
// using RefMatX4i = RefMatX4<int>;
using RefMatX4u1 = RefMatX4<uint8_t>;
using RefMatX4u2 = RefMatX4<uint16_t>;
using RefMatX4u4 = RefMatX4<uint32_t>;
using RefMatX4u8 = RefMatX4<uint64_t>;
// using RefMatX4u = RefMatX4<uint>;
using RefMatX4f = RefMatX4<float>;
using RefMatX4d = RefMatX4<double>;

using CRefMatX4i1 = CRefMatX4<int8_t>;
using CRefMatX4i2 = CRefMatX4<int16_t>;
using CRefMatX4i4 = CRefMatX4<int32_t>;
using CRefMatX4i8 = CRefMatX4<int64_t>;
// using CRefMatX4i = CRefMatX4<int>;
using CRefMatX4u1 = CRefMatX4<uint8_t>;
using CRefMatX4u2 = CRefMatX4<uint16_t>;
using CRefMatX4u4 = CRefMatX4<uint32_t>;
using CRefMatX4u8 = CRefMatX4<uint64_t>;
// using CRefMatX4u = CRefMatX4<uint>;
using CRefMatX4f = CRefMatX4<float>;
using CRefMatX4d = CRefMatX4<double>;
}