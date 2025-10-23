#ifndef RENDER_MERSENNE_TWISTER_HPP
#define RENDER_MERSENNE_TWISTER_HPP

#include <random>

namespace render {

  class mersenne_twister {
  private:
    std::mt19937 estado_mt;

  public:
    // Constructor de clase con una semilla por defecto = 0
    mersenne_twister(unsigned int seed);
    // Funcion que retorna el siguiente numero aleatorio
    double siguiente_numero();
  };

}  // namespace render
#endif
