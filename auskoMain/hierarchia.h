#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include "Citac.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <libds/adt/table.h>

class hierarchia {
public:
    hierarchia();
    ~hierarchia();

    void naplnenieHierarchie();

    // 2. úroveň (strom)
    void doplnKodyObciam(const std::string& filename, std::vector<Obec>& obce);
    void nacitajStukturuObci(const std::string& filename, std::vector<Obec>& obce);
    void nacitajObceDoStromu(std::vector<Obec>& obce);
    void vypocitajAkumulativneHodnoty(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* uzol);
    void aktualizujKumulativneUdaje();
    void vypisStrom();

    void vypisObceVPodstrome(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, std::function<bool(const Obec&)> predikat);

    int vypocitajPocetObyvatelovPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* koren, int rok);

    ds::amt::MultiWayExplicitHierarchy<Obec>* getStrom() { return strom; }

    // 3. úroveň (tabuľky)
    void initTables();
    void indexAllNodes();

    using NodePtr = ds::amt::MultiWayExplicitHierarchyBlock<Obec>*;
    using NodeList = std::list<NodePtr>;
    

    /// Vráti *všetky* uzly so zadaným menom a typom
    std::list<NodePtr> findAll(const std::string& name,
        TypUzemnejJednotky type) const;
    

    /// Vráti prvý nájdený uzol alebo nullptr
    NodePtr find(const std::string& name,
        TypUzemnejJednotky type) const;

    /// Debug: vypíše celý obsah tabuliek
    void printTables() const;

    // utility
    static std::string removeBrackets(const std::string& str);
    static std::string removeAT(const std::string& str);

private:
    std::vector<Obec> zozbierajUzlyPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, TypUzemnejJednotky typ);
    /// Pomocná metóda, ktorá zoberie za parameter ukazovateľ na blok a
    /// vráti vektor všetkých Obec v pod­stro­me.
    std::vector<Obec>
        collectObceVPodstrome(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node);
    ds::amt::MultiWayExplicitHierarchy<Obec>* strom = nullptr;

    // jediná mapa: typ územnej jednotky → hash-tabuľka kľúč→zoznam uzlov
    std::unordered_map<TypUzemnejJednotky,
        std::unique_ptr< ds::adt::Table<std::string, NodeList> >
    > byType;


};
