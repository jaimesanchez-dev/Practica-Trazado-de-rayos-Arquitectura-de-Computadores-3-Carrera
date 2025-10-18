#include "cilindro.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(CilindroTest, InterseccionCurva) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector base(5.0, -1.0, -1.0);
  vector eje(0.0, 2.0, 2.0);
  double radio(1.0);
  cilindro c(base, eje, radio);

  double t       = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}

TEST(CilindroTest, SinInterseccion) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(0.0, 1.0, 0.0);
  rayo r(origen, direccion);

  vector base(5.0, -1.0, -1.0);
  vector eje(0.0, 2.0, 2.0);
  double radio(1.0);
  cilindro c(base, eje, radio);

  double t       = -1.0;  // Valor por defecto, si existe solución se modifica después
  bool resultado = c.interseccion(r, t);

  EXPECT_FALSE(resultado);
}

TEST(CilindroTest, InterseccionBaseSuperior) {
  vector origen(0.0, -1.0, -1.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector base(5.0, -1.0, -1.0);
  vector eje(0.0, 2.0, 2.0);
  double radio(1.0);
  cilindro c(base, eje, radio);

  double t       = -1.0;
  bool resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}

TEST(CilindroTest, InterseccionBaseInferior) {
  vector origen(0.0, -1.0, -1.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector base(5.0, -1.0, -1.0);
  vector eje(0.0, 2.0, 2.0);
  double radio(1.0);
  cilindro c(base, eje, radio);

  double t       = -1.0;
  bool resultado = c.interseccion(r, t);

  EXPECT_TRUE(resultado);
  EXPECT_NEAR(t, 4.0, 1e-6);
}
