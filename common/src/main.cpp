#include "cilindro.hpp"
#include "configuracion.hpp"
#include "esfera.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <fstream>
#include <iostream>
#include <string>

/*Hay que aññadir el resto de includes segun se vayan haciendo para las llamadas*/

static int leer_configuracion(std::string const & archivo_config) {
  std::ifstream archivo(archivo_config);
  if (!archivo.is_open()) {
    std::cerr << "Error al abrir el archivo de configuracion: " << archivo_config << "\n";
    return 1;
  }

  /*Leer la configuracion del archivo*/

  archivo.close();
  return 0;
}

int main(int argc, char * argv[]) {
  /*Comprobamos los agumentos*/
  if (argc != 4) {
    std::cerr << "Invalid number of arguments: " << argc - 1 << "\n";
    return 1;
  }

  std::string const archivo_config = argv[1];
  std::string const archivo_escena = argv[2];
  std::string const archivo_salida = argv[3];

  return 0;
}
