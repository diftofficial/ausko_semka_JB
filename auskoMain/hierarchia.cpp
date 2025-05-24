#include "hierarchia.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "algoritmus.h"
#include <stack>

#include <libds/amt/explicit_hierarchy.h>  // pre MultiWayExplicitHierarchyBlock
#include <libds/amt/abstract_memory_type.h> // pre AbstractMemoryStructure

hierarchia::hierarchia()
{
    // 1) build all the empty tables
    initTables();
   
    strom = new ds::amt::MultiWayExplicitHierarchy<Obec>();

    // Koreň: Rakúsko
    auto& koren = strom->emplaceRoot();
    koren.data_.name = "Rakusko";
    koren.data_.uzemnaJednotka = TypUzemnejJednotky::Rakusko;
    koren.data_.kod = 0; // Koreň nemá kód

}

hierarchia::~hierarchia()
{
    /**
    byType.clear();
    if (strom) {
        // 1) Zavoláme deštruktory blokov (post‐order), aby sa uvoľnili interné std::string atď.
        using BlockType = ds::amt::MultiWayExplicitHierarchyBlock<Obec>;
        strom->processPostOrder(strom->accessRoot(), [](BlockType* b) {
            b->~BlockType();
            });
        std::cout << "Cistim strom..." << std::endl;
        strom->clear();
        std::cout << "Strom uvolneny." << std::endl;
        // 2) Teraz clear() v knižnici uvoľní surovú pamäť blokov
        

        // 3) A už môžeme zničiť celú štruktúru
        delete strom;
        strom = nullptr;
    }
    //delete strom;
    //strom = nullptr;
    **/

    byType.clear();

    if (strom) {
        // Let the library clean up all nodes correctly
        strom->clear();
        delete strom;
        strom = nullptr;
    }
}

void hierarchia::naplnenieHierarchie()
{
}

void hierarchia::doplnKodyObciam(const std::string& filename, std::vector<Obec>& obce) {

    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    std::ifstream file(filename);
    std::string line;

    //  Vytvor?me mapu ID -> ukazate? na Obec
    std::unordered_map<int, Obec*> mapaObciPodlaID;
    for (auto& obec : obce) {
        mapaObciPodlaID[obec.id] = &obec;
    }

    // Spracovanie s?boru s k?dmi
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string namestr, idstr, kodstr;

        std::getline(ss, namestr, ';');
        std::getline(ss, idstr, ';');
        std::getline(ss, kodstr, ';');

        //potrebujeme sa zbavit tych <>
        std::string cleanedId = hierarchia::removeBrackets(idstr);
        int id = std::stoi(cleanedId);
        //potrebujeme sa zbavit aj tych <> ale aj AT , takze ide prec toto <AT  >
        //std::string cleanedKod = hierarchia::removeBrackets(kodstr);
        std::string cleanedCleanedKod = hierarchia::removeAT(kodstr);
        int kod = std::stoi(cleanedCleanedKod);

        // R?chle doh?adanie obce pod?a ID
        auto it = mapaObciPodlaID.find(id);
        if (it != mapaObciPodlaID.end()) {
            it->second->kod = kod;
        }
    }
}



