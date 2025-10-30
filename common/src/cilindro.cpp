#include "cilindro.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <algorithm>
#include <cmath>

namespace render {

  bool cilindro::interseccion(rayo const & r, double & t) const {
    double t_temp = INFINITY;

    interseccion_curva(r, t_temp);
    interseccion_base_superior(r, t_temp);
    interseccion_base_inferior(r, t_temp);

    if (t_temp < INFINITY) {
      t = t_temp;
      return true;
    }

    return false;
  }

  namespace {

    struct datos_cilindro {
      vector base;
      vector eje;
      double radio;
      double altura;
    };

    struct parametros_perpendiculares {
      vector r_c_perp;
      vector d_perp;
    };

    struct coeficientes_cuadratica {
      double a;
      double b;
      double c;
    };

    parametros_perpendiculares calcular_perpendiculares(vector const & r_c, vector const & r_dir,
                                                        vector const & eje) {
      double const r_c_a    = r_c.producto_escalar(eje);
      vector const r_c_perp = r_c.resta(eje.producto_constante(r_c_a));

      double const d_a    = r_dir.producto_escalar(eje);
      vector const d_perp = r_dir.resta(eje.producto_constante(d_a));

      return {r_c_perp, d_perp};
    }

    bool altura_valida(vector const & punto, datos_cilindro const & cil) {
      double const altura_punto = punto.resta(cil.base).producto_escalar(cil.eje);
      return altura_punto >= 0.0 and altura_punto <= cil.altura;
    }

    void procesar_solucion(double lambda, rayo const & r, datos_cilindro const & cil,
                           double & t_temp) {
      if (lambda <= 1e-6) {
        return;
      }

      vector const punto =
          r.obtener_origen().suma(r.obtener_direccion().producto_constante(lambda));

      if (altura_valida(punto, cil)) {
        t_temp = std::min(t_temp, lambda);
      }
    }

    void resolver_ecuacion_cuadratica(coeficientes_cuadratica const & coef, rayo const & r,
                                      datos_cilindro const & cil, double & t_temp) {
      double const discriminante = (coef.b * coef.b) - (4 * coef.a * coef.c);
      if (discriminante < 0) {
        return;
      }

      double const raiz    = std::sqrt(discriminante);
      double const lambda1 = (-coef.b - raiz) / (2 * coef.a);
      double const lambda2 = (-coef.b + raiz) / (2 * coef.a);

      procesar_solucion(lambda1, r, cil, t_temp);
      procesar_solucion(lambda2, r, cil, t_temp);
    }

  }  // namespace

  bool cilindro::interseccion_curva(rayo const & r, double & t_temp) const {
    datos_cilindro const cil = {obtener_base(), obtener_eje(), obtener_radio(), obtener_altura()};

    vector const r_c              = r.obtener_origen().resta(cil.base);
    auto const [r_c_perp, d_perp] = calcular_perpendiculares(r_c, r.obtener_direccion(), cil.eje);

    coeficientes_cuadratica const coef = {
      d_perp.producto_escalar(d_perp), 2 * d_perp.producto_escalar(r_c_perp),
      r_c_perp.producto_escalar(r_c_perp) - (cil.radio * cil.radio)};

    resolver_ecuacion_cuadratica(coef, r, cil, t_temp);

    return true;
  }

  namespace {

    struct datos_base {
      vector punto;
      vector normal;
      double radio;
    };

    bool verificar_interseccion_base(rayo const & r, datos_base const & base, double & t_temp) {
      vector const r_origen = r.obtener_origen();
      vector const r_dir    = r.obtener_direccion();

      double const denom = r_dir.producto_escalar(base.normal);
      if (std::abs(denom) <= 1e-8) {
        return true;
      }

      double const lambda = base.punto.resta(r_origen).producto_escalar(base.normal) / denom;
      if (lambda <= 1e-6 or lambda >= t_temp) {
        return true;
      }

      vector const interseccion = r_origen.suma(r_dir.producto_constante(lambda));
      double const distancia    = interseccion.resta(base.punto).magnitude();

      if (distancia <= base.radio) {
        t_temp = lambda;
      }

      return true;
    }

  }  // namespace

  bool cilindro::interseccion_base_superior(rayo const & r, double & t_temp) const {
    vector const c_base   = obtener_base();
    vector const c_eje    = obtener_eje();
    double const c_altura = obtener_altura();

    datos_base const superior = {c_base.suma(c_eje.producto_constante(c_altura / 2.0)), c_eje,
                                 obtener_radio()};

    return verificar_interseccion_base(r, superior, t_temp);
  }

  bool cilindro::interseccion_base_inferior(rayo const & r, double & t_temp) const {
    vector const c_base   = obtener_base();
    vector const c_eje    = obtener_eje();
    double const c_altura = obtener_altura();

    datos_base const inferior = {c_base.resta(c_eje.producto_constante(c_altura / 2.0)),
                                 c_eje.producto_constante(-1.0), obtener_radio()};

    return verificar_interseccion_base(r, inferior, t_temp);
  }

}  // namespace render
