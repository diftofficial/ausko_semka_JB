#include <iostream>
#include "Citac.h"
#include "algoritmus.h"
#include <stack>
#include "hierarchia.h"
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>


int main() {
    // zapni debug heap + dump pri ukončení
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    // Vytvorenie objektu Citac a na??tanie ?dajov zo s?
    {
    //skusobny memory leak
    //int* leakyInt = new int(123);
    Citac citac;
    
    if (!citac.loadData(true, "2020.csv")) {
        return 1;  // Ak sa nepodar? na??ta? d?ta, ukon?ime program
    }
    if (!citac.loadData(false, "2021.csv")) {
        return 1;  // Ak sa nepodar? na??ta? d?ta, ukon?ime program
    }
    if (!citac.loadData(false, "2022.csv")) {
        return 1;  // Ak sa nepodar? na??ta? d?ta, ukon?ime program
    }
    if (!citac.loadData(false, "2023.csv")) {
        return 1;  // Ak sa nepodar? na??ta? d?ta, ukon?ime program
    }
    if (!citac.loadData(false, "2024.csv")) {
        return 1;  // Ak sa nepodar? na??ta? d?ta, ukon?ime program
    }
    // Definovanie n?zvu, ktory chceme hladat
    std::string hladanyNazov = "Pöggstall";

    int maxPocet = 100;
    int rokPreMaxPocet = 2023;

    int minPocet = 10000000;
    int rokPreMinPocet = 2023;

    const std::vector<Obec>& obce = citac.getObce();
    std::vector<Obec>& obce3= citac.getObcebezconst();

    int celkovy = citac.spocitajCelkovoObyvatelov(obce3);
    


    // Filtrovanie pod?a n?zvu
    std::vector<Obec> filtrovaneObce = Algoritmus::filterObce(obce, [&hladanyNazov](const Obec& o) { return Algoritmus::containsStr(o, hladanyNazov); });
    std::cout << "Obce obsahujuce:" << hladanyNazov << "\n";
    Algoritmus::vypisObce(filtrovaneObce);

    // Filtrovanie pod?a maxim?lneho po?tu obyvate?ov
    filtrovaneObce = Algoritmus::filterObce(obce, [maxPocet, rokPreMaxPocet](const Obec& o) { return Algoritmus::hasMaxResidents(o, maxPocet, rokPreMaxPocet); });
    std::cout << "Obce s max. obyvatelmi:" << maxPocet << "za rok:" << rokPreMaxPocet <<"\n";
    Algoritmus::vypisObce(filtrovaneObce);

    //Filtrovanie pod?a minim?lneho po?tu obyvate?ov
    filtrovaneObce = Algoritmus::filterObce(obce, [minPocet, rokPreMinPocet](const Obec& o) { return Algoritmus::hasMinResidents(o, minPocet, rokPreMinPocet); });
    std::cout << "Obce s min. obyvatelmi:" << minPocet << "za rok:" << rokPreMinPocet << "\n";
    Algoritmus::vypisObce(filtrovaneObce);

    std::vector<Obec>& Obce = citac.getObcebezconst();
    
    //2cast semestralky
    hierarchia hierarchia;
    hierarchia.doplnKodyObciam("obce.csv", Obce);
    hierarchia.nacitajStukturuObci("uzemie.csv", Obce);
    hierarchia.nacitajObceDoStromu(Obce);
    hierarchia.vypocitajAkumulativneHodnoty(hierarchia.getStrom()->accessRoot());

    //hierarchia.vypisStrom();
    // Aktualizácia kumulatívnych údajov
    // Aktualizácia kumulatívnych údajov
    std::cout << "Aktualizujem kumulativne udaje...\n";
    hierarchia.aktualizujKumulativneUdaje();
    
    std::cout << "Kumulativne udaje boli aktualizovane.\n";
    

    // Do?asný výpis hierarchie na kontrolu
    std::cout << "\n=== Hierarchia po aktualizacii ===\n";
    //hierarchia.vypisStrom();

    // Inicializácia aktuálneho uzla na kore? (Rakúsko)
    ds::amt::MultiWayExplicitHierarchyBlock<Obec>* currentNode = hierarchia.getStrom()->accessRoot();
    if (!currentNode) {
        std::cerr << "Hierarchia je prazdna.\n";
        return 1;
    }
    //3 uroven tabulka
    hierarchia.indexAllNodes();
    hierarchia.printTables();
    
    
    
    
    
    // Hlavné menu konzolovej aplikácie
    //std::cout << "Celkovo obyvatelov vo vsetkych obciach (2020–2024): " << celkovy << std::endl;
    int volba;
    do {
        std::cout << "\n=== Hlavne menu ===\n";
        std::cout << "Aktualny uzol: " << currentNode->data_.name << " (kod: " << currentNode->data_.kod << ", typ: " << static_cast<int>(currentNode->data_.uzemnaJednotka) << ")\n";
        std::cout << "1. Ist hore hierarchiou\n";
        std::cout << "2. Ist dole hierarchiou\n";
        std::cout << "3. Vypis aktualny uzol s predikatom\n";
        std::cout << "4. Prehladavanie tabulky\n";
        std::cout << "5. Koniec\n";
        std::cout << "Vyber volbu: ";
        std::cin >> volba;

        if (volba == 1) {
            // Ís? hore hierarchiou (k rodi?ovi)
            auto* parent = hierarchia.getStrom()->accessParent(*currentNode);
            if (parent) {
                currentNode = parent;
                std::cout << "Presun na rodica: " << currentNode->data_.name << " (kod: " << currentNode->data_.kod << ")\n";
            }
            else {
                std::cout << "Uzol je koren, nema rodica.\n";
            }
        }
        else if (volba == 2) {
            // Ís? dole hierarchiou (k synovi)
            if (currentNode->sons_ && !currentNode->sons_->isEmpty()) {
                std::cout << "Synovia uzla " << currentNode->data_.name << ":\n";
                int index = 0;
                for (auto it = currentNode->sons_->begin(); it != currentNode->sons_->end(); ++it, ++index) {
                    auto* son = *it;
                    std::cout << index << ": " << son->data_.name << " (kod: " << son->data_.kod << ")\n";
                }
                std::cout << "Vyber syna (index): ";
                int sonIndex;
                std::cin >> sonIndex;
                if (sonIndex >= 0 && sonIndex < currentNode->sons_->size()) {
                    // Použitie iterátora na získanie správneho syna
                    auto it = currentNode->sons_->begin();
                    for (int i = 0; i < sonIndex; ++i) {
                        ++it;
                    }
                    currentNode = *it;
                    std::cout << "Presun na syna: " << currentNode->data_.name << " (kod: " << currentNode->data_.kod << ")\n";
                }
                else {
                    std::cout << "Neplatny index syna.\n";
                }
            }
            else {
                std::cout << "Uzol nema synov.\n";
            }
        }
        else if (volba == 3) {
            // Výpis uzla a podstromu s predikátom
            int predikat;
            std::cout << "Vyber predikat:\n";
            std::cout << "1. containsStr\n";
            std::cout << "2. hasMaxResidence\n";
            std::cout << "3. hasMinResidence\n";
            std::cout << "4. hasType\n";
            std::cout << "5. Ziadny predikat\n";
            std::cout << "Vyber predikat: ";
            std::cin >> predikat;

            // Funkcia na výpis uzla
            auto vypisUzol = [](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                std::cout << "Uzol: " << node->data_.name << ", Kod: " << node->data_.kod
                    << ", Typ: " << static_cast<int>(node->data_.uzemnaJednotka);
                if (node->data_.uzemnaJednotka != TypUzemnejJednotky::Obec) {
                    std::cout << ", 2020: " << (node->data_.akumulativnavalue1muzi + node->data_.akumulativnavalue2zeny)
                        << " (muži: " << node->data_.akumulativnavalue1muzi << ", ženy: " << node->data_.akumulativnavalue2zeny << ")"
                        << ", 2021: " << (node->data_.akumulativnavalue3muzi + node->data_.akumulativnavalue4zeny)
                        << " (muži: " << node->data_.akumulativnavalue3muzi << ", ženy: " << node->data_.akumulativnavalue4zeny << ")"
                        << ", 2022: " << (node->data_.akumulativnavalue5muzi + node->data_.akumulativnavalue6zeny)
                        << " (muži: " << node->data_.akumulativnavalue5muzi << ", ženy: " << node->data_.akumulativnavalue6zeny << ")"
                        << ", 2023: " << (node->data_.akumulativnavalue7muzi + node->data_.akumulativnavalue8zeny)
                        << " (muži: " << node->data_.akumulativnavalue7muzi << ", ženy: " << node->data_.akumulativnavalue8zeny << ")"
                        << ", 2024: " << (node->data_.akumulativnavalue9muzi + node->data_.akumulativnavalue10zeny)
                        << " (muži: " << node->data_.akumulativnavalue9muzi << ", ženy: " << node->data_.akumulativnavalue10zeny << ")"
                        << "\n";
                }
                else {
                    std::cout
                        << ", 2020: " << (node->data_.value1muzi + node->data_.value2zeny)
                        << " (muži: " << node->data_.value1muzi << ", ženy: " << node->data_.value2zeny << ")"
                        << ", 2021: " << (node->data_.value3muzi + node->data_.value4zeny)
                        << " (muži: " << node->data_.value3muzi << ", ženy: " << node->data_.value4zeny << ")"
                        << ", 2022: " << (node->data_.value5muzi + node->data_.value6zeny)
                        << " (muži: " << node->data_.value5muzi << ", ženy: " << node->data_.value6zeny << ")"
                        << ", 2023: " << (node->data_.value7muzi + node->data_.value8zeny)
                        << " (muži: " << node->data_.value7muzi << ", ženy: " << node->data_.value8zeny << ")"
                        << ", 2024: " << (node->data_.value9muzi + node->data_.value10zeny)
                        << " (muži: " << node->data_.value9muzi << ", ženy: " << node->data_.value10zeny << ")"
                        << "\n";
                }
                };

            // Funkcia na rekurzívne prechádzanie podstromu a aplikáciu predikátu
            std::function<void(ds::amt::MultiWayExplicitHierarchyBlock<Obec>*, std::function<bool(const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*)>)> aplikujPredikatNaPodstrom;
            aplikujPredikatNaPodstrom = [&aplikujPredikatNaPodstrom, &vypisUzol](ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, std::function<bool(const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*)> predikat) {
                if (!node) return;

                // Aplikujeme predikát na aktuálny uzol
                if (predikat(node)) {
                    vypisUzol(node);
                }

                // Rekurzívne prejdeme synov
                if (node->sons_ && !node->sons_->isEmpty()) {
                    for (auto it = node->sons_->begin(); it != node->sons_->end(); ++it) {
                        aplikujPredikatNaPodstrom(*it, predikat);
                    }
                }
                };

            // Definícia predikátu
            std::function<bool(const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*)> predikatFunkcia;
            if (predikat == 1) {
                // containsStr
                std::string retazec;
                std::cout << "Zadaj retazec na vyhladanie: ";
                std::cin.ignore();
                std::getline(std::cin, retazec);
                predikatFunkcia = [retazec](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    return node->data_.name.find(retazec) != std::string::npos;
                    };
            }
            else if (predikat == 2) {
                // hasMaxResidence
                int maxPocet, rok;
                std::cout << "Zadaj maximalny pocet obyvatelov: ";
                std::cin >> maxPocet;
                std::cout << "Zadaj rok (2020-2024): ";
                std::cin >> rok;
                if (rok < 2020 || rok > 2024) {
                    std::cout << "Neplatny rok.\n";
                    continue;
                }
                predikatFunkcia = [maxPocet, rok](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    int pocet = 0;
                    if (rok == 2020) pocet = node->data_.value1muzi + node->data_.value2zeny;
                    else if (rok == 2021) pocet = node->data_.value3muzi + node->data_.value4zeny;
                    else if (rok == 2022) pocet = node->data_.value5muzi + node->data_.value6zeny;
                    else if (rok == 2023) pocet = node->data_.value7muzi + node->data_.value8zeny;
                    else if (rok == 2024) pocet = node->data_.value9muzi + node->data_.value10zeny;
                    return pocet <= maxPocet;
                    };
            }
            else if (predikat == 3) {
                // hasMinResidence
                int minPocet, rok;
                std::cout << "Zadaj minimalny pocet obyvatelov: ";
                std::cin >> minPocet;
                std::cout << "Zadaj rok (2020-2024): ";
                std::cin >> rok;
                if (rok < 2020 || rok > 2024) {
                    std::cout << "Neplatny rok.\n";
                    continue;
                }
                predikatFunkcia = [minPocet, rok](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    int pocet = 0;
                    if (rok == 2020) pocet = node->data_.value1muzi + node->data_.value2zeny;
                    else if (rok == 2021) pocet = node->data_.value3muzi + node->data_.value4zeny;
                    else if (rok == 2022) pocet = node->data_.value5muzi + node->data_.value6zeny;
                    else if (rok == 2023) pocet = node->data_.value7muzi + node->data_.value8zeny;
                    else if (rok == 2024) pocet = node->data_.value9muzi + node->data_.value10zeny;
                    return pocet >= minPocet;
                    };
            }
            else if (predikat == 4) {
                // hasType
                int typ;
                std::cout << "Zadaj typ uzemnej jednotky (0=Rakusko, 1=GeografickaPoloha, 2=Republika, 3=Region, 4=Obec): ";
                std::cin >> typ;
                if (typ < 0 || typ > 4) {
                    std::cout << "Neplatny typ uzemnej jednotky.\n";
                    continue;
                }
                predikatFunkcia = [typ](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    return static_cast<int>(node->data_.uzemnaJednotka) == typ;
                    };
            }
            else if (predikat == 5) {
                // Žiadny predikát
                predikatFunkcia = [](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*) { return true; };
            }
            else {
                std::cout << "Neplatny predikat.\n";
                continue;
            }

            // Aplikácia predikátu na celý podstrom
            aplikujPredikatNaPodstrom(currentNode, predikatFunkcia);
        }
        else if (volba == 4) {
            std::cout << "Vyber typ uzemnej jednotky:\n"
                << " 0 = Obec\n"
                << " 1 = Region\n"
                << " 2 = Republika\n"
                << " 3 = GeografickaPoloha\n"
                << " 4 = Rakusko\n"
                << "Zadaj cislo [0–4]: ";

            int choice;
            std::cin >> choice;
            TypUzemnejJednotky type;

            switch (choice) {
            case 0: type = TypUzemnejJednotky::Obec; break;
            case 1: type = TypUzemnejJednotky::Region; break;
            case 2: type = TypUzemnejJednotky::Republika; break;
            case 3: type = TypUzemnejJednotky::GeografickaPoloha; break;
            case 4: type = TypUzemnejJednotky::Rakusko; break;
            default:
                std::cerr << "Neplatna volba!\n";
                return 1;
            }
            //auto type = static_cast<TypUzemnejJednotky>(choice);

            // 2) Zbavíme sa newline z input‐buffra, aby getline fungoval
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // 3) Prečítame názov
            std::cout << "Zadaj nazov uzemia: ";
            std::string name;
            std::getline(std::cin, name);

            // 4) Teraz už môžeme hľadať
            auto results = hierarchia.findAll(name, type);

            // 5) Vypíšeme výsledky
            if (results.empty()) {
                std::cout << "Nenajdene!\n";
            }
            else {
                std::cout << "Nájdené " << results.size() << " uzly:\n";
                for (auto node : results) {
                    // predpokladám, že node je ukazovateľ na blok, takže:
                    std::cout << " • " << node->data_.name
                        << "  (kod=" << node->data_.kod << ")\n";
                }
            }

            
        }
        else if (volba == 5) {
            std::cout << "Koniec programu.\n";
        }
        else {
            std::cout << "Neplatna volba, skus znova.\n";
        }
    } while (volba != 5);
    //hierarchia.clearHierarchy();
    

    // Ziskanie a vypis vsetkych obci
    //const std::vector<Obec>& obce = citac.getObce();
    /**
    for (const auto& obec : obce) {
        std::cout << "Meno obce: " << obec.name << std::endl;
        std::cout << "ID: " << obec.id << std::endl;
        std::cout << "KOD: " << obec.kod << std::endl;
        std::cout << "Hodnota 1muzi: " << obec.value1muzi << std::endl;
        std::cout << "Hodnota 2zeny: " << obec.value2zeny << std::endl;
        std::cout << "Hodnota 3muzi: " << obec.value3muzi << std::endl;
        std::cout << "Hodnota 4zeny: " << obec.value4zeny << std::endl;
        std::cout << "Hodnota 5muzi: " << obec.value5muzi << std::endl;
        std::cout << "Hodnota 6zeny: " << obec.value6zeny << std::endl;
        std::cout << "Hodnota 7muzi: " << obec.value7muzi << std::endl;
        std::cout << "Hodnota 8zeny: " << obec.value8zeny << std::endl;
        std::cout << "Hodnota 9muzi: " << obec.value9muzi << std::endl;
        std::cout << "Hodnota 10zeny: " << obec.value10zeny << std::endl;
        std::cout << std::endl;
    }**/
    //3 CAST semestralky----------------------------------------------------
    
    // namiesto jednej find() teraz voláme findAll()
    // interaktívny vstup
    // nájdi všetky duplicitné uzly s daným menom a typom
    

    
    auto results = hierarchia.findAll("Warth", TypUzemnejJednotky::Obec);
    if (results.empty()) {
        std::cout << "Nenájdené!\n";
    }
    else {
        std::cout << "Nájdené " << results.size() << " uzly:\n";
        for (auto node : results) {
            std::cout
                << " • " << node->data_.name
                << " (kod=" << node->data_.kod << ")\n";
        }
    }

    } //ukoncivaci 
    _CrtDumpMemoryLeaks();
    return 0;
    
}


