#include "vector.hpp"

#include <cmath>

namespace render {

  double vector::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
  }

  vector vector::suma(vector const & otro_vector) const {
    return {x + otro_vector.x, y + otro_vector.y, z + otro_vector.z};
  }

  vector vector::resta(vector const & otro_vector) const {
    return {x - otro_vector.x, y - otro_vector.y, z - otro_vector.z};
  }

  double vector::producto(vector const & otro_vector) const {
    return x * otro_vector.x + y * otro_vector.y + z * otro_vector.z;
  }

  vector vector::producto_vectorial(vector const & otro_vector) const {
    return {y * otro_vector.z - z * otro_vector.y, z * otro_vector.x - x * otro_vector.z,
            x * otro_vector.y - y * otro_vector.x};
  }

  vector vector::normalizar() const {
    double mag = magnitude();  // Usamos el metodo magnitude()
    if (mag == 0.0) {
      // Por si es un vector nulo
      return {0.0, 0.0, 0.0};
    }
    return {x / mag, y / mag, z / mag};
  }

  //...

}  // namespace render
