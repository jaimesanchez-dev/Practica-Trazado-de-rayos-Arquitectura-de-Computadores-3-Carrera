#include "trazador_rayos.hpp"
#include "cilindro.hpp"
#include "color.hpp"
#include "configuracion.hpp"
#include "esfera.hpp"
#include "interseccion.hpp"
#include "lector_archivo_escena.hpp"
#include "material.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <cmath>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace render {

  trazador_rayos::trazador_rayos(Configuracion const & cfg, mersenne_twister & mt)
      : config{cfg}, mt_materiales{mt} { }

  color trazador_rayos::calcular_color_fondo(vector const & direccion) const {
    double t = (direccion.getY() + 1.0) * 0.5;

    // Clamp a [0,1] por robustez frente a entradas no esperadas
    if (t < 0.0) {
      t = 0.0;
    } else if (t > 1.0) {
      t = 1.0;
    }

    color const claro(config.background_light_color);
    color const oscuro(config.background_dark_color);

    return claro.escalar(1.0 - t).suma(oscuro.escalar(t));
  }

  void trazador_rayos::cargar_escena(std::string const & archivo_escena) {
    // Usar los tipos del lector_archivo_escena (sin namespace render)
    std::vector<::esfera> esferas_temp;
    std::vector<::cilindro> cilindros_temp;
    leer_escena(archivo_escena, esferas_temp, cilindros_temp);

    // Convertir de estructuras de lectura a clases render
    for (auto const & esf : esferas_temp) {
      render::esfera const esfera_render(esf.centro, esf.radio);
      scn.agregar_esfera(esfera_render);

      std::string const nombre_mat = "mat_esf_" + std::to_string(material_esferas.size());
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
      render::cilindro const cilindro_render(cil.centro, cil.eje, cil.radio);
      scn.agregar_cilindro(cilindro_render);

      std::string const nombre_mat = "mat_cil_" + std::to_string(material_cilindros.size());
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

  std::shared_ptr<material> trazador_rayos::buscar_material(rayo const & r,
                                                            interseccion const & inter) const {
    auto esferas   = scn.obtener_esferas();
    auto cilindros = scn.obtener_cilindros();

    constexpr double EPSILON = 1e-6;
    for (size_t i = 0; i < esferas.size(); ++i) {
      double t_temp = 0.0;
      if (esferas[i].interseccion(r, t_temp) and std::abs(t_temp - inter.obtener_t()) < EPSILON) {
        if (i < material_esferas.size()) {
          return materiales.at(material_esferas[i]);
        }
        break;
      }
    }

    for (size_t i = 0; i < cilindros.size(); ++i) {
      double t_temp = 0.0;
      if (cilindros[i].interseccion(r, t_temp) and std::abs(t_temp - inter.obtener_t()) < EPSILON) {
        if (i < material_cilindros.size()) {
          return materiales.at(material_cilindros[i]);
        }
        break;
      }
    }

    // Material por defecto
    constexpr double DEFAULT_REFLECTANCE = 0.5;
    return std::make_shared<material_mate>(
        vector(DEFAULT_REFLECTANCE, DEFAULT_REFLECTANCE, DEFAULT_REFLECTANCE));
  }

  color trazador_rayos::procesar_material_refractivo(std::shared_ptr<material> const & mat,
                                                     rayo const & r, interseccion const & inter,
                                                     int profundidad) {
    auto mat_refractivo       = std::dynamic_pointer_cast<material_refractivo>(mat);
    vector const normal       = inter.obtener_normal();
    bool const frente_externo = inter.obtener_frente_externo();

    /* Calcular direcciÃ³n usando frente_externo */
    vector const dir_resultado = mat_refractivo->calcular_direccion_reflexion(
        r.obtener_direccion(), normal, frente_externo, mt_materiales);

    constexpr double OFFSET = 1e-4;
    vector punto_origen(0, 0, 0);

    if (dir_resultado.producto_escalar(normal) > 0) {
      punto_origen = inter.obtener_punto().suma(normal.producto_constante(OFFSET));
    } else {
      punto_origen = inter.obtener_punto().resta(normal.producto_constante(OFFSET));
    }

    rayo const nuevo_rayo(punto_origen, dir_resultado);
    return trazar_rayo(nuevo_rayo, profundidad - 1);
  }

  color trazador_rayos::procesar_material_no_refractivo(std::shared_ptr<material> const & mat,
                                                        rayo const & r, interseccion const & inter,
                                                        int profundidad) {
    bool const frente_externo = inter.obtener_frente_externo();

    vector const nueva_dir = mat->calcular_direccion_reflexion(
        r.obtener_direccion(), inter.obtener_normal(), frente_externo, mt_materiales);

    constexpr double OFFSET = 1e-3;
    rayo const rayo_reflejado(
        inter.obtener_punto().suma(inter.obtener_normal().producto_constante(OFFSET)), nueva_dir);

    color const color_reflejado = trazar_rayo(rayo_reflejado, profundidad - 1);
    vector const refl           = mat->obtener_reflectancia();
    return color_reflejado.multiplicar(color(refl));
  }

  color trazador_rayos::trazar_rayo(rayo const & r, int profundidad) {
    if (profundidad <= 0) {
      return {0.0, 0.0, 0.0};
    }

    interseccion const inter = scn.interseccion_mas_cercana(r);

    if (!inter.obtener_existe()) {
      return calcular_color_fondo(r.obtener_direccion());
    }

    std::shared_ptr<material> const mat = buscar_material(r, inter);

    if (mat->obtener_tipo() == tipo_material::REFRACTIVO) {
      return procesar_material_refractivo(mat, r, inter, profundidad);
    }

    return procesar_material_no_refractivo(mat, r, inter, profundidad);
  }

}  // namespace render
