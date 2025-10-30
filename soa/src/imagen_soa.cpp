
#include "imagen_soa.hpp"
#include <fstream>
#include <stdexcept>

namespace render {

  imagen_soa::imagen_soa(int w, int h) : ancho{w}, alto{h} {
    if (w <= 0 || h <= 0) {
      throw std::invalid_argument("Dimensiones de imagen invÃ¡lidas");
    }
    size_t tam = static_cast<size_t>(w * h);
    canal_r.resize(tam, 0);
    canal_g.resize(tam, 0);
    canal_b.resize(tam, 0);
  }

  void imagen_soa::establecer_pixel(int x, int y, color const & c) {
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
      return;
    }
    size_t indice   = static_cast<size_t>(y * ancho + x);
    canal_r[indice] = c.mapear_r();
    canal_g[indice] = c.mapear_g();
    canal_b[indice] = c.mapear_b();
  }

  void imagen_soa::guardar_ppm(std::string const & archivo) const {
    std::ofstream salida(archivo);
    if (!salida.is_open()) {
      throw std::runtime_error("No se pudo crear el archivo: " + archivo);
    }

    salida << "P3\n";
    salida << ancho << " " << alto << "\n";
    salida << "255\n";

    size_t total = static_cast<size_t>(ancho * alto);
    for (size_t i = 0; i < total; ++i) {
      salida << static_cast<int>(canal_r[i]) << " " << static_cast<int>(canal_g[i]) << " "
             << static_cast<int>(canal_b[i]) << "\n";
    }
  }

}  // namespace render