#include "rayo.hpp"
#include "vector.hpp"
#include <gtest/gtest.h>

using namespace render;

TEST(RayoTest, PosicionOrigen) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector p = r.posicion(0.0);

  EXPECT_DOUBLE_EQ(p.getX(), 0.0);
  EXPECT_DOUBLE_EQ(p.getY(), 0.0);
  EXPECT_DOUBLE_EQ(p.getZ(), 0.0);
}

TEST(RayoTest, SumaDeUnaCoordenada) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(1.0, 0.0, 0.0);
  rayo r(origen, direccion);

  vector p = r.posicion(1.0);

  EXPECT_DOUBLE_EQ(p.getX(), 1.0);
  EXPECT_DOUBLE_EQ(p.getY(), 0.0);
  EXPECT_DOUBLE_EQ(p.getZ(), 0.0);
}

TEST(RayoTest, SumaDeDosCoordenadas) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(2.0, 5.0, 0.0);
  rayo r(origen, direccion);

  vector p = r.posicion(1.0);

  EXPECT_DOUBLE_EQ(p.getX(), 2.0);
  EXPECT_DOUBLE_EQ(p.getY(), 5.0);
  EXPECT_DOUBLE_EQ(p.getZ(), 0.0);
}

TEST(RayoTest, SumaDeTresCoordenadas) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(2.0, 5.0, 8.0);
  rayo r(origen, direccion);

  vector p = r.posicion(1.0);

  EXPECT_DOUBLE_EQ(p.getX(), 2.0);
  EXPECT_DOUBLE_EQ(p.getY(), 5.0);
  EXPECT_DOUBLE_EQ(p.getZ(), 8.0);
}

TEST(RayoTest, MultiplicacionMayorQueUno) {
  vector origen(0.0, 0.0, 0.0);
  vector direccion(2.0, 5.0, 8.0);
  rayo r(origen, direccion);

  vector p = r.posicion(3.0);

  EXPECT_DOUBLE_EQ(p.getX(), 6.0);
  EXPECT_DOUBLE_EQ(p.getY(), 15.0);
  EXPECT_DOUBLE_EQ(p.getZ(), 24.0);
}
