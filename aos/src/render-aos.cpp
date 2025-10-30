#include "render-aos.hpp"
#include <fstream>
#include <iostream>
#include <algorithm>



namespace render {

    // Constructor
    RenderAOS::RenderAOS(int ancho, int alto) 
    : ancho(ancho), alto(alto) {
    // Inicializar todos los píxeles en negro
        pixeles.resize(ancho * alto, Pixel(0, 0, 0));
}

    void RenderAOS::renderizar(const Escena& escena) {
    // Implementación básica del trazado de rayos

    
    vector posicion_camara(0.0, 0.0, -5.0);

    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            // Convertir coordenadas de píxel a coordenadas normalizadas
            double u = (x + 0.5) / ancho;  // 0 a 1
            double v = (y + 0.5) / alto;   // 0 a 1
        
            u *= static_cast<double>(ancho) / alto;
            
            vector direccion(u, v, 1.0);
            rayo rayo(posicion_camara, direccion);
            
            int indice = y * ancho + x;

            // Para MÚLTIPLES objetos
            Interseccion interseccion = escena.interseccion_mas_cercana(rayo);
            if (interseccion.obtener_existe()) {
            // Calcular color basado en el material del objeto
                Color color = interseccion.obtener_material().calcular_color();
                pixeles[indice] = color;
            }
            else {
                pixel[indice] = Pixel(0, 0, 0); //esto está mal (siempre habra interseccion??)
            }
        }
    }
    }

    
    // Guardar la imagen en formato PPM
    void guardarPPM(const std::string& nombre_archivo) const;{

        // Abrir archivo para escribir
    std::ofstream archivo(nombre_archivo);
    
    if (!archivo) {
        std::cerr << "No se pudo crear " << nombre_archivo << std::endl;
        return;
    }
    //Escribir TODOS los píxeles
    for (int y = 0; y < alto; ++y) {
        for (int x = 0; x < ancho; ++x) {
            const Pixel& pixel = pixeles[y * ancho + x];
            archivo << static_cast<int>(pixel.r) << " "
                    << static_cast<int>(pixel.g) << " " 
                    << static_cast<int>(pixel.b) << "\n";
        }
    }


    }


}
