#include "hecho.h"

Hecho::Hecho() {
    id = "";
    fc = 0.0;
}

Hecho::Hecho(const string& nid, const double& nfc) {
    id = nid;
    fc = nfc;
}

string Hecho::getID() { return id; }

void Hecho::setFC(const double& nfc) { fc = nfc; }

double Hecho::getFC() { return fc; }