void hierarchia::nacitajStukturuObci(const std::string& filename, std::vector<Obec>& obce)
{
    SetConsoleCP(1250);
    SetConsoleOutputCP(1250);

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Nepodarilo sa otvorit subor: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) {
            std::cerr << "Prazdny riadok v subore!" << std::endl;
            continue;
        }

        std::stringstream ss(line);
        std::string nazov, idKorena;

        if (!std::getline(ss, nazov, ';') || !std::getline(ss, idKorena, ';')) {
            std::cerr << "Chybny format riadku: " << line << std::endl;
            continue;
        }

        std::string cleanedId = removeBrackets(idKorena);
        std::string cleanedKod = removeAT(cleanedId);

        try {
            int kod = std::stoi(cleanedKod);

            // Určenie úrovne podľa dĺžky kódu
            TypUzemnejJednotky typ;
            ds::amt::MultiWayExplicitHierarchyBlock<Obec>* parent = nullptr;
            if (cleanedId.length() == 3) { // Napr. AT1, AT2, AT3
                typ = TypUzemnejJednotky::GeografickaPoloha;
                parent = strom->accessRoot(); // Rodič je koreň
            }
            else if (cleanedId.length() == 4) { // Napr. AT11, AT12, AT21
                typ = TypUzemnejJednotky::Republika;
                // Nájdi rodiča (napr. AT1 pre AT11)
                std::string parentKod = cleanedId.substr(0, 3);
                strom->processPreOrder(strom->accessRoot(), [&](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    if (node->data_.kod == std::stoi(removeAT(parentKod))) {
                        parent = const_cast<ds::amt::MultiWayExplicitHierarchyBlock<Obec>*>(node);
                    }
                    });
            }
            else if (cleanedId.length() == 5) { // Napr. AT111, AT112
                typ = TypUzemnejJednotky::Region;
                // Nájdi rodiča (napr. AT11 pre AT111)
                std::string parentKod = cleanedId.substr(0, 4);
                strom->processPreOrder(strom->accessRoot(), [&](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
                    if (node->data_.kod == std::stoi(removeAT(parentKod))) {
                        parent = const_cast<ds::amt::MultiWayExplicitHierarchyBlock<Obec>*>(node);
                    }
                    });
            }
            else {
                std::cerr << "Neznamy format kodu: " << cleanedId << std::endl;
                continue;
            }

            if (!parent) {
                std::cerr << "Rodicsky uzol nebol najdeny pre: " << nazov << " s kodom: " << cleanedId << std::endl;
                continue;
            }

            // Pridaj nový uzol
            auto& novyUzel = strom->emplaceSon(*parent, parent->sons_->size());
            novyUzel.data_.name = nazov;
            novyUzel.data_.kod = kod;
            novyUzel.data_.uzemnaJednotka = typ;
            novyUzel.data_.akumulativnavalue1muzi = 0;  // Hodnota 1 muzi
            novyUzel.data_.akumulativnavalue2zeny = 0;  // Hodnota 2 zeny
            novyUzel.data_.akumulativnavalue3muzi = 0;  // Hodnota 3 muzi
            novyUzel.data_.akumulativnavalue4zeny = 0;  // Hodnota 4 zeny
            novyUzel.data_.akumulativnavalue5muzi = 0;  // Hodnota 5 muzi
            novyUzel.data_.akumulativnavalue6zeny = 0;  // Hodnota 6 zeny
            novyUzel.data_.akumulativnavalue7muzi = 0;  // Hodnota 7 muzi
            novyUzel.data_.akumulativnavalue8zeny = 0;  // Hodnota 8 zeny
            novyUzel.data_.akumulativnavalue9muzi = 0;  // Hodnota 9 muzi
            novyUzel.data_.akumulativnavalue10zeny = 0;  // Hodnota 10 zeny

            //std::cout << "Pridany uzol: " << nazov << " s kodom: " << kod << " pod: " << parent->data_.name << std::endl;

        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Chyba: Neplatny format kodu v riadku: " << line << std::endl;
            continue;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Chyba: Kod mimo rozsah v riadku: " << line << std::endl;
            continue;
        }
    }

    file.close();
    
}

void hierarchia::vypocitajAkumulativneHodnoty(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* uzol)
{
    if (!uzol) return;

    // Reset akumulovaných hodnôt pre každý uzol
    uzol->data_.akumulativnavalue1muzi = 0;
    uzol->data_.akumulativnavalue2zeny = 0;
    uzol->data_.akumulativnavalue3muzi = 0;
    uzol->data_.akumulativnavalue4zeny = 0;
    uzol->data_.akumulativnavalue5muzi = 0;
    uzol->data_.akumulativnavalue6zeny = 0;
    uzol->data_.akumulativnavalue7muzi = 0;
    uzol->data_.akumulativnavalue8zeny = 0;
    uzol->data_.akumulativnavalue9muzi = 0;
    uzol->data_.akumulativnavalue10zeny = 0;

    if (uzol->sons_ != nullptr) {
        for (auto* potomok : *(uzol->sons_)) {
            vypocitajAkumulativneHodnoty(potomok);

            // Sčítame hodnoty iba z obcí (listy) – nie z vyšších úrovní
            if (potomok->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
                uzol->data_.akumulativnavalue1muzi += potomok->data_.value1muzi;
                uzol->data_.akumulativnavalue2zeny += potomok->data_.value2zeny;
                uzol->data_.akumulativnavalue3muzi += potomok->data_.value3muzi;
                uzol->data_.akumulativnavalue4zeny += potomok->data_.value4zeny;
                uzol->data_.akumulativnavalue5muzi += potomok->data_.value5muzi;
                uzol->data_.akumulativnavalue6zeny += potomok->data_.value6zeny;
                uzol->data_.akumulativnavalue7muzi += potomok->data_.value7muzi;
                uzol->data_.akumulativnavalue8zeny += potomok->data_.value8zeny;
                uzol->data_.akumulativnavalue9muzi += potomok->data_.value9muzi;
                uzol->data_.akumulativnavalue10zeny += potomok->data_.value10zeny;
            }
            else {
                // Pripočítame akumulované hodnoty potomka, ak to nie je obec
                uzol->data_.akumulativnavalue1muzi += potomok->data_.akumulativnavalue1muzi;
                uzol->data_.akumulativnavalue2zeny += potomok->data_.akumulativnavalue2zeny;
                uzol->data_.akumulativnavalue3muzi += potomok->data_.akumulativnavalue3muzi;
                uzol->data_.akumulativnavalue4zeny += potomok->data_.akumulativnavalue4zeny;
                uzol->data_.akumulativnavalue5muzi += potomok->data_.akumulativnavalue5muzi;
                uzol->data_.akumulativnavalue6zeny += potomok->data_.akumulativnavalue6zeny;
                uzol->data_.akumulativnavalue7muzi += potomok->data_.akumulativnavalue7muzi;
                uzol->data_.akumulativnavalue8zeny += potomok->data_.akumulativnavalue8zeny;
                uzol->data_.akumulativnavalue9muzi += potomok->data_.akumulativnavalue9muzi;
                uzol->data_.akumulativnavalue10zeny += potomok->data_.akumulativnavalue10zeny;
            }
        }
    }

    // Debug výpis
    if (uzol->data_.uzemnaJednotka != TypUzemnejJednotky::Obec) {
        std::cout << "Uzol: " << uzol->data_.name << " | Akumulované (muži/ženy): "
            << uzol->data_.akumulativnavalue1muzi << "/" << uzol->data_.akumulativnavalue2zeny << ", "
            << uzol->data_.akumulativnavalue3muzi << "/" << uzol->data_.akumulativnavalue4zeny << ", "
            << uzol->data_.akumulativnavalue5muzi << "/" << uzol->data_.akumulativnavalue6zeny << ", "
            << uzol->data_.akumulativnavalue7muzi << "/" << uzol->data_.akumulativnavalue8zeny << ", "
            << uzol->data_.akumulativnavalue9muzi << "/" << uzol->data_.akumulativnavalue10zeny
            << std::endl;
    }
}


