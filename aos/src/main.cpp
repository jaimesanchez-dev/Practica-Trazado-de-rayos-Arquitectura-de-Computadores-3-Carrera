#include "color.hpp"
#include "configuracion.hpp"
#include "geometria.hpp"
#include "imagen_aos.hpp"
#include "mersenne_twister.hpp"
#include "tamaño_ventana.hpp"
#include "trazador_rayos.hpp"
#include "ventana.hpp"
#include <iostream>
#include <print>

int main(int argc, char * argv[]) {
  if (argc != 4) {
    std::println("Error: Invalid number of arguments: {}", argc - 1);
    return 1;
  }

  std::string const archivo_config = argv[1];
  std::string const archivo_escena = argv[2];
  std::string const archivo_salida = argv[3];

  try {
    std::println("Starting AOS rendering");

    Configuracion config = leer_configuracion(archivo_config);

    int alto_imagen =
        static_cast<int>(static_cast<double>(config.image_width * config.aspect_height) /
                         static_cast<double>(config.aspect_width));

    render::imagen_aos imagen(config.image_width, alto_imagen);

    render::geometria geo(config.camera_position, config.camera_target, config.camera_north,
                          config.field_of_view);

    render::tamaño_imagen tam(config.image_width, alto_imagen);

    render::mersenne_twister mt_rayos(static_cast<unsigned>(config.ray_rng_seed));
    render::mersenne_twister mt_materiales(static_cast<unsigned>(config.material_rng_seed));

    render::ventana vent(geo, tam, mt_rayos);

    render::trazador_rayos trazador(config, mt_materiales);
    trazador.cargar_escena(archivo_escena);

    for (int y = 0; y < alto_imagen; ++y) {
      for (int x = 0; x < config.image_width; ++x) {
        render::color pixel_color(0.0, 0.0, 0.0);

        for (int s = 0; s < config.samples_per_pixel; ++s) {
          render::rayo r  = vent.generar_rayos_pixel(x, y);
          render::color c = trazador.trazar_rayo(r, config.max_depth);
          pixel_color     = pixel_color.suma(c);
        }

        double inv_samples = 1.0 / static_cast<double>(config.samples_per_pixel);
        pixel_color        = pixel_color.escalar(inv_samples);
        pixel_color        = pixel_color.aplicar_gamma(config.gamma);

        imagen.establecer_pixel(x, y, pixel_color);
      }
    }

    imagen.guardar_ppm(archivo_salida);
    std::println("Image generated: {}", archivo_salida);

  } catch (std::exception const & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
