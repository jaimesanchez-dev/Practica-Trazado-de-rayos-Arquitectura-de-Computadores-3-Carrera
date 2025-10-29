#ifndef RENDER_IMAGEN_AOS_HPP
#define RENDER_IMAGEN_AOS_HPP

#include "color.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace render {

  struct pixel_aos {
    uint8_t r, g, b;

    pixel_aos(uint8_t red = 0, uint8_t green = 0, uint8_t blue = 0) : r{red}, g{green}, b{blue} { }

    explicit pixel_aos(color const & c) : r{c.mapear_r()}, g{c.mapear_g()}, b{c.mapear_b()} { }
  };

  class imagen_aos {
  private:
    std::vector<pixel_aos> pixeles;
    int ancho, alto;

  public:
    imagen_aos(int w, int h);

    void establecer_pixel(int x, int y, color const & c);

    void guardar_ppm(std::string const & archivo) const;

    [[nodiscard]] int obtener_ancho() const { return ancho; }

    [[nodiscard]] int obtener_alto() const { return alto; }
  };

}  // namespace render

#endif