/**
void hierarchia::nacitajObceDoStromu(std::vector<Obec>& obce) {
    for (auto& obec : obce) {
        ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node = nullptr;
        strom->processPreOrder(strom->accessRoot(), [&node, &obec](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* n) {
            if (n->data_.kod == obec.kod && n->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
                node = const_cast<ds::amt::MultiWayExplicitHierarchyBlock<Obec>*>(n);
            }
            });

        if (node != nullptr) {
            node->data_ = obec;
        }
    }

**/
void hierarchia::nacitajObceDoStromu(std::vector<Obec>& obce) {
    for (const auto& obec : obce) {
        int regionKod = obec.kod; // Kod obce je zároveň kod regiónu (napr. 111)

        // Nájdenie rodičovského regiónu
        ds::amt::MultiWayExplicitHierarchyBlock<Obec>* parent = nullptr;
        strom->processPreOrder(strom->accessRoot(), [&](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
            if (node->data_.kod == regionKod && node->data_.uzemnaJednotka == TypUzemnejJednotky::Region) {
                parent = const_cast<ds::amt::MultiWayExplicitHierarchyBlock<Obec>*>(node);
            }
            });

        if (!parent) {
            std::cerr << "Region nebol najdeny pre obec: " << obec.name << " s kodom: " << obec.kod << std::endl;
            continue;
        }

        // Pridaj obec do stromu
        auto& novyUzel = strom->emplaceSon(*parent, parent->sons_->size());
        novyUzel.data_ = obec;
        novyUzel.data_.uzemnaJednotka = TypUzemnejJednotky::Obec;
        std::cout << "Pridana obec: " << obec.name << " s kodom: " << obec.kod << " pod: " << parent->data_.name << std::endl;
    }
}

std::string hierarchia::removeBrackets(const std::string& str)
{
    if (str.size() >= 2 && str.front() == '<' && str.back() == '>') {
        return str.substr(1, str.size() - 2); // Odstr?ni prv? a posledn? znak
    }
    return std::string();
}

std::string hierarchia::removeAT(const std::string& str)
{
    // Skontrolujeme, ?i re?azec za??na na "AT" a m? aspo? 2 znaky
    if (str.size() >= 2 && str.substr(0, 2) == "AT") {
        // Vr?time zvy?ok re?azca po "AT"
        return str.substr(2);
    }
    // Ak neza??na na "AT", vr?time pr?zdny re?azec
    return std::string();
}

//docasna metoda na overenie
void hierarchia::vypisStrom() {
    strom->processPreOrder(strom->accessRoot(), [](const ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
        std::cout << "Uzel: " << node->data_.name << ", Kod: " << node->data_.kod << ", Typ: " << static_cast<int>(node->data_.uzemnaJednotka) << std::endl;
        });
}

// Implementácia vypisObceVPodstrome s predikátom
void hierarchia::vypisObceVPodstrome(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, std::function<bool(const Obec&)> predikat) {
    using BlockType = ds::amt::MultiWayExplicitHierarchyBlock<Obec>;
    std::stack<BlockType*> stack;
    stack.push(node);

    // Zbieraj obce do vektora
    std::vector<Obec> obceVPodstrome;
    while (!stack.empty()) {
        BlockType* current = stack.top();
        stack.pop();

        // Ak je uzol obec, pridaj ju do vektora
        if (current->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
            obceVPodstrome.push_back(current->data_);
        }

        // Pridaj synov do zásobníka v opačnom poradí (aby sa spracovali zľava doprava)
        if (current->sons_ != nullptr) {
            std::vector<BlockType*> sons;
            for (auto it = current->sons_->begin(); it != current->sons_->end(); ++it) {
                sons.push_back(*it);
            }
            for (auto it = sons.rbegin(); it != sons.rend(); ++it) {
                stack.push(*it);
            }
        }
    }

    // Filtruj obce pomocou predikátu
    std::vector<Obec> filteredObce = Algoritmus::filterObce(obceVPodstrome, predikat);

    // Vypíš filtrované obce
    if (filteredObce.empty()) {
        std::cout << "Žiadne obce v tomto podstrome, ktoré spĺňajú predikát.\n";
    }
    else {
        Algoritmus::vypisObce(filteredObce);
    }
}

