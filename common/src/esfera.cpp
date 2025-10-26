#include "esfera.hpp"
#include <cmath>

namespace render {

  bool esfera::interseccion(rayo const & r, double & t) const {
    /* Obtenemos los datos del rayo y la esfera */
    vector r_origen    = r.obtener_origen();     // O_r
    vector r_direccion = r.obtener_direccion();  // d_r

    vector e_centro = obtener_centro();  // C
    double e_radio  = obtener_radio();   // r

    /* Calculamos el vector r_c */
    vector r_c = e_centro.resta(r_origen);

    /* Calculamos los parámetros necesarios para la ecuación de segundo grado */
    double a        = r_direccion.producto_escalar(r_direccion);
    double producto = r_direccion.producto_escalar(r_c);
    double b        = -2 * producto;
    double c        = r_c.producto_escalar(r_c) - pow(e_radio, 2);

    /* Calculamos el valor del discriminante */
    double discriminante = pow(b, 2) - 4 * a * c;

    if (discriminante < 0) {
      return false;
    }

    /* Si llegamos aquí es que existe solución, calculamos los valores de lambda */
    double discriminante_raiz = std::sqrt(discriminante);
    double lambda1            = (-b - discriminante_raiz) / (2 * a);
    double lambda2            = (-b + discriminante_raiz) / (2 * a);

    /* Devolvemos el resultado que nos dé nuestra función solución */
    return resultado(lambda1, lambda2, t);
  }

  bool esfera::resultado(double lambda1, double lambda2, double & t) {
    /* Elegimos el valor más pequeño positivo */
    if (lambda1 >= 0.0 and lambda2 >= 0.0) {
      t = std::min(lambda1, lambda2);
      return true;
    }
    if (lambda1 > 0.0) {
      t = lambda1;
      return true;
    }
    if (lambda2 > 0.0) {
      t = lambda2;
      return true;
    }

    return false;
  }

}  // namespace render
