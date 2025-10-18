#include "esfera.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(EsferaTest, InterseccionFrontal) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector centro(5.0, 0.0, 0.0);
  double radio(1.0);
  esfera e(centro, radio);

  double t       = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}

TEST(EsferaTest, SinInterseccion) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(0.0, 1.0, 0.0);
  rayo r(origen, direccion);

  vector centro(5.0, 0.0, 0.0);
  double radio(1.0);
  esfera e(centro, radio);

  double t       = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool resultado = e.interseccion(r, t);

  EXPECT_FALSE(resultado);
}

TEST(EsferaTest, InterseccionTangente) {
  vector origen(0.0, 1.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector centro(5.0, 0.0, 0.0);
  double radio(1.0);
  esfera e(centro, radio);

  double t       = -1.0;
  bool resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 5.0, 1e-6);
}

TEST(EsferaTest, RayoDentroDeLaEsfera) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector centro(0.0, 0.0, 0.0);
  double radio(1.0);
  esfera e(centro, radio);

  double t       = -1.0;
  bool resultado = e.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 1.0, 1e-6);
}
