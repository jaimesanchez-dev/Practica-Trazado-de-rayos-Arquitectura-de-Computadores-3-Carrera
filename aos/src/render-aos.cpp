#include "render-aos.hpp"
#include <fstream>
#include <iostream>
#include <ostream>
#include <vector>

namespace render {

  RenderAOS::RenderAOS(int ancho, int alto) : ancho(ancho), alto(alto) {
    int new_size = (ancho * alto);
    pixeles.resize(unsigned(new_size), Pixel(0, 0, 0));
  }

  void RenderAOS::renderizar(escena const & escena) {
    // Posición simple de cámara - deberías usar tu clase ventana
    vector posicion_camara(0.0, 0.0, -5.0);

    for (int y = 0; y < alto; ++y) {
      for (int x = 0; x < ancho; ++x) {
        // Coordenadas normalizadas (-1 a 1)
        double u = (2.0 * (x + 0.5) / ancho - 1.0);
        double v = (1.0 - 2.0 * (y + 0.5) / alto);

        // Ajustar aspect ratio
        u *= static_cast<double>(ancho) / alto;

        vector direccion(u, v, 1.0);
        direccion = direccion.normalizar();

        rayo rayo_actual(posicion_camara, direccion);
        int indice = y * ancho + x;

        // Calcular intersección
        interseccion inter = escena.interseccion_mas_cercana(rayo_actual);

        if (inter.obtener_existe()) {
          // Color simple basado en la normal
          vector normal = inter.obtener_normal();
          auto r        = static_cast<uint8_t>((normal.getX() + 1.0) * 0.5 * 255);
          auto g        = static_cast<uint8_t>((normal.getY() + 1.0) * 0.5 * 255);
          auto b        = static_cast<uint8_t>((normal.getZ() + 1.0) * 0.5 * 255);

          pixeles[unsigned(indice)] = Pixel(r, g, b);
        } else {
          // Fondo azul claro
          pixeles[unsigned(indice)] = Pixel(135, 206, 235);
        }
      }
    }
  }

  void RenderAOS::guardarPPM(std::string const & nombre_archivo) const {
    std::ofstream archivo(nombre_archivo);

    if (!archivo) {
      std::cerr << "No se pudo crear " << nombre_archivo << '\n';
      return;
    }

    // Escribir cabecera PPM
    archivo << "P3\n" << ancho << " " << alto << "\n255\n";

    // Escribir píxeles
    for (int y = 0; y < alto; ++y) {
      for (int x = 0; x < ancho; ++x) {
        Pixel const & pixel = pixeles[unsigned(y * ancho + x)];
        archivo << static_cast<int>(pixel.r) << " " << static_cast<int>(pixel.g) << " "
                << static_cast<int>(pixel.b) << "\n";
      }
    }

    archivo.close();
  }

}  // namespace render
