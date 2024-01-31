/**
 * \file regla.h
 * \brief Este es el archivo de cabecera de la clase Regla
 * \author Jos‚ Luis Abell n Monreal
 * \date 25/01/2024
*/
#ifndef __REGLA_H
#define __REGLA_H
#include <string>
#include <vector>
using namespace std;

class Regla {
private:
    string id;  /**< El ID de la Regla */
    double fc;  /**< El FC de la Regla */
    unsigned short int op;  /**< (0) unitario, (1) conjunci¢n o (2) disyunci¢n */
    vector<string> ops; /**< ID de los dos Hecho operadores */
    string res; /**< El ID del Hecho resultante de la regla */
public:
    /**
     * El constructor por defecto de la clase Regla.
     * En el motor de inferencias no se utiliza.
    */
    Regla();
    /**
     * El constructor completo de la clase Regla.
     * Este constructor recibe todos los datos para la Regla.
    */
    Regla(const string& nid, const double& nfc, const unsigned short int& nop, const string& op1, const string& op2, const string& nres);
    /**
     * M‚todo getter para el ID de la Regla.
     * \return El ID resultante de la Regla.
    */
    string getID();
    /**
     * M‚todo getter para el FC de la Regla.
     * \return El FC del Hecho resultante de la Regla.
    */
    double getFC();
    /**
     * M‚todo getter para la operaci¢n a realizar en la Regla.
     * \return Un entero indicando si es unitario, conjunci¢n o disyunci¢n.
    */
    unsigned short int getOP();
    /**
     * M‚todo getter para los IDs de los antecesores de la regla.
     * \return Un vector de IDs que indican los hechos antecesores a dicha Regla.
    */
    vector<string> getOPS();
    /**
     * M‚todo getter para el hecho resultante.
     * \return El ID de el hecho resultante de la Regla.
    */
    string getRes();
};

#endif
