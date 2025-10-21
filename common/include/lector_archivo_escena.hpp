#ifndef LECTOR_CONFIG_HPP
#define LECTOR_CONFIG_HPP

#include "vector.hpp"
#include <string>

/*Estructura para almacenar los parámetros de la escena en un struct*/

struct mate {
  std::string nombre;
  double R;
  double G;
  double B;
};

struct metal {
  std::string nombre;
  double R;
  double G;
  double B;
  double refraccion;
};

struct refractivo {
  std::string nombre;
  double refraccion;
};

struct datos_esfera {
  render::vector centro;
  double radio;
  void const * material;
};

struct datos_cilindro {
  render::vector centro;
  double radio;
  render::vector eje;
  void const * material;
};

#endif