int hierarchia::vypocitajPocetObyvatelovPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* koren, int rok) {
    int pocet = 0;
    // Použijeme uložené údaje (kumulatívne pre GeografickaPoloha, Republika, Region, Rakúsko; priame pre Obec)
    switch (rok) {
    case 2020: pocet = koren->data_.value1muzi + koren->data_.value2zeny; break;
    case 2021: pocet = koren->data_.value3muzi + koren->data_.value4zeny; break;
    case 2022: pocet = koren->data_.value5muzi + koren->data_.value6zeny; break;
    case 2023: pocet = koren->data_.value7muzi + koren->data_.value8zeny; break;
    case 2024: pocet = koren->data_.value9muzi + koren->data_.value10zeny; break;
    }
    return pocet;
}
/**
int hierarchia::vypocitajPocetObyvatelovPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* koren, int rok) {
    int pocet = 0;
    // Dočasne prechádzame podstrom, aby sme zistili, či sú údaje správne
    strom->processPostOrder(koren, [&pocet, rok](ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
        if (node->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
            switch (rok) {
            case 2020: pocet += node->data_.value1muzi + node->data_.value2zeny; break;
            case 2021: pocet += node->data_.value3muzi + node->data_.value4zeny; break;
            case 2022: pocet += node->data_.value5muzi + node->data_.value6zeny; break;
            case 2023: pocet += node->data_.value7muzi + node->data_.value8zeny; break;
            case 2024: pocet += node->data_.value9muzi + node->data_.value10zeny; break;
            }
        }
        });
    return pocet;
}
**/
// Zozbiera uzly zadaného typu v podstrome pomocou iterátora
std::vector<Obec> hierarchia::zozbierajUzlyPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, TypUzemnejJednotky typ) {
    std::vector<Obec> uzly;

    // Použijeme processPostOrder na prehľadávanie podstromu
    strom->processPostOrder(node, [&uzly, typ](ds::amt::MultiWayExplicitHierarchyBlock<Obec>* currentNode) {
        if (currentNode->data_.uzemnaJednotka == typ) {
            uzly.push_back(currentNode->data_);
        }
        });

    return uzly;
}

// Pomocná metóda na zozbieranie obcí v podstrome daného uzla
std::vector<Obec> hierarchia::collectObceVPodstrome(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
    std::vector<Obec> obceVPodstrome;

    // Použijeme processPostOrder na prehľadávanie podstromu
    strom->processPostOrder(node, [&obceVPodstrome](ds::amt::MultiWayExplicitHierarchyBlock<Obec>* currentNode) {
        if (currentNode->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
            obceVPodstrome.push_back(currentNode->data_);
        }
        });

    return obceVPodstrome;
}

