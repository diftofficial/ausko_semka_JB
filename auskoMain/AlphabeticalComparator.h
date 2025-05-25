// Comparators.h
#pragma once
#include "Citac.h"   // for Obec
#include <libds/amt/explicit_hierarchy.h>

using NodePtr = ds::amt::MultiWayExplicitHierarchyBlock<Obec>*;

struct CompareNodeByName {
    // MUST return -1, 0 or +1
    int operator()(const NodePtr& A, const NodePtr& B) const {
        const auto& a = A->data_.name;
        const auto& b = B->data_.name;
        if (a < b)  return -1;
        if (a > b)  return +1;
        return 0;
    }
};
