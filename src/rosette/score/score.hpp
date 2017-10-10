#include <Eigen/Dense>
#include <iostream>
#include <rosette/types.hpp>

namespace rosette {
namespace score {

namespace Etype {  // just score for enum... can't use enum class
enum Etype { fa_atr = 0, fa_rep, fa_sol, Ntypes };
}

template <typename F>
F dummy_func() {
  return 12345.0;
}

template <typename F>
MatX<F> rosette_score_safe(Ref<MatX4<F>> atoms, Ref<VecX<int32_t>> atype,
                           Ref<VecX<int32_t>> exatype,
                           Ref<MatX4<int32_t>> connect) {
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