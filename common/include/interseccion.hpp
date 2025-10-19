#ifndef RENDER_INTERSECCION_HPP
#define RENDER_INTERSECCION_HPP

#include "vector.hpp"

namespace render {

  class interseccion {
  private:
    bool existe   = false;                  // indica si hay o no colisión
    double t      = 0.0;                    // distancia desde el origen del rayo
    vector punto  = vector(0.0, 0.0, 0.0);  // coordenadas del punto de impacto
    vector normal = vector(0.0, 0.0, 0.0);  // vector normal a la superficie en ese punto

  public:
    interseccion() = default;

    [[nodiscard]] bool obtener_existe() const { return existe; }

    [[nodiscard]] double obtener_t() const { return t; }

    [[nodiscard]] vector obtener_punto() const { return punto; }

    [[nodiscard]] vector obtener_normal() const { return normal; }
  };

}  // namespace render

#endif
