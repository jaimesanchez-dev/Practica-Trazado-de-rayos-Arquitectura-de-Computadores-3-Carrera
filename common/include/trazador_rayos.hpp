#ifndef RENDER_TRAZADOR_RAYOS_HPP
#define RENDER_TRAZADOR_RAYOS_HPP

#include "color.hpp"
#include "configuracion.hpp"
#include "escena.hpp"
#include "interseccion.hpp"
#include "material.hpp"
#include "mersenne_twister.hpp"
#include "rayo.hpp"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace render {

  class trazador_rayos {
  private:
    Configuracion const & config;
    escena scn;
    mersenne_twister & mt_materiales;
    std::unordered_map<std::string, std::shared_ptr<material>> materiales;
    std::vector<std::string> material_esferas;
    std::vector<std::string> material_cilindros;

    [[nodiscard]] color calcular_color_fondo(vector const & direccion) const;

    [[nodiscard]] std::shared_ptr<material> buscar_material(rayo const & r,
                                                            interseccion const & inter) const;

    [[nodiscard]] color procesar_material_refractivo(std::shared_ptr<material> const & mat,
                                                     rayo const & r, interseccion const & inter,
                                                     int profundidad);

    [[nodiscard]] color procesar_material_no_refractivo(std::shared_ptr<material> const & mat,
                                                        rayo const & r, interseccion const & inter,
                                                        int profundidad);

  public:
    trazador_rayos(Configuracion const & cfg, mersenne_twister & mt);

    // Eliminar constructores de copia y movimiento porque tiene referencias
    trazador_rayos(trazador_rayos const &)             = delete;
    trazador_rayos & operator=(trazador_rayos const &) = delete;
    trazador_rayos(trazador_rayos &&)                  = delete;
    trazador_rayos & operator=(trazador_rayos &&)      = delete;
    ~trazador_rayos()                                  = default;

    void cargar_escena(std::string const & archivo_escena);

    [[nodiscard]] color trazar_rayo(rayo const & r, int profundidad);

    [[nodiscard]] escena const & obtener_escena() const { return scn; }
  };

}  // namespace render

#endif
