#pragma once

#include <complexities/complexity_analyzer.h>
#include <libds/adt/table.h>
#include <random>
#include <ctime>
#include <string>
#include <memory>
#include <vector>

namespace ds::utils
{

    //
    // 1) Základný analyzer, ktorý pripraví kapacitu a vygeneruje náhodný kľúč/hodnotu
    //
    template<typename Table>
    class TableAnalyzerBase : public ComplexityAnalyzer<Table>
    {
    public:
        explicit TableAnalyzerBase(const std::string& name)
            : ComplexityAnalyzer<Table>(name),
            _rngKey(static_cast<unsigned>(std::time(nullptr))),
            _rngVal(static_cast<unsigned>(std::time(nullptr))),
            _currentKey(),
            _currentVal(0)
        {
            // pred každou operáciou vyrobíme nový kľúč/hodnotu
            this->registerBeforeOperation([this](Table&)
                {
                    std::uniform_int_distribution<int> dist(100, 200000);
                    _currentKey = "key_" + std::to_string(dist(_rngKey));
                    _currentVal = _rngVal();
                });
        }

    protected:
        // Naplní tabuľku "targetSize" báznymi kľúčmi
        void growToSize(Table& tbl, size_t targetSize) override
        {
            tbl.clear();
            while (tbl.size() < targetSize)
            {
                auto k = std::string("base") + std::to_string(tbl.size());
                tbl.insert(k, static_cast<int>(tbl.size()));
            }
        }

        std::string  pickKey()   const { return _currentKey; }
        int          pickValue() const { return _currentVal; }

        mutable std::default_random_engine _rngKey;
        mutable std::default_random_engine _rngVal;
        mutable std::string               _currentKey;
        mutable int                       _currentVal;
    };

    //
    // 2) Analyzer pre INSERT operácie
    //
    template<typename Table>
    class TableInsertAnalyzer : public TableAnalyzerBase<Table>
    {
    public:
        explicit TableInsertAnalyzer(const std::string& name)
            : TableAnalyzerBase<Table>(name)
        {
        }

    protected:
        void executeOperation(Table& tbl) override
        {
            // získaj kľúč/hodnotu
            auto k = this->pickKey();
            int* existing = nullptr;
            // ak už existuje, generuj nový
            std::uniform_int_distribution<int> dupDist(1'0000'000, 1'9999'999);
            while (tbl.tryFind(k, existing))
            {
                k = "key" + std::to_string(dupDist(this->_rngKey));
            }
            tbl.insert(k, this->pickValue());
        }
    };

    //
    // 3) Analyzer pre FIND operácie
    //
    template<typename Table>
    class TableFindAnalyzer : public TableAnalyzerBase<Table>
    {
    public:
        explicit TableFindAnalyzer(const std::string& name)
            : TableAnalyzerBase<Table>(name)
        {
        }

    protected:
        void executeOperation(Table& tbl) override
        {
            int* out = nullptr;
            tbl.tryFind(this->pickKey(), out);
        }
    };

    //
    // 4) Zložený Analyzer – zaregistrujeme insert + find
    //
    class TablesAnalyzer : public CompositeAnalyzer
    {
    public:
        TablesAnalyzer()
            : CompositeAnalyzer("tables")
        {
            using MapType = ds::adt::HashTable<std::string, int>;

            // Ak váš CompositeAnalyzer vyžaduje unique_ptr<ComplexityAnalyzer<...>>
            this->addAnalyzer(
                std::unique_ptr<ComplexityAnalyzer<MapType>>(
                    new TableInsertAnalyzer<MapType>("hashtable-insert")
                )
            );
            this->addAnalyzer(
                std::unique_ptr<ComplexityAnalyzer<MapType>>(
                    new TableFindAnalyzer<MapType>("hashtable-find")
                )
            );
        }
    };

} // namespace ds::utils
