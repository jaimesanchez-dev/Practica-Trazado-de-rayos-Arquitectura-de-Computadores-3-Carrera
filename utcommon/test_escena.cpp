#include "cilindro.hpp"
#include "escena.hpp"
#include "esfera.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(EscenaTest, InterseccionConEsfera) {
  escena scn;
  esfera e(vector(0.0, 0.0, -5.0), 1.0);
  scn.agregar_esfera(e);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
  EXPECT_NEAR(inter.obtener_t(), 4.0, 1e-6);  // el rayo impacta a z = -4
}

TEST(EscenaTest, InterseccionConCilindro) {
  escena scn;
  cilindro c(vector(0.0, 0.0, -5.0), vector(0.0, 1.0, 0.0), 1.0);
  scn.agregar_cilindro(c);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
}

TEST(EscenaTest, SinInterseccion) {
  escena scn;
  esfera e(vector(10.0, 0.0, 0.0), 1.0);
  scn.agregar_esfera(e);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 1.0, 0.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_FALSE(inter.obtener_existe());
}

TEST(EscenaTest, EligeEsferaMasCercana) {
  escena scn;

  esfera esfera1(vector(0.0, 0.0, -3.0), 1.0);

  esfera esfera2(vector(0.0, 0.0, -8.0), 1.0);

  scn.agregar_esfera(esfera1);
  scn.agregar_esfera(esfera2);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());

  EXPECT_NEAR(inter.obtener_t(), 2.0, 1e-6);

  vector p = inter.obtener_punto();
  EXPECT_NEAR(p.getZ(), -2.0, 1e-6);
}

TEST(EscenaTest, EligeCilindroMasCercano) {
  escena scn;

  cilindro cilindro1(vector(0.0, 0.0, -4.0), vector(0.0, 1.0, 0.0), 1.0);

  cilindro cilindro2(vector(0.0, 0.0, -10.0), vector(0.0, 1.0, 0.0), 1.0);

  scn.agregar_cilindro(cilindro1);
  scn.agregar_cilindro(cilindro2);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());

  EXPECT_NEAR(inter.obtener_t(), 3.0, 1e-6);
}

TEST(EscenaTest, EligeEsferaSobreCilindro) {
  escena scn;

  esfera e(vector(0.0, 0.0, -3.0), 1.0);

  cilindro c(vector(0.0, 0.0, -8.0), vector(0.0, 1.0, 0.0), 1.0);

  scn.agregar_esfera(e);
  scn.agregar_cilindro(c);

  rayo r(vector(0.0, 0.0, 0.0), vector(0.0, 0.0, -1.0));

  interseccion inter = scn.interseccion_mas_cercana(r);

  EXPECT_TRUE(inter.obtener_existe());
  EXPECT_NEAR(inter.obtener_t(), 2.0, 1e-6);

  vector p = inter.obtener_punto();
  EXPECT_NEAR(p.getZ(), -2.0, 1e-6);
}
