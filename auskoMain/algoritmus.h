#pragma once
#include <vector>
#include <iostream>
#include "Citac.h"

class Algoritmus {
public:
    template <typename Predicate>
    static std::vector<Obec> filterObce(const std::vector<Obec>& obce, Predicate predikat) {
        std::vector<Obec> vysledok;
        for (const auto& obec : obce) {
            if (predikat(obec)) {
                vysledok.push_back(obec);
            }
        }
        return vysledok;
    }

    // Predik?ty ako met?dy
    static bool containsStr(const Obec& obec, const std::string& hladany) {
        return obec.name.find(hladany) != std::string::npos;
    }

    // Predik?t s maxim?lnym po?tom obyvate?ov a rokom
    static bool hasMaxResidents(const Obec& obec, int maxPocet, int rok) {
        if (rok == 2020) {
            return (obec.value1muzi + obec.value2zeny) <= maxPocet;
        }
        else if (rok == 2021) {
            return (obec.value3muzi + obec.value4zeny) <= maxPocet;
        }
        else if (rok == 2022) {
            return (obec.value5muzi + obec.value6zeny) <= maxPocet;
        }
        else if (rok == 2023) {
            return (obec.value7muzi + obec.value8zeny) <= maxPocet;
        }
        //2024
        return (obec.value9muzi + obec.value10zeny) <= maxPocet;
    }


    static bool hasMinResidents(const Obec& obec, int minPocet, int rok) {
        if (rok == 2020) {
            return (obec.value1muzi + obec.value2zeny) >= minPocet;
        }
        else if (rok == 2021) {
            return (obec.value3muzi + obec.value4zeny) >= minPocet;
        }
        else if (rok == 2022) {
            return (obec.value5muzi + obec.value6zeny) >= minPocet;
        }
        else if (rok == 2023) {
            return (obec.value7muzi + obec.value8zeny) >= minPocet;
        }
        //2024
        return (obec.value9muzi + obec.value10zeny) >= minPocet;
    }

    static bool hasType(const Obec& obec, TypUzemnejJednotky typ) {
        return obec.uzemnaJednotka == typ;
    }

    // Funkcia na v?pis obc?
    static void vypisObce(const std::vector<Obec>& obce) {
        for (const auto& obec : obce) {
            std::cout << "Meno obce: " << obec.name << '\n';
            std::cout << "ID: " << obec.id << '\n';
            std::cout << "Hodnota 1muzi za rok 2020: " << obec.value1muzi << '\n';
            std::cout << "Hodnota 2zeny za rok 2020: " << obec.value2zeny << '\n';
            std::cout << "Hodnota 3muzi za rok 2021: " << obec.value3muzi << '\n';
            std::cout << "Hodnota 4zeny za rok 2021: " << obec.value4zeny << '\n';
            std::cout << "Hodnota 5muzi za rok 2022: " << obec.value5muzi << '\n';
            std::cout << "Hodnota 6zeny za rok 2022: " << obec.value6zeny << '\n';
            std::cout << "Hodnota 7muzi za rok 2023: " << obec.value7muzi << '\n';
            std::cout << "Hodnota 8zeny za rok 2023: " << obec.value8zeny << '\n';
            std::cout << "Hodnota 9muzi za rok 2024: " << obec.value9muzi << '\n';
            std::cout << "Hodnota 10zeny za rok 2024: " << obec.value10zeny << '\n';
            std::cout << '\n';
        }
    }

};


