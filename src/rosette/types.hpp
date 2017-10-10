#include <Eigen/Dense>

namespace rosette {
using Eigen::Ref;
template <typename T>
using MatX = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
template <typename T>
using VecX = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template <typename T>
using MatX4 = Eigen::Matrix<T, Eigen::Dynamic, 4, Eigen::RowMajor>;
}