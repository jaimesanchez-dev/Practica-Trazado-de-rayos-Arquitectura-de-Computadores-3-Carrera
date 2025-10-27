#ifndef RENDER_AOS_HPP
#define RENDER_AOS_HPP

#include "escena.hpp"
#include <cstdint>
#include <string>
#include <vector>

namespace render {

  struct Pixel {
    uint8_t r, g, b;

    Pixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) : r(r), g(g), b(b) { }
  };

  class RenderAOS {
  private:
    std::vector<Pixel> pixeles;
    int ancho, alto;

  public:
    // ✅ DECLARACIÓN del constructor
    RenderAOS(int ancho, int alto);

    // ✅ DECLARACIÓN de los métodos
    void renderizar(escena const & escena);
    void guardarPPM(std::string const & nombre_archivo) const;

    [[nodiscard]] int obtener_ancho() const { return ancho; }

    [[nodiscard]] int obtener_alto() const { return alto; }
  };

}  // namespace render

#endif
