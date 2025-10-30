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
    vector const dir_norm = direccion.normalizar();
    double const t        = (dir_norm.getY() + 1.0) * 0.5;

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

  color trazador_rayos::trazar_rayo(rayo const & r, int profundidad) {
    if (profundidad <= 0) {
      return {0.0, 0.0, 0.0};
    }

    interseccion const inter = scn.interseccion_mas_cercana(r);

    if (!inter.obtener_existe()) {
      return calcular_color_fondo(r.obtener_direccion());
    }

    // Determinar qué material usar
    std::shared_ptr<material> mat;
    auto esferas   = scn.obtener_esferas();
    auto cilindros = scn.obtener_cilindros();

    // Buscar cuál objeto fue intersectado
    constexpr double EPSILON = 1e-6;
    for (size_t i = 0; i < esferas.size(); ++i) {
      double t_temp = 0.0;
      if (esferas[i].interseccion(r, t_temp) and std::abs(t_temp - inter.obtener_t()) < EPSILON) {
        if (i < material_esferas.size()) {
          mat = materiales[material_esferas[i]];
        }
        break;
      }
    }

    if (!mat) {
      for (size_t i = 0; i < cilindros.size(); ++i) {
        double t_temp = 0.0;
        if (cilindros[i].interseccion(r, t_temp) and std::abs(t_temp - inter.obtener_t()) < EPSILON)
        {
          if (i < material_cilindros.size()) {
            mat = materiales[material_cilindros[i]];
          }
          break;
        }
      }
    }

    if (!mat) {
      // Material por defecto
      constexpr double DEFAULT_REFLECTANCE = 0.5;
      mat                                  = std::make_shared<material_mate>(
          vector(DEFAULT_REFLECTANCE, DEFAULT_REFLECTANCE, DEFAULT_REFLECTANCE));
    }

    // Manejar materiales refractivos
    // Manejar materiales refractivos
    if (mat->obtener_tipo() == tipo_material::REFRACTIVO) {
      auto mat_refractivo = std::dynamic_pointer_cast<material_refractivo>(mat);

      // La normal de intersección ya apunta hacia afuera del objeto
      vector const normal = inter.obtener_normal();

      // El método calcular_direccion_reflexion YA maneja todo:
      // - Determina si entra o sale
      // - Calcula reflexión total interna si aplica
      // - Calcula refracción si no hay reflexión total
      vector const dir_resultado = mat_refractivo->calcular_direccion_reflexion(
          r.obtener_direccion(), normal, mt_materiales);

      // CLAVE: Aplicar offset basándose en la dirección RESULTANTE
      constexpr double OFFSET = 1e-4;
      vector punto_origen(0, 0, 0);

      // Si la dirección resultante apunta en el mismo sentido que la normal (hacia afuera)
      if (dir_resultado.producto_escalar(normal) > 0) {
        // Mover el punto hacia afuera
        punto_origen = inter.obtener_punto().suma(normal.producto_constante(OFFSET));
      } else {
        // Mover el punto hacia adentro
        punto_origen = inter.obtener_punto().resta(normal.producto_constante(OFFSET));
      }

      rayo const nuevo_rayo(punto_origen, dir_resultado);

      // Según el PDF (sección 3.5.3), la reflectancia de materiales refractivos es (1,1,1)
      color color_resultado = trazar_rayo(nuevo_rayo, profundidad - 1);
      return color_resultado;
    }

    // Para materiales no refractivos (mate y metal)
    vector const nueva_dir = mat->calcular_direccion_reflexion(
        r.obtener_direccion(), inter.obtener_normal(), mt_materiales);

    // Crear nuevo rayo con pequeño offset para evitar auto-intersección
    constexpr double OFFSET = 1e-3;
    rayo const rayo_reflejado(
        inter.obtener_punto().suma(inter.obtener_normal().producto_constante(OFFSET)), nueva_dir);

    // Trazar recursivamente
    color const color_reflejado = trazar_rayo(rayo_reflejado, profundidad - 1);

    // Aplicar reflectancia del material
    vector const refl = mat->obtener_reflectancia();
    return color_reflejado.multiplicar(color(refl));
  }

}  // namespace render
