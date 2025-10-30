#include "imagen_aos.hpp"
#include "color.hpp"
#include <cstddef>
#include <fstream>
#include <stdexcept>
#include <string>

namespace render {

  imagen_aos::imagen_aos(int w, int h) : ancho{w}, alto{h} {
    if (w <= 0 or h <= 0) {
      throw std::invalid_argument("Dimensiones de imagen invÃ¡lidas");
    }
    pixeles.resize(static_cast<size_t>(w) * static_cast<size_t>(h));
  }

  void imagen_aos::establecer_pixel(int x, int y, color const & c) {
    if (x < 0 or x >= ancho or y < 0 or y >= alto) {
      return;
    }
    int const indice                     = y * ancho + x;
    pixeles[static_cast<size_t>(indice)] = pixel_aos(c);
  }

  void imagen_aos::guardar_ppm(std::string const & archivo) const {
    std::ofstream salida(archivo);
    if (!salida.is_open()) {
      throw std::runtime_error("No se pudo crear el archivo: " + archivo);
    }

    salida << "P3\n";
    salida << ancho << " " << alto << "\n";
    salida << "255\n";

    for (auto const & pixel : pixeles) {
      salida << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
             << static_cast<int>(pixel.b) << "\n";
    }
  }

}  // namespace render
