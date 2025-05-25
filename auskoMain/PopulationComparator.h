// Comparators.h
#pragma once
#include "Citac.h"
#include <libds/amt/explicit_hierarchy.h>

using NodePtr = ds::amt::MultiWayExplicitHierarchyBlock<Obec>*;

struct CompareByPopulation {
    enum class Gender { MUZI, ZENY, SPOLU };

    CompareByPopulation(int year, Gender g)
        : year_{ year }, gender_{ g } {
    }

    // MUST return –1 / 0 / +1
    int operator()(NodePtr A, NodePtr B) const {
        int pa = pop(A->data_, year_, gender_);
        int pb = pop(B->data_, year_, gender_);
        if (pa < pb) return -1;
        else if (pa > pb) return +1;
        else              return 0;
    }

private:
    int year_;
    Gender gender_;

    static int pop(const Obec& o, int year, Gender g) {
        int m = 0, f = 0;
        switch (year) {
        case 2020: m = o.value1muzi;  f = o.value2zeny;  break;
        case 2021: m = o.value3muzi;  f = o.value4zeny;  break;
        case 2022: m = o.value5muzi;  f = o.value6zeny;  break;
        case 2023: m = o.value7muzi;  f = o.value8zeny;  break;
        case 2024: m = o.value9muzi;  f = o.value10zeny; break;
        default:   m = f = 0;
        }
        switch (g) {
        case Gender::MUZI:  return m;
        case Gender::ZENY:  return f;
        case Gender::SPOLU: return m + f;
        }
        return m + f;
    }
};
