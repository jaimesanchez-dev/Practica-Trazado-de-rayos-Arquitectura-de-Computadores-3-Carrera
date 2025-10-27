#include "cilindro.hpp"
#include "configuracion.hpp"
#include "render-aos.hpp"

#include "escena.hpp"
#include "lector_archivo_escena.hpp"
#include <iostream>
#include <vector>

using namespace render;

int main(int argc, char * argv[]) {
  std::vector<std::string_view> const args(argv, argv + argc);
  if (argc != 4) {
    std::cerr << "Uso: " << args[0] << " <config> <escena> <salida>\n";
    return 1;
  }

  try {
    // Leer configuración
    Configuracion config = leer_configuracion(std::string(args[1]));

    // Leer escena
    std::vector<::esfera> esferas;
    std::vector<::cilindro> cilindros;

    leer_escena(std::string(args[2]), esferas, cilindros);

    // Construir escena de renderizado
    escena escena_render;
    for (auto const & e : esferas) {
      escena_render.agregar_esfera(render::esfera(e.centro, e.radio));
    }
    for (auto const & c : cilindros) {
      escena_render.agregar_cilindro(render::cilindro(c.centro, c.eje, c.radio));
    }

    // Renderizar
    int ancho = config.image_width;
    int alto  = config.image_width * config.aspect_height / config.aspect_width;

    RenderAOS renderizador(ancho, alto);
    renderizador.renderizar(escena_render);
    renderizador.guardarPPM(std::string(args[3]));

  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }

  return 0;
}
