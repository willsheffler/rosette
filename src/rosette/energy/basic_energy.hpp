#include <Eigen/Dense>
#include <iostream>
#include <rosette/types.hpp>

namespace rosette {
namespace energy {

namespace Etype {  // just score for enum... can't use enum class
enum Etype { fa_atr = 0, fa_rep, fa_sol, Ntypes };
}

template <typename F>
F dummy_func() {
  return 12345.0;
}

/// takes these Ref<Eigen::Matrix<...>> things... they map transparently wtih
/// python. if they are Ref<const Eigen::Matrix...> allegedly no copy
template <typename F>
MatX<F> basic_energy_safe(CRefMatX4<F> atoms, CRefVecXi4 atype,
                          CRefVecXi4 exatype, CRefMatX4i4 connect) {
  MatX<F> scores(atoms.rows(), (int)Etype::Ntypes);
  for (int iatom = 0; iatom < atoms.rows(); ++iatom) {
    // std::cout << iatom << " " << atoms.row(iatom) << ' ' << atype[iatom] << "
    // "
    // << exatype[iatom] << " " << connect.row(iatom) << std::endl;
    scores(iatom, Etype::fa_atr) = 0.0;
    scores(iatom, Etype::fa_rep) = 0.0;
    scores(iatom, Etype::fa_sol) = 0.0;
  }
  return scores;
}
}
}