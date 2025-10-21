#ifndef RENDER_AOS_HPP
#define RENDER_AOS_HPP

#include render-2025-a-m81-01/common/include/vector.hpp
#include render-2025-a-m81-01/common/include/rayo.hpp
#include render-2025-a-m81-01/common/include/esfera.hpp
#include render-2025-a-m81-01/common/include/cilindro.hpp
#include "vector.hpp"
#include "rayo.hpp"
#include "esfera.hpp"
#include "cilindro.hpp"
#include "material.hpp"
#include "escena.hpp"
#include <vector>
#include <cstdint>
#include <string>

// píxeles de la imagen como un único array
//  donde cada elemento es una estructura de tres RGB

    struct Pixel {
      uint8_t r, g, b; //0-255 para cada componente

      //Pixel() : r(0), g(0), b(0) {}  // Constructor por defecto (negro)

      Pixel(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) 
        : r(r), g(g), b(b) {}
    };  // Un solo vector



  class ImageAOS {
  
    private:

    std::vector<Pixel> pixeles;
      int ancho, alto;

  public:

    // Constructor
    RenderAOS(int ancho, int alto);

    
    // Método principal que genera la imagen
    void renderizar(const esfera& esfera);

    
    // Guardar la imagen en formato PPM
    void guardarPPM(const std::string& nombre_archivo) const;


    [[nodiscard]] int obtener_ancho() const { return ancho; }
    [[nodiscard]] int obtener_alto() const { return alto; }
  };

 // namespace render?

#endif  // RENDER_IMAGE_AOS_HPP  ← ¡IMPORTANTE!
