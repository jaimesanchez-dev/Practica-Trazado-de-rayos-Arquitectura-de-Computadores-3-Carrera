#include "imagen_aos.hpp"
#include "color.hpp"
#include "configuracion.hpp"
#include "geometria.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "tamaño_ventana.hpp"
#include "trazador_rayos.hpp"
#include "ventana.hpp"
#include <cstddef>
#include <exception>
#include <iostream>
#include <print>
#include <span>
#include <string>

namespace {

  void ejecutar_renderizado(std::string const & archivo_config, std::string const & archivo_escena,
                            std::string const & archivo_salida) {
    std::println("Starting AOS rendering");

    Configuracion const config = leer_configuracion(archivo_config);

    int const alto_imagen =
        static_cast<int>(static_cast<double>(config.image_width * config.aspect_height) /
                         static_cast<double>(config.aspect_width));

    render::imagen_aos imagen(config.image_width, alto_imagen);
    render::geometria const geo(config.camera_position, config.camera_target, config.camera_north,
                                config.field_of_view);

    render::tamaño_imagen const tam(config.image_width, alto_imagen);
    render::mersenne_twister const mt_rayos(static_cast<unsigned>(config.ray_rng_seed));
    render::mersenne_twister mt_materiales(static_cast<unsigned>(config.material_rng_seed));
    render::ventana vent(geo, tam, mt_rayos);
    render::trazador_rayos trazador(config, mt_materiales);
    trazador.cargar_escena(archivo_escena);

    for (int y = 0; y < alto_imagen; ++y) {
      for (int x = 0; x < config.image_width; ++x) {
        render::color pixel_color(0.0, 0.0, 0.0);

        for (int s = 0; s < config.samples_per_pixel; ++s) {
          render::rayo const r  = vent.generar_rayos_pixel(x, y);
          render::color const c = trazador.trazar_rayo(r, config.max_depth);
          pixel_color           = pixel_color.suma(c);
        }

        double const inv_samples = 1.0 / static_cast<double>(config.samples_per_pixel);
        pixel_color              = pixel_color.escalar(inv_samples);
        pixel_color              = pixel_color.aplicar_gamma(config.gamma);

        imagen.establecer_pixel(x, y, pixel_color);
      }
    }

    imagen.guardar_ppm(archivo_salida);
    std::println("Image generated: {}", archivo_salida);
  }

}  // namespace

int main(int argc, char * argv[]) {
  try {
    std::span<char *> const args(argv, static_cast<size_t>(argc));

    if (argc != 4) {
      std::println("Error: Invalid number of arguments: {}", argc - 1);
      return 1;
    }

    std::string const archivo_config = args[1];
    std::string const archivo_escena = args[2];
    std::string const archivo_salida = args[3];

    ejecutar_renderizado(archivo_config, archivo_escena, archivo_salida);

    return 0;

  } catch (std::exception const & e) {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  } catch (...) {
    std::cerr << "Error desconocido durante la ejecución\n";
    return 1;
  }
}
