/** \file main.cpp
 * \brief Este es el archivo main, en el que est� todo el
 *          código del motor de inferencias y la funci�n main.
 * \author José Luis Abellán Monreal
 * \date 25/01/2024
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
#include "hecho.h"
#include "regla.h"

using namespace std;

vector<Hecho> BH; /**< Base de Hechos. La base se declara como un vector de Hecho. */
vector<Regla> BC; /**< Base de Conocimientos. La base se declara como un vector de Regla. */

/** \brief Este método sirve para imprimir las bases de hechos
 *          y de conocimientos.
 */
void print_bases(ofstream& salida) {
    salida << "--- BC ---\n";
    for (int i = 0; i < (int) BC.size(); i++) {
        salida << "Nº " << i+1 << " = ID: " << BC[i].getID() << ", FC: " << BC[i].getFC() <<
        ", OP: " << BC[i].getOP() << ", OP1: " << BC[i].getOPS()[0] << ", OP2: " << BC[i].getOPS()[1] <<
        ", RES: " << BC[i].getRes() << endl;
    }

    salida << "--- BH ---\n";
    for (int i = 0; i < (int) BH.size(); i++) {
        salida << "Nº " << i+1 << " = ID: " << BH[i].getID() << ", FC: " << BH[i].getFC() << endl;
    }
}

/** \brief Este método sirve para separar una cadena en subcadenas
 *          dado un patrón de entrada.
 * \param cadena La cadena completa a separar.
 * \param patron El patron por el que separar la cadena.
 * \return Un vector de cadenas que contiene todas las subcadenas separadas por el patrón.
 */
vector<string> separar_campos(const string& cadena, const string& patron) {
    vector<string> resultado;
    int inicio = 0;
    int fin = cadena.find(patron);

    while (fin != (int) string::npos) {
        resultado.push_back(cadena.substr(inicio, fin - inicio));
        inicio = fin + patron.length();
        fin = cadena.find(patron, inicio);
    }
    resultado.push_back(cadena.substr(inicio, fin));
    return resultado;
}

/** \brief Este método sirve para buscar un hecho en la BH.
 * \param meta El identificador (ID) del Hecho.
 * \return Un puntero al hecho de la BH o NULL si el ID no está en la BH.
 */
Hecho * buscar_hecho(string meta) {
    for (int i = 0; i < (int) BH.size(); i++) {
        if (BH[i].getID() == meta)
            return &BH[i];
    }
    return NULL;
}

/**
 * \brief Este método comprueba si en un string dado por parámetro hay espacios.
 * \param str El string a comprobar.
 * \return Un booleano indicando si hay un espacio en el string o no.
*/
bool hay_espacios(const string& str) {
    for (int i = 0; i < (int) str.length(); i++) {
        if (str[i] == ' ')
            return true;
    }
    return false;
}

/**
 * \brief Este método devuelve las Reglas que derivan en un Hecho dado por parámetro.
 * \param meta El ID del Hecho a comprobar.
*/
vector<Regla> equiparar(string meta) {
    vector<Regla> res;
    for (int i = 0; i < (int) BC.size(); i++) {
        if (BC[i].getRes() == meta)
            res.push_back(BC[i]);
    }
    return res;
}

/**
 * \brief Este método calcula los Casos 1 y 3 para las actualizaciones de los FC para hallar el FC meta.
 * \param R La Regla por la que concatenar los FC.
 * \return Un doble que es el resultado de concatenar los FC de los Hechos antecesores con el de la Regla.
*/
double calcular_prob(Regla R, ofstream& salida) {
    salida << "Regla a comprobar: " << R.getID() << endl;
    switch (R.getOP()) {
    case 1:
        salida << "\tCaso 1: min(" << buscar_hecho(R.getOPS()[0])->getFC() << ", " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        salida << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, min(buscar_hecho(R.getOPS()[0])->getFC(), buscar_hecho(R.getOPS()[1])->getFC()));
    case 2:
        salida << "\tCaso 1: max(" << buscar_hecho(R.getOPS()[0])->getFC() << ", " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        salida << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[1])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, max(buscar_hecho(R.getOPS()[0])->getFC(), buscar_hecho(R.getOPS()[1])->getFC()));
    default:
        salida << "\tCaso 3: " << R.getFC() << " * max(0, " << buscar_hecho(R.getOPS()[0])->getFC() << ")" << endl;
        return R.getFC() * max((double) 0, buscar_hecho(R.getOPS()[0])->getFC());
    }
}

