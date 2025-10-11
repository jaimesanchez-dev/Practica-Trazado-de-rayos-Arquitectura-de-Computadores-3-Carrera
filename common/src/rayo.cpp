#include "rayo.hpp"

namespace render {

  vector rayo::posicion(double distancia) const {
    /* Hay que multiplicar distancia por el vector de direccion, por lo que extraemos las
     * coordenadas */
    double coordenada_x = direccion.getX();
    double coordenada_y = direccion.getY();
    double coordenada_z = direccion.getZ();

    /* Multiplicamos cada coordenada por separado */
    double vector_x = coordenada_x * distancia;
    double vector_y = coordenada_y * distancia;
    double vector_z = coordenada_z * distancia;

    /* Con la multiplicación ya hecha, creamos el vector */
    vector nuevo_vector(vector_x, vector_y, vector_z);

    return origen.suma(nuevo_vector);
  }

}  // namespace render
