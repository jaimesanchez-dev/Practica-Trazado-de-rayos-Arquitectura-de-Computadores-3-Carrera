#include "trazador_rayos.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  trazador_rayos::trazador_rayos(Configuracion const & cfg, mersenne_twister & mt)
      : config{cfg}, mt_materiales{mt} { }

  color trazador_rayos::calcular_color_fondo(vector const & direccion) const {
    vector dir_norm = direccion.normalizar();
    double t        = (dir_norm.getY() + 1.0) * 0.5;

    color claro(config.background_light_color);
    color oscuro(config.background_dark_color);

    return claro.escalar(1.0 - t).suma(oscuro.escalar(t));
  }

  void trazador_rayos::cargar_escena(std::string const & archivo_escena) {
    // Usar los tipos del lector_archivo_escena (sin namespace render)
    std::vector<::esfera> esferas_temp;
    std::vector<::cilindro> cilindros_temp;
    leer_escena(archivo_escena, esferas_temp, cilindros_temp);

    // Convertir de estructuras de lectura a clases render
    for (auto const & esf : esferas_temp) {
      render::esfera esfera_render(esf.centro, esf.radio);
      scn.agregar_esfera(esfera_render);

      std::string nombre_mat = "mat_esf_" + std::to_string(material_esferas.size());
      material_esferas.push_back(nombre_mat);

      if (esf.tipo_mat == ::MATE) {
        materiales[nombre_mat] = std::make_shared<material_mate>(esf.reflectancia);
      } else if (esf.tipo_mat == ::METAL) {
        materiales[nombre_mat] = std::make_shared<material_metal>(esf.reflectancia, esf.difusion);
      } else {
        materiales[nombre_mat] = std::make_shared<material_refractivo>(esf.indice_refraccion);
      }
    }

    for (auto const & cil : cilindros_temp) {
      render::cilindro cilindro_render(cil.centro, cil.eje, cil.radio);
      scn.agregar_cilindro(cilindro_render);

      std::string nombre_mat = "mat_cil_" + std::to_string(material_cilindros.size());
      material_cilindros.push_back(nombre_mat);

      if (cil.tipo_mat == ::MATE) {
        materiales[nombre_mat] = std::make_shared<material_mate>(cil.reflectancia);
      } else if (cil.tipo_mat == ::METAL) {
        materiales[nombre_mat] = std::make_shared<material_metal>(cil.reflectancia, cil.difusion);
      } else {
        materiales[nombre_mat] = std::make_shared<material_refractivo>(cil.indice_refraccion);
      }
    }
  }
