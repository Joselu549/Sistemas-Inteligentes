/**
 * \file hecho.h
 * \brief Este es el archivo de cabecera de la clase Hecho
 * \author Jos‚ Luis Abell n Monreal
 * \date 25/01/2024
*/
#ifndef __HECHO_H
#define __HECHO_H
#include <string>
using namespace std;

/**
 * \brief Clase para modelar los hechos.
 *          Contiene dos atributos, uno el
 *          identificador del hecho y el factor de
 *          certeza de dicho hecho
*/
class Hecho {
private:
    string id;
    double fc;
public:
    /**
     * El constructor por defecto del Hecho. En el motor
     * de inferencias no se utiliza.
    */
    Hecho();
    /**
     * El constructor con todos los parÃ¡metros del Hecho. Dado un
     * string ID y un doble FC, construye el Hecho
    */
    Hecho(const string& nid, const double& nfc);
    /**
     * M‚todo getter para el ID del Hecho
     * \return El ID del Hecho
    */
    string getID();
    /**
     * M‚todo getter del FC del Hecho
     * \return El FC del Hecho
    */
    double getFC();
    /**
     * M‚todo setter para el FC del Hecho
    */
    void setFC(const double& nfc);
};

#endif
