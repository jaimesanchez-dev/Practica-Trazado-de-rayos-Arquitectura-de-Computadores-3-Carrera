#include "mersenne_twister.hpp"
#include <random>

namespace render {

  mersenne_twister::mersenne_twister(unsigned int seed) : estado_mt(seed) { }

  double mersenne_twister::siguiente_numero() {
    std::uniform_real_distribution<double> rango(-0.5, 0.5);
    return rango(estado_mt);
  }

}  // namespace render
