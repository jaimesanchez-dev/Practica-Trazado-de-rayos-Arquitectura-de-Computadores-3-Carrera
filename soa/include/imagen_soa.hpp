#ifndef RENDER_IMAGEN_SOA_HPP
#define RENDER_IMAGEN_SOA_HPP

#include "color.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace render {

  class imagen_soa {
  private:
    std::vector<uint8_t> canal_r;
    std::vector<uint8_t> canal_g;
    std::vector<uint8_t> canal_b;
    int ancho;
    int alto;

  public:
    imagen_soa(int w, int h);

    // Regla de los cinco
    imagen_soa(imagen_soa const &)             = default;
    imagen_soa & operator=(imagen_soa const &) = default;
    imagen_soa(imagen_soa &&)                  = default;
    imagen_soa & operator=(imagen_soa &&)      = default;
    ~imagen_soa()                              = default;

    void establecer_pixel(int x, int y, color const & c);

    void guardar_ppm(std::string const & archivo) const;

    [[nodiscard]] int obtener_ancho() const { return ancho; }

    [[nodiscard]] int obtener_alto() const { return alto; }
  };

}  // namespace render

#endif
