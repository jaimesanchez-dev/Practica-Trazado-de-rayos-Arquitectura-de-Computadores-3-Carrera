#include "cilindro.hpp"
#include "configuracion.hpp"
#include "esfera.hpp"
#include "lector_archivo_escena.hpp"
#include "rayo.hpp"
#include "vector.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

/*Hay que aññadir el resto de includes segun se vayan haciendo para las llamadas*/

int main(int argc, char * argv[]) {
  /*Comprobamos los agumentos*/
  if (argc != 4) {
    std::cerr << "Invalid number of arguments: " << argc - 1 << "\n";
    return 1;
  }

  std::string const archivo_config = argv[1];
  std::string const archivo_escena = argv[2];
  std::string const archivo_salida = argv[3];

  /*Leemos la configuracion*/

  Configuracion config = leer_configuracion(archivo_config);

  /*Leemos la escena*/
  std::vector<esfera> esferas;
  std::vector<cilindro> cilindros;
  leer_escena(archivo_escena, esferas, cilindros);

  /*generamos arhcivo de salida:*/
  std::ofstream salida(archivo_salida);
  if (!salida.is_open()) {
    std::cerr << "Error al abrir el archivo de salida: " << archivo_salida << "\n";
    return 1;
  }
  /*...................*/

  return 0;
}
