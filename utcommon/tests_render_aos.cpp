#include "color.hpp"
#include "imagen_aos.cpp"
#include "imagen_aos.hpp"
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using namespace render;

TEST(ImagenAos, CreacionValidaEInvalida) {
  // Dimensiones válidas
  imagen_aos img(10, 5);
  EXPECT_EQ(img.obtener_ancho(), 10);
  EXPECT_EQ(img.obtener_alto(), 5);

  // Dimensiones inválidas
  EXPECT_THROW(imagen_aos(0, 10), std::invalid_argument);
  EXPECT_THROW(imagen_aos(-5, 10), std::invalid_argument);
  EXPECT_THROW(imagen_aos(10, 0), std::invalid_argument);
}

TEST(ImagenAos, EstablecerPixelDentroDeLimites) {
  imagen_aos img(3, 2);
  color rojo(1.0, 0.0, 0.0);
  img.establecer_pixel(1, 1, rojo);

  std::string const archivo = "test_output.ppm";
  img.guardar_ppm(archivo);

  std::ifstream in(archivo);
  ASSERT_TRUE(in.is_open());

  std::string header;
  std::getline(in, header);
  EXPECT_EQ(header, "P3");

  in.close();
  (void) std::remove(archivo.c_str());
}

TEST(ImagenAos, EstablecerPixelFueraDeLimitesNoLanza) {
  imagen_aos img(3, 3);
  color verde(0.0, 1.0, 0.0);

  EXPECT_NO_THROW(img.establecer_pixel(-1, 0, verde));
  EXPECT_NO_THROW(img.establecer_pixel(3, 0, verde));
  EXPECT_NO_THROW(img.establecer_pixel(0, 3, verde));
}

TEST(ImagenAos, GuardarPPMGeneraArchivoCorrecto) {
  imagen_aos img(2, 2);
  img.establecer_pixel(0, 0, color(1.0, 0.0, 0.0));  // rojo
  img.establecer_pixel(1, 0, color(0.0, 1.0, 0.0));  // verde
  img.establecer_pixel(0, 1, color(0.0, 0.0, 1.0));  // azul
  img.establecer_pixel(1, 1, color(1.0, 1.0, 1.0));  // blanco

  std::string const archivo = "imagen_test.ppm";
  img.guardar_ppm(archivo);

  std::ifstream in(archivo);
  ASSERT_TRUE(in.is_open());

  std::string header;
  std::getline(in, header);
  EXPECT_EQ(header, "P3");

  int ancho = 0, alto = 0, maxval = 0;
  in >> ancho >> alto >> maxval;
  EXPECT_EQ(ancho, 2);
  EXPECT_EQ(alto, 2);
  EXPECT_EQ(maxval, 255);

  in.close();
  (void) std::remove(archivo.c_str());
}
