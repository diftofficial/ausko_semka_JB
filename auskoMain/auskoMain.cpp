#include <iostream>
#include "Citac.h"
#include "algoritmus.h"
#include <stack>
#include "hierarchia.h"
#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

#include "AlphabeticalComparator.h"
#include "PopulationComparator.h"

#include <libds/adt/sorts.h>            // QuickSort :contentReference[oaicite:0]{index=0}
#include <libds/amt/implicit_sequence.h> // ImplicitSequence :contentReference[oaicite:1]{index=1}



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
   // hierarchia.printTables();
    
    
    
    
    
    // Hlavné menu konzolovej aplikácie
    //std::cout << "Celkovo obyvatelov vo vsetkych obciach (2020–2024): " << celkovy << std::endl;
    int volba;
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
                << "\n"
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
                << "\n"
                << "\n";
        }
        };
    do {
        hierarchia::NodeList myList;
        myList.clear();

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

            

            // Funkcia na rekurzívne prechádzanie podstromu a aplikáciu predikátu
            std::function<
                void(ds::amt::MultiWayExplicitHierarchyBlock<Obec>*,
                    std::function<bool(const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*)>)
            > aplikujPredikatNaPodstrom;

            aplikujPredikatNaPodstrom =
                [&aplikujPredikatNaPodstrom, &vypisUzol, &myList]
                (ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node,
                    const std::function<bool(const ds::amt::MultiWayExplicitHierarchyBlock<Obec>*)>& predikat)
                {
                    if (!node) return;

                    if (predikat(node)) {
                        // 3a) vypíšeme uzol
                        vypisUzol(node);
                        // 3b) a zároveň ho ukladáme
                        myList.push_back(node);
                    }

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
            continue;

            
        }
        else if (volba == 5) {
            std::cout << "Koniec programu.\n";
            break;
        }
        else {
            std::cout << "Neplatna volba, skus znova.\n";
        }

        // alias for readability
        using NodePtr = ds::amt::MultiWayExplicitHierarchyBlock<Obec>*;

        // 1) Build the ImplicitSequence
        ds::amt::ImplicitSequence<NodePtr> seq;
        if (myList.size() > 0) {
            seq.reserveCapacity(myList.size());
        }
        // reserve exact capacity

        
        // no reserveCapacity()

       
        

        int rozhodnutieKomparator;

        while (true) {
            std::cout << "aky chces pouzit komparator:\n"
                << " 0 = ziadny\n"
                << " 1 = abecedne\n"
                << " 2 = populacie\n"
                << "Zadaj cislo [0–2]: ";
            std::cin >> rozhodnutieKomparator;

            // if user typed something that's not an int, clear and retry
            if (!std::cin) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                continue;
            }

            if (rozhodnutieKomparator < 0 || rozhodnutieKomparator > 2) {
                std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                continue;   // jump back and re-prompt
            }

            // valid input 0–2, break out of the loop
            break;
        }

        // here rozhodnutieKomparator is guaranteed 0,1 or 2
        if (rozhodnutieKomparator == 0) {
            continue;
        }
        else if (rozhodnutieKomparator == 1) {
            for (auto it = myList.begin(); it != myList.end(); ++it) {
                auto& blk = seq.insertLast();
                blk.data_ = *it;
            }

            // 2) Sort it with QuickSort + your CompareNodeByName
            ds::adt::QuickSort<NodePtr> sorter;              // QuickSort<T> :contentReference[oaicite:1]{index=1}
            CompareNodeByName cmp;
            sorter.sort(seq,
                [&](NodePtr a, NodePtr b) {
                    return cmp(a, b) < 0;   // –1 means a < b
                }
            );

            // 3) Write the newly‐sorted pointers back into your SimpleList
            myList.clear();
            for (size_t i = 0; i < seq.size(); ++i) {
                myList.push_back(seq.access(i)->data_);
            }

            // 4) Print them in alphabetical order
            std::cout << "\n";
            std::cout << "Zoradené uzly abecedne:\n";
            std::cout << "\n";
            for (auto ptr = myList.begin(); ptr != myList.end(); ++ptr) {
                vypisUzol(*ptr);
            }
        }
        else { // ==2

            int rozhodnutieKtoryRok;

            while (true) {
                
                std::cout << "pre aky rok:\n"
                    << " 0 = 2020\n"
                    << " 1 = 2021\n"
                    << " 2 = 2022\n"
                    << " 3 = 2023\n"
                    << " 4 = 2024\n"
                    << "Zadaj cislo [0–4]: ";
                std::cin >> rozhodnutieKtoryRok;

                // if user typed something that's not an int, clear and retry
                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                    continue;
                }

                if (rozhodnutieKtoryRok < 0 || rozhodnutieKtoryRok > 4) {
                    std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                    continue;   // jump back and re-prompt
                }

                // valid input 0–4, break out of the loop
                break;
            }
            int selectedYear = 2020;
            if (rozhodnutieKtoryRok==0)
            {
                selectedYear = 2020;
            }else if(rozhodnutieKtoryRok == 1) {
                selectedYear = 2021;
            }
            else if (rozhodnutieKtoryRok == 2) {
                selectedYear = 2022;
            }
            else if (rozhodnutieKtoryRok == 3) {
                selectedYear = 2023;
            }
            else if (rozhodnutieKtoryRok == 4) {
                selectedYear = 2024;
            }



            int rozhodnutieKriteria;

            while (true) {

                std::cout << "podla akeho kriteria:\n"
                    << " 0 = SPOLU\n"
                    << " 1 = muzi\n"
                    << " 2 = zeny\n"
                    << "Zadaj cislo [0–2]: ";
                std::cin >> rozhodnutieKriteria;

                // if user typed something that's not an int, clear and retry
                if (!std::cin) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                    continue;
                }

                if (rozhodnutieKtoryRok < 0 || rozhodnutieKtoryRok > 2) {
                    std::cout << "Zle zadane cislo. Skus znovu.\n\n";
                    continue;   // jump back and re-prompt
                }

                // valid input 0–4, break out of the loop
                break;
            }

            // Build sequence
            ds::amt::ImplicitSequence<NodePtr> seqPopulation;
            for (auto it = myList.begin(); it != myList.end(); ++it) {
                auto& blk = seqPopulation.insertLast();
                blk.data_ = *it;
            }
            ds::adt::QuickSort<NodePtr> sorterPopulation;
            
            if (rozhodnutieKriteria == 0)
            {
                // Sort by population
                
                CompareByPopulation popCmp(selectedYear, CompareByPopulation::Gender::SPOLU);
                sorterPopulation.sort(seqPopulation,
                    [&](NodePtr a, NodePtr b) {
                        return popCmp(a, b) < 0;  // –1 means a<b
                    }
                );
            }
            else if (rozhodnutieKriteria == 1) {
                // Sort by population
                
                CompareByPopulation popCmp(selectedYear, CompareByPopulation::Gender::MUZI);
                sorterPopulation.sort(seqPopulation,
                    [&](NodePtr a, NodePtr b) {
                        return popCmp(a, b) < 0;  // –1 means a<b
                    }
                );
            }
            else if (rozhodnutieKriteria == 2) {
                // Sort by population
                
                CompareByPopulation popCmp(selectedYear, CompareByPopulation::Gender::ZENY);
                sorterPopulation.sort(seqPopulation,
                    [&](NodePtr a, NodePtr b) {
                        return popCmp(a, b) < 0;  // –1 means a<b
                    }
                );

            }
            

            

            // Write back & print
            myList.clear();
            for (size_t i = 0; i < seqPopulation.size(); ++i)
                myList.push_back(seqPopulation.access(i)->data_);

            std::cout << "\n";
            std::cout << "Zoradené uzly podla populacie za rok" << selectedYear << ":\n";
            std::cout << "\n";
            for (auto p : myList)
                vypisUzol(p);
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
    }
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
    }**/
    } 
    _CrtDumpMemoryLeaks();
    return 0;
    
}


