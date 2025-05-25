#pragma once

#include <libds/amt/explicit_hierarchy.h>
#include "Citac.h"
#include <memory>
#include <string>
#include <vector>
#include "SimpleList.h"
#include "SimpleMap.h"
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
    using NodeList = SimpleList<NodePtr>;

    NodeList findAll(const std::string& name, TypUzemnejJednotky type) const;
    NodePtr find(const std::string& name, TypUzemnejJednotky type) const;

    void printTables() const;

    // utility
    static std::string removeBrackets(const std::string& str);
    static std::string removeAT(const std::string& str);

private:
    std::vector<Obec> zozbierajUzlyPodstromu(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node, TypUzemnejJednotky typ);
    std::vector<Obec> collectObceVPodstrome(ds::amt::MultiWayExplicitHierarchyBlock<Obec>* node);
    ds::amt::MultiWayExplicitHierarchy<Obec>* strom = nullptr;

    // Vlastná mapa: typ územnej jednotky → tabuľka názov→NodeList
    SimpleMap<TypUzemnejJednotky, ds::adt::Table<std::string, NodeList>*> byType;
};
