#include "color.hpp"
#include "imagen_soa.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

using namespace render;

class ImagenSoaTest : public ::testing::Test {
protected:
  std::string archivo_temporal;

  void SetUp() override { archivo_temporal = "test_imagen_soa_temp.ppm"; }

  void TearDown() override {
    if (std::filesystem::exists(archivo_temporal)) {
      std::filesystem::remove(archivo_temporal);
    }
  }
};

TEST_F(ImagenSoaTest, ConstruccionBasica) {
  imagen_soa img(800, 600);

  EXPECT_EQ(img.obtener_ancho(), 800);
  EXPECT_EQ(img.obtener_alto(), 600);
}

TEST_F(ImagenSoaTest, DimensionesInvalidas) {
  EXPECT_THROW(imagen_soa img(0, 100), std::invalid_argument);
  EXPECT_THROW(imagen_soa img(100, 0), std::invalid_argument);
  EXPECT_THROW(imagen_soa img(-100, 100), std::invalid_argument);
  EXPECT_THROW(imagen_soa img(100, -100), std::invalid_argument);
}

TEST_F(ImagenSoaTest, EstablecerPixel) {
  imagen_soa img(10, 10);
  color c(1.0, 0.5, 0.0);

  // No debe lanzar excepción
  EXPECT_NO_THROW(img.establecer_pixel(5, 5, c));
}

TEST_F(ImagenSoaTest, EstablecerPixelFueraDeRango) {
  imagen_soa img(10, 10);
  color c(1.0, 0.5, 0.0);

  // Estas operaciones no deben causar errores (se ignoran silenciosamente según la implementación)
  EXPECT_NO_THROW(img.establecer_pixel(-1, 5, c));
  EXPECT_NO_THROW(img.establecer_pixel(5, -1, c));
  EXPECT_NO_THROW(img.establecer_pixel(10, 5, c));
  EXPECT_NO_THROW(img.establecer_pixel(5, 10, c));
}

TEST_F(ImagenSoaTest, GuardarPPM) {
  imagen_soa img(2, 2);
  color rojo(1.0, 0.0, 0.0);
  color verde(0.0, 1.0, 0.0);
  color azul(0.0, 0.0, 1.0);
  color blanco(1.0, 1.0, 1.0);

  img.establecer_pixel(0, 0, rojo);
  img.establecer_pixel(1, 0, verde);
  img.establecer_pixel(0, 1, azul);
  img.establecer_pixel(1, 1, blanco);

  EXPECT_NO_THROW(img.guardar_ppm(archivo_temporal));
  EXPECT_TRUE(std::filesystem::exists(archivo_temporal));
}

TEST_F(ImagenSoaTest, ContenidoPPM) {
  imagen_soa img(2, 1);
  color rojo(1.0, 0.0, 0.0);
  color verde(0.0, 1.0, 0.0);

  img.establecer_pixel(0, 0, rojo);
  img.establecer_pixel(1, 0, verde);

  img.guardar_ppm(archivo_temporal);

  std::ifstream archivo(archivo_temporal);
  ASSERT_TRUE(archivo.is_open());

  std::string linea;

  // Verificar cabecera P3
  std::getline(archivo, linea);
  EXPECT_EQ(linea, "P3");

  // Verificar dimensiones
  std::getline(archivo, linea);
  EXPECT_EQ(linea, "2 1");

  // Verificar valor máximo
  std::getline(archivo, linea);
  EXPECT_EQ(linea, "255");

  // Verificar primer pixel (rojo)
  std::getline(archivo, linea);
  EXPECT_EQ(linea, "255 0 0");

  // Verificar segundo pixel (verde)
  std::getline(archivo, linea);
  EXPECT_EQ(linea, "0 255 0");
}

TEST_F(ImagenSoaTest, ImagenGrande) {
  imagen_soa img(1'920, 1'080);

  EXPECT_EQ(img.obtener_ancho(), 1'920);
  EXPECT_EQ(img.obtener_alto(), 1'080);

  // Establecer algunos píxeles
  color c(0.5, 0.5, 0.5);
  EXPECT_NO_THROW(img.establecer_pixel(960, 540, c));
  EXPECT_NO_THROW(img.establecer_pixel(0, 0, c));
  EXPECT_NO_THROW(img.establecer_pixel(1'919, 1'079, c));
}

TEST_F(ImagenSoaTest, ImagenPequena) {
  imagen_soa img(1, 1);

  EXPECT_EQ(img.obtener_ancho(), 1);
  EXPECT_EQ(img.obtener_alto(), 1);

  color c(0.5, 0.5, 0.5);
  EXPECT_NO_THROW(img.establecer_pixel(0, 0, c));
}

TEST_F(ImagenSoaTest, MultiplesPixeles) {
  imagen_soa img(10, 10);

  for (int y = 0; y < 10; ++y) {
    for (int x = 0; x < 10; ++x) {
      double r = static_cast<double>(x) / 9.0;
      double g = static_cast<double>(y) / 9.0;
      double b = 0.5;
      color c(r, g, b);
      EXPECT_NO_THROW(img.establecer_pixel(x, y, c));
    }
  }
}

TEST_F(ImagenSoaTest, GuardarImagenCompleta) {
  imagen_soa img(100, 100);

  // Llenar toda la imagen con un gradiente
  for (int y = 0; y < 100; ++y) {
    for (int x = 0; x < 100; ++x) {
      double intensidad = static_cast<double>(x + y) / 198.0;
      color c(intensidad, intensidad, intensidad);
      img.establecer_pixel(x, y, c);
    }
  }

  EXPECT_NO_THROW(img.guardar_ppm(archivo_temporal));
  EXPECT_TRUE(std::filesystem::exists(archivo_temporal));
}
