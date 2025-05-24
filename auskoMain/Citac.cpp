#include "Citac.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>

// Konstruktor triedy, ktor? prij?ma n?zov s?boru
Citac::Citac() {}


// Na??tanie ?dajov zo s?boru
bool Citac::loadData(bool hodnota, const std::string& nazov) {
    prvySubor = hodnota;
    // Nastavenie k?dovania konzoly na Windows-1250
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);


    std::ifstream file(nazov);  // Otvorenie s?boru

    if (!file.is_open()) {
        std::cerr << "Nepodarilo sa otvori? s?bor!" << std::endl;
        return false;
    }

    std::string line;

    int pocitadlo = 0;
    // Pre??tanie s?boru riadok po riadku
    while (std::getline(file, line)) {
        std::stringstream ss(line);

        // Rozdelenie riadku pod?a bodko?iarky a ulo?enie do ?trukt?ry Obec
        std::string namestr = "";
        std::string idstr = "";
        std::string value1str = "";
        std::string value2str = "";

        std::getline(ss, namestr, ';');
        std::getline(ss, idstr, ';');
        std::getline(ss, value1str, ';');
        std::getline(ss, value2str, ';');

        if (prvySubor)
        {
            Obec tempObec;
            tempObec.name = namestr;
            std::string cleanedId = removeBrackets(idstr);
            tempObec.uzemnaJednotka = TypUzemnejJednotky::Obec; //sem zadavam typ uzemnej jednotky
            tempObec.id = std::stoi(cleanedId);
            tempObec.value1muzi = std::stoi(value1str);
            tempObec.value2zeny = std::stoi(value2str);
            // Pridanie obce do zoznamu
            obce.push_back(tempObec);
        }
        else if (pocetNacitaniaSuborov == 1) {
            obce[pocitadlo].value3muzi = std::stoi(value1str);
            obce[pocitadlo].value4zeny = std::stoi(value2str);
        }
        else if (pocetNacitaniaSuborov == 2) {
            obce[pocitadlo].value5muzi = std::stoi(value1str);
            obce[pocitadlo].value6zeny = std::stoi(value2str);
        }
        else if (pocetNacitaniaSuborov == 3) {
            obce[pocitadlo].value7muzi = std::stoi(value1str);
            obce[pocitadlo].value8zeny = std::stoi(value2str);
        }
        else if (pocetNacitaniaSuborov == 4) {
            obce[pocitadlo].value9muzi = std::stoi(value1str);
            obce[pocitadlo].value10zeny = std::stoi(value2str);
        }
        pocitadlo++;

    }

    file.close();  // Zatvorenie s?boru
    this->pocetNacitaniaSuborov++; //aby som vedel kolky subor v poradi nacitavam
    return true;
}

// Z?skanie v?etk?ch na??tan?ch obc?
const std::vector<Obec>& Citac::getObce() const {
    return obce;
}
// Z?skanie v?etk?ch na??tan?ch obc? bez const
std::vector<Obec>& Citac::getObcebezconst() {
    return obce;
}


std::string Citac::removeBrackets(const std::string& str) {
    if (str.size() >= 2 && str.front() == '<' && str.back() == '>') {
        return str.substr(1, str.size() - 2); // Odstr?ni prv? a posledn? znak
    }
    return str; // Ak nem? zob??iky, vr?ti p?vodn? string
}



int Citac::spocitajCelkovoObyvatelov( std::vector<Obec>& obce) {
    int celkovyPocet = 0;
    for (const auto& obec : obce) {
        celkovyPocet += obec.value1muzi + obec.value2zeny;
        celkovyPocet += obec.value3muzi + obec.value4zeny;
        celkovyPocet += obec.value5muzi + obec.value6zeny;
        celkovyPocet += obec.value7muzi + obec.value8zeny;
        celkovyPocet += obec.value9muzi + obec.value10zeny;
    }
    return celkovyPocet;
}