/**
 * \brief Método principal para el motor de inferencias con encadenamiento hacia atrás.
 *          Desde este método se comprueba si el Hecho pasado por parámetro puede ser resultado de los Hechos iniciales.
 * \param meta El ID de la meta a verificar.
 * \return Un booleano que indica si la meta se puede derivar de los Hechos iniciales.
*/
bool verificar(string meta, ofstream& salida) {
    bool verificado = false;
    /** Si la meta está ya contenida en la BH, devuelve true y el método no sigue. */
    if (buscar_hecho(meta) != NULL)
        verificado = true;
    else {
        /** Saca las reglas que derivan en la meta */
        vector<Regla> CC = equiparar(meta);
        /** Printea el Conjunto Conflicto */
        salida << "Conjunto Conflicto para la meta " << meta << ":" << endl;
        for (int i = 0; i < (int) CC.size(); i++) {
            salida << "\tNº " << i+1 << " = ID: " << CC[i].getID() << ", FC: " << CC[i].getFC() <<
            ", OP: " << CC[i].getOP() << ", OP1: " << CC[i].getOPS()[0] << ", OP2: " << CC[i].getOPS()[1] <<
            ", RES: " << CC[i].getRes() << endl;
        }
        /** Recorremos todas las reglas que derivan en la meta */
        while (!CC.empty()) {
            Regla R = CC[0];        /** Seleccionamos la primera Regla */
            CC.erase(CC.begin());   /** Y la eliminamos */
            salida << "Regla " << R.getID() << " (seleccionada)" << endl;
            /** Cogemos los Hechos antecesores a la Regla escogida */
            vector<string> nuevasMetas = R.getOPS();
            /** Si es unitario, el segundo operando lo eliminamos porque no existe */
            if (R.getOP() == 0)
                nuevasMetas.erase(nuevasMetas.begin() + 1);
            /** Y printeamos los antecesores */
            salida << "Antecesores de " << R.getID() << ":" << endl;
            for (int i = 0; i < (int) nuevasMetas.size(); i++) {
                salida << "\tNº " << i+1 << ": ID: " << nuevasMetas[i] << endl;
            }
            /** Verificamos el Hecho */
            verificado = true;
            /** Recorremos todos los Hechos antecesores a la Regla */
            while (!nuevasMetas.empty()) {
                string nMet = nuevasMetas[0];               /** Seleccionamos el primer Hecho y eliminamos */
                nuevasMetas.erase(nuevasMetas.begin());
                verificado = verificar(nMet, salida);               /** Recursividad */
            }

            if (verificado) {
                /** Si está verificado y no está en la BH */
                if (buscar_hecho(meta) == NULL) {
                    /** Creamos el Hecho y lo añadimos a la BH */
                    Hecho aux(meta, calcular_prob(R, salida));
                    BH.push_back(aux);
                    salida << "Añadido " << meta << " a la BH con FC = " << buscar_hecho(R.getRes())->getFC() << endl;
                } else {
                    /** Si está en la BH, concatenamos el anterior FC calculado
                       Aquí es donde se comprueba el caso 2 para concatenar los FC */
                    double prob_ant = buscar_hecho(meta)->getFC();
                    double prob_calc = calcular_prob(R, salida);
                    double prob_nueva = 0.0;
                    salida << "Concatenar " << prob_ant << " y " << prob_calc << endl;
                    salida << "\tCaso 2: ";
                    if (prob_calc >= 0.0 && prob_ant >= 0.0) {
                        prob_nueva = prob_ant + prob_calc * (1 - prob_ant);
                        salida << prob_ant << " + " << prob_calc << " * (1 - " << prob_ant << ")" << endl;
                    } else if (prob_calc <= 0.0 && prob_ant <= 0.0) {
                        prob_nueva = prob_ant + prob_calc * (1 + prob_ant);
                        salida << prob_ant << " + " << prob_calc << " * (1 + " << prob_ant << ")" << endl;
                    } else {
                        prob_nueva = (prob_ant + prob_calc) / (1 - min(abs(prob_ant), abs(prob_calc)));
                        salida << "(" << prob_ant << " + " << prob_calc << ") / (1 - min(abs(" << prob_ant << "), abs(" << prob_calc << ")))" << endl;
                    }
                    buscar_hecho(meta)->setFC(prob_nueva);
                    salida << "Actualizada " << meta << " a la BH con FC = " << prob_nueva << endl;
                }
            }
        }
    }
    return verificado;
}

