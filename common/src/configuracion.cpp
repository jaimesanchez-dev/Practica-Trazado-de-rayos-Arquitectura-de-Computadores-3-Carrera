#include "configuracion.hpp"
#include "vector.hpp"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

  struct contexto_parseo {
    std::string linea;
    int num_linea = 0;
  };

  void trim(std::string & s) {
    // Elimina espacios en blanco al inicio y al final
    size_t const start = s.find_first_not_of(" \t\r\n");
    size_t const end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
      s.clear();
    } else {
      s = s.substr(start, end - start + 1);
    }
  }

  void parsear_aspect_ratio(Configuracion & cfg, std::istringstream & iss,
                            contexto_parseo const & ctx) {
    int w = 0, h = 0;
    std::string mensaje;
    std::string extra;

    if (!(iss >> w >> h)) {
      mensaje = "Error: Invalid value for key: [aspect_ratio:]";
    } else if (w <= 0 and h <= 0) {
      mensaje = "Error: Non-positive value for key: [aspect_ratio:]";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [aspect_ratio:]";
    }

    if (!mensaje.empty()) {
      std::string msg = mensaje;
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine " + std::to_string(ctx.num_linea) + ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
    cfg.aspect_width  = w;
    cfg.aspect_height = h;
  }

  void parsear_image_width(Configuracion & cfg, std::istringstream & iss,
                           contexto_parseo const & ctx) {
    int val = 0;
    std::string mensaje;
    std::string extra;

    if (!(iss >> val)) {
      mensaje = "Error: Invalid value for key: [image_width:]";
    } else if (val <= 0) {
      mensaje = "Error: Non-positive value for key: [image_width:]";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [image_width:]";
    }
    if (!mensaje.empty()) {
      std::string msg = mensaje;
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine " + std::to_string(ctx.num_linea) + ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
    cfg.image_width = val;
  }

  void parsear_gamma(Configuracion & cfg, std::istringstream & iss, contexto_parseo const & ctx) {
    double g = 0.0;
    std::string mensaje;
    std::string extra;

    if (!(iss >> g)) {
      mensaje = "Error: Invalid value for key: [gamma:]";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [gamma:]";
    }
    if (!mensaje.empty()) {
      std::string msg = mensaje;
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine " + std::to_string(ctx.num_linea) + ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
    cfg.gamma = g;
  }

  void parsear_vector(render::vector & vec, std::istringstream & iss, std::string const & etiqueta,
                      contexto_parseo const & ctx) {
    double x = 0.0, y = 0.0, z = 0.0;
    std::string mensaje;
    std::string extra;

    if (!(iss >> x >> y >> z)) {
      mensaje = "Error: Invalid value for key: [" + etiqueta + "]";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [" + etiqueta + "]";
    }
    if (!mensaje.empty()) {
      std::string msg = mensaje;
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine " + std::to_string(ctx.num_linea) + ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
    vec = {x, y, z};
  }

  void parsear_field_of_view(Configuracion & cfg, std::istringstream & iss,
                             contexto_parseo const & ctx) {
    double fov = 0.0;
    std::string mensaje;
    std::string extra;

    if (!(iss >> fov)) {
      mensaje = "Error: Invalid value for key: [field_of_view:]";
    } else if (fov <= 0.0 and fov >= 180.0) {
      mensaje = "Error: Value out of range for key: [field_of_view:]";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [field_of_view:]";
    }
    if (!mensaje.empty()) {
      std::string msg = mensaje;
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine " + std::to_string(ctx.num_linea) + ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
    cfg.field_of_view = fov;
  }

  void parsear_entero(int & val, std::istringstream & iss, std::string const & etiqueta,
                      contexto_parseo const & ctx) {
    std::string mensaje;
    std::string extra;

    if (!(iss >> val)) {
      mensaje = "Error: Invalid value for key: [";
    } else if (val <= 0) {
      mensaje = "Error: Invalid non-positive value for key: [";
    } else if (iss >> extra) {
      mensaje = "Error: Extra data after configuration value for key: [";
    }
    if (!mensaje.empty()) {
      std::string msg = mensaje;
      msg += etiqueta;
      msg += "]";
      if (!extra.empty()) {
        msg += "\nExtra: \"" + extra + "\"";
      }
      msg += "\nLine ";
      msg += std::to_string(ctx.num_linea);
      msg += ": \"" + ctx.linea + "\"";
      throw std::runtime_error(msg);
    }
  }

  void procesar_etiqueta(Configuracion & cfg, std::string const & etiqueta,
                         std::istringstream & iss, contexto_parseo const & ctx) {
    if (etiqueta == "aspect_ratio:") {
      parsear_aspect_ratio(cfg, iss, ctx);
    } else if (etiqueta == "image_width:") {
      parsear_image_width(cfg, iss, ctx);
    } else if (etiqueta == "gamma:") {
      parsear_gamma(cfg, iss, ctx);
    } else if (etiqueta == "camera_position:") {
      parsear_vector(cfg.camera_position, iss, etiqueta, ctx);
    } else if (etiqueta == "camera_target:") {
      parsear_vector(cfg.camera_target, iss, etiqueta, ctx);
    } else if (etiqueta == "camera_north:") {
      parsear_vector(cfg.camera_north, iss, etiqueta, ctx);
    } else if (etiqueta == "background_dark_color:") {
      parsear_vector(cfg.background_dark_color, iss, etiqueta, ctx);
    } else if (etiqueta == "background_light_color:") {
      parsear_vector(cfg.background_light_color, iss, etiqueta, ctx);
    } else if (etiqueta == "field_of_view:") {
      parsear_field_of_view(cfg, iss, ctx);
    } else if (etiqueta == "samples_per_pixel:") {
      parsear_entero(cfg.samples_per_pixel, iss, etiqueta, ctx);
    } else if (etiqueta == "max_depth:") {
      parsear_entero(cfg.max_depth, iss, etiqueta, ctx);
    } else if (etiqueta == "material_rng_seed:") {
      parsear_entero(cfg.material_rng_seed, iss, etiqueta, ctx);
    } else if (etiqueta == "ray_rng_seed:") {
      parsear_entero(cfg.ray_rng_seed, iss, etiqueta, ctx);
    } else {
      throw std::runtime_error("Error: Unknown configuration key: [" + etiqueta + "]");
    }
  }

}  // namespace

Configuracion leer_configuracion(std::string const & ruta) {
  Configuracion config;
  std::ifstream archivo(ruta);
  if (!archivo.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo de configuración: " + ruta);
  }
  /*leemos el archivo linea por linea*/
  std::string linea;
  int numero_linea = 0;
  while (std::getline(archivo, linea)) {
    numero_linea++;
    trim(linea);
    /*Ignorar líneas vacías*/
    if (linea.empty()) {
      continue;
    }
    /*Leemos la etiqueta de cada linea ej:gamma*/
    std::istringstream iss(linea);
    std::string etiqueta;
    iss >> etiqueta;
    /*Verificamos que la etiqueta sea válida*/
    if (etiqueta.empty() or etiqueta.back() != ':') {
      throw std::runtime_error("Error: Unknown configuration key: [" + etiqueta + "]");
    }

    contexto_parseo const ctx{linea, numero_linea}; /*he creado el struct para pasar parametros para
                                                 que no haya problemas con el clan-tidy*/

    procesar_etiqueta(config, etiqueta, iss, ctx);
  }

  return config;
}
