#include "geometria.hpp"

namespace render {

  geometria::geometria(vector vector_pov, vector vector_destino, vector vector_norte, double fov)
      : POV(vector_pov), DESTINO(vector_destino), NORTE(vector_norte), FOV(fov * (M_PI / 180)) { }

}  // namespace render