/**
 * \brief Función que llama a la función principal "verificar".
 * \param meta La meta inicial a comprobar.
 * \return Un booleano que indica si la meta se verifica o no.
*/
bool encadenamiento_hacia_atras(string meta, ofstream& salida) {
    if (verificar(meta, salida))
        return true;
    return false;
}

int main(int argc, char ** argv) {
    /** Comprobamos si el número de parámetros es correcto */
    if (argc != 3) {
        cerr << "ERROR: Número de parámetros incorrecto" << endl;
        cerr << "USO: " << argv[0] << "BaseConocimientos.txt BaseHechos.txt" << endl;
        return 1;
    }

    ifstream bh, bc;
    /** Abrimos la BC */
    bc.open(argv[1]);
    if (!bc) {
        cerr << "ERROR: El archivo " << argv[1] << " no se pudo abrir correctamente" << endl;
        return 2;
    }
    /** Abrimos la BH */
    bh.open(argv[2]);
    if (!bh) {
        cerr << "ERROR: El archivo " << argv[2] << " no se pudo abrir correctamente" << endl;
        return 3;
    }

    int numHechos, numConocimientos;
    string linea;
    bh >> numHechos;
    getline(bh, linea);
    /** Leemos la BH */
    for (int i = 0; i < numHechos; i++) {
        getline(bh, linea);
        vector<string> campos = separar_campos(linea, ", FC=");
        istringstream iss(campos[1].c_str());
        double valor;
        iss >> valor;
        Hecho aux(campos[0], valor);
        BH.push_back(aux);
    }
    getline(bh, linea);
    getline(bh, linea);
    vector<string> metas = separar_campos(linea, ", ");

    bc >> numConocimientos;
    getline(bc, linea);
    /** Leemos la BC */
    for (int i = 0; i < numConocimientos; i++) {
        getline(bc, linea);
        vector<string> campos = separar_campos(linea, ":");                 // campos[0] = id, campos[1] = resto
        vector<string> campos2 = separar_campos(campos[1], ", FC=");        // campos2[0] = regla, campos2[1] = fc
        vector<string> campos3 = separar_campos(campos2[0], " Entonces ");  // campos3[0] = cond. regla, campos3[1] = res
        string cond = campos3[0].substr(4);
        unsigned short int op = 0;
        string op1 = "";
        string op2 = "";
        string res = campos3[1];
        if (!hay_espacios(cond)) {
            op1 = cond;
        } else {
            vector<string> operacion = separar_campos(cond, " ");
            op1 = operacion[0];
            if (operacion[1] == "y") {
                op = 1;
                op2 = operacion[2];
            } else if (operacion[1] == "o") {
                op = 2;
                op2 = operacion[2];
            }

        }
        istringstream iss(campos2[1].c_str());
        double valor;
        iss >> valor;
        Regla aux(campos[0], valor, op, op1, op2, res);
        BC.push_back(aux);
    }

    ofstream salida("salida.txt");
    print_bases(salida);
    int numMetas = metas.size();

    if (numMetas == 1)
        salida << "Encontrada 1 meta" << endl;
    else
        salida << "Encontradas " << numMetas << " metas" << endl;

    for (int i = 0; i < (int) metas.size(); i++) {
        salida << "Objetivo " << i+1 << ": " << metas[i] << endl;
        if (encadenamiento_hacia_atras(metas[i], salida)) {
            Hecho * fin = buscar_hecho(metas[i]);
            salida << "El algoritmo SBR con encadenamiento hacia atrás verifica la meta " << metas[i] << " con FC = " << fin->getFC() << endl;
        } else
            salida << "El algoritmo SBR con encadenamiento hacia atrás NO verifica la meta " << metas[i] << endl;
    }
    return 0;
}
