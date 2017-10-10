#include <Eigen/Dense>

namespace rosette {

using Eigen::Ref;

template <typename T>
using VecX = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template <typename T>
using RefVecX = Eigen::Ref<VecX<T>>;

template <typename T>
using MatX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
template <typename T>
using RefMatX = Eigen::Ref<MatX<T>>;

template <typename T>
using MatX4 = Eigen::Matrix<T, Eigen::Dynamic, 4, Eigen::RowMajor>;
template <typename T>
using RefMatX4 = Eigen::Ref<MatX4<T>>;
}