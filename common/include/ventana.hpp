#ifndef RENDER_VENTANA_HPP
#define RENDER_VENTANA_HPP

#include "geometria.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include "tamaño_ventana.hpp"
#include "vector.hpp"

namespace render {

  class ventana {
  private:
    // Almacenamiento de parametros de la ventana
    geometria geo;
    tamaño_imagen tam_img;
    mersenne_twister mt;

    // Datos derivados de los parametros
    vector v_focal = vector(0, 0, 0), v_focal_normalizado = vector(0, 0, 0),
           v_direccion_x = vector(0, 0, 0), v_direccion_y = vector(0, 0, 0),
           v_marco_x = vector(0, 0, 0), v_marco_y = vector(0, 0, 0), delta_x = vector(0, 0, 0),
           delta_y = vector(0, 0, 0), origen_ventana = vector(0, 0, 0);

    double tam_ventana_x = 0, tam_ventana_y = 0, d_focal = 0;

  public:
    ventana(geometria geo, tamaño_imagen tam_img, mersenne_twister mt);

    // Funciones de generacion de la ventana
    void calcular_vector_focal();
    void calcular_distancia_focal();
    void calcular_dimensiones_ventana();
    void calcular_v_focal_normalizado();
    void calcular_v_direccion();
    void calcular_marco_ventana();
    void calcular_delta_x_y();
    void calcular_origen_ventana();

    // Funciones de generacion de rayos
    vector calcular_coordenadas_dispersas(int pixel_x, int pixel_y);
    rayo generar_rayos_pixel(int pixel_x, int pixel_y);
    void aplicar_correccion_gamma();
    void aplicar_mapeo_255();
  };

}  // namespace render

#endif
