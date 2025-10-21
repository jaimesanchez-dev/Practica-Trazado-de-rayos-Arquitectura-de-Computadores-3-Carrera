#include "configuracion.hpp"
#include "vector.hpp"
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {

  struct contexto_parseo {
    std::string linea;
    int num_linea = 0;
  };

  void trim(std::string & s) {
    // Elimina espacios en blanco al inicio y al final
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
      s.clear();
    } else {
      s = s.substr(start, end - start + 1);
    }
  }

}  // namespace

int leer_configuracion(std::string const & ruta) {
  std::ifstream archivo(ruta);
  if (!archivo.is_open()) {
    throw std::runtime_error("No se pudo abrir el archivo de configuración: " + ruta);
  }
  /*leemos el archivo linea por linea*/
  std::string linea;
  int numero_linea = 0;
  while (std::getline(archivo, linea)) {
    numero_linea++;
    trim(linea);
    /*Ignorar líneas vacías*/
    if (linea.empty()) {
      continue;
    }
    /*Leemos la etiqueta de cada linea ej:gamma*/
    std::istringstream iss(linea);
    std::string etiqueta;
    iss >> etiqueta;
    /*Verificamos que la etiqueta sea válida*/
    if (etiqueta.empty() or etiqueta.back() != ':') {
      throw std::runtime_error("Error: Unknown configuration key: [" + etiqueta + "]");
    }

    contexto_parseo ctx{linea, numero_linea}; /*he creado el struct para pasar parametros para que
                                                 no haya problemas con el clan-tidy*/

    void procesar_etiqueta(, std::string const & etiqueta, std::istringstream & iss,
                           contexto_parseo const & ctx)
    // procesar_etiqueta(config, etiqueta, iss, ctx);
  }
}