void hierarchia::aktualizujKumulativneUdaje() {
    strom->processPostOrder(strom->accessRoot(), [this](ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node) {
        // Vynulujeme údaje pre každý vrchol (aj pre Obec, aby sme zabezpečili konzistenciu)

        if (node->data_.uzemnaJednotka == TypUzemnejJednotky::Obec) {
            // Pre vrcholy typu Obec ponecháme ich vlastné údaje
            // (Tieto údaje už boli nastavené v nacitajObceDoStromu)
        }
        else {
            node->data_.value1muzi = 0;
            node->data_.value2zeny = 0;
            node->data_.value3muzi = 0;
            node->data_.value4zeny = 0;
            node->data_.value5muzi = 0;
            node->data_.value6zeny = 0;
            node->data_.value7muzi = 0;
            node->data_.value8zeny = 0;
            node->data_.value9muzi = 0;
            node->data_.value10zeny = 0;
            // Pre vrcholy typu GeografickaPoloha, Republika, Region a Rakúsko sčítame údaje zo synov
            if (node->sons_ != nullptr) {
                for (auto it = node->sons_->begin(); it != node->sons_->end(); ++it) {
                    auto* son = *it;
                    node->data_.value1muzi += son->data_.value1muzi;
                    node->data_.value2zeny += son->data_.value2zeny;
                    node->data_.value3muzi += son->data_.value3muzi;
                    node->data_.value4zeny += son->data_.value4zeny;
                    node->data_.value5muzi += son->data_.value5muzi;
                    node->data_.value6zeny += son->data_.value6zeny;
                    node->data_.value7muzi += son->data_.value7muzi;
                    node->data_.value8zeny += son->data_.value8zeny;
                    node->data_.value9muzi += son->data_.value9muzi;
                    node->data_.value10zeny += son->data_.value10zeny;
                }
            }
        }
        });
}
//3 uroven
void hierarchia::indexAllNodes() {
    using Block = ds::amt::MultiWayExplicitHierarchyBlock<Obec>;
    using NodePtr = Block*;

    if (!strom || !strom->accessRoot()) return;

    strom->processPreOrder(
        strom->accessRoot(),
        [this](const Block* node) {
            // z mapy vyberieme unique_ptr na Table<string,NodeList>
            auto& uptr = byType[node->data_.uzemnaJednotka];
            auto* tbl = static_cast<
                ds::adt::HashTable<std::string, NodeList>*
            >(uptr.get());

            NodeList* slot = nullptr;
            if (!tbl->tryFind(node->data_.name, slot)) {
                // prvé vloženie: vytvoríme list s jedným prvkom
                tbl->insert(node->data_.name,
                    NodeList{ const_cast<NodePtr>(node) });
            }
            else {
                // duplikát: pripojeme ďalší uzol do listu
                slot->push_back(const_cast<NodePtr>(node));
            }
        }
    );
}


void hierarchia::initTables() {
    using HT = ds::adt::HashTable<std::string, NodeList>;
    byType[TypUzemnejJednotky::Obec] = std::make_unique<HT>();
    byType[TypUzemnejJednotky::Region] = std::make_unique<HT>();
    byType[TypUzemnejJednotky::Republika] = std::make_unique<HT>();
    byType[TypUzemnejJednotky::GeografickaPoloha] = std::make_unique<HT>();
    byType[TypUzemnejJednotky::Rakusko] = std::make_unique<HT>();
}


hierarchia::NodeList
hierarchia::findAll(const std::string& name,
    TypUzemnejJednotky type) const
{
    NodeList out;
    auto it = byType.find(type);
    if (it == byType.end()) return out;

    // získame HashTable<string,NodeList>*
    auto* tbl = static_cast<
        ds::adt::HashTable<std::string, NodeList>*
    >(it->second.get());

    // prejdeme všetky položky tabuľky
    for (auto iter = tbl->begin(); iter != tbl->end(); ++iter) {
        auto const& item = *iter;  // TableItem<string,NodeList>&
        if (item.key_ == name) {
            // pripojeme celý list z tabuľky do výsledkov
            out.insert(
                out.end(),
                item.data_.begin(),
                item.data_.end()
            );
        }
    }
    return out;
}




void hierarchia::printTables() const {
    using HT = ds::adt::HashTable<std::string, NodeList>;

    for (auto const& [type, tblPtr] : byType) {
        std::cout << "=== Table for type "
            << static_cast<int>(type)
            << " ===\n";
        auto* tbl = static_cast<HT*>(tblPtr.get());

        if (tbl->isEmpty()) {
            std::cout << "  (empty)\n\n";
            continue;
        }

        for (auto it = tbl->begin(); it != tbl->end(); ++it) {
            auto const& item = *it;  // TableItem<string,NodeList>&
            std::cout << "  key=\"" << item.key_ << "\" → [ ";
            bool first = true;
            for (auto node : item.data_) {
                if (!first) std::cout << ", ";
                std::cout << node->data_.name
                    << "(kod=" << node->data_.kod << ")";
                first = false;
            }
            std::cout << " ]\n";
        }
        std::cout << "\n";
    }
}

