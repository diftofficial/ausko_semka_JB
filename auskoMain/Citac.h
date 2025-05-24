#ifndef CITAC_H
#define CITAC_H

#include <string>
#include <vector>

//enum na typUzemnejJendotky
enum class TypUzemnejJednotky {
    Rakusko,
    GeografickaPoloha,
    Republika,
    Region,
    Obec
};
// ?trukt?ra Obec na uchov?vanie ?dajov o obci
struct Obec {
    TypUzemnejJednotky uzemnaJednotka;
    std::string name;    // N?zov obce
    int id;      // ID obce
    int value1muzi;  // Hodnota 1 muzi
    int value2zeny;  // Hodnota 2 zeny
    int value3muzi;  // Hodnota 3 muzi
    int value4zeny;  // Hodnota 4 zeny
    int value5muzi;  // Hodnota 5 muzi
    int value6zeny;  // Hodnota 6 zeny
    int value7muzi;  // Hodnota 7 muzi
    int value8zeny;  // Hodnota 8 zeny
    int value9muzi;  // Hodnota 9 muzi
    int value10zeny;  // Hodnota 10 zeny

    int kod;  // dop??ame nesk?r v hierarchi ked citame subor obce.csv

    int akumulativnavalue1muzi;  // Hodnota 1 muzi
    int akumulativnavalue2zeny;  // Hodnota 2 zeny
    int akumulativnavalue3muzi;  // Hodnota 3 muzi
    int akumulativnavalue4zeny;  // Hodnota 4 zeny
    int akumulativnavalue5muzi;  // Hodnota 5 muzi
    int akumulativnavalue6zeny;  // Hodnota 6 zeny
    int akumulativnavalue7muzi;  // Hodnota 7 muzi
    int akumulativnavalue8zeny;  // Hodnota 8 zeny
    int akumulativnavalue9muzi;  // Hodnota 9 muzi
    int akumulativnavalue10zeny;  // Hodnota 10 zeny

    //overloadujes ==
    // Define the == operator for Obec
    bool operator==(const Obec& other) const {
        return this->id == other.id && this->name == other.name;
    }
    bool operator!=(const Obec& other) const {
        return !(*this == other);
    }
};

// Trieda Citac na ??tanie ?dajov zo s?boru
class Citac {
public:
    // Konstruktor triedy
    Citac();

    // Met?da na na??tanie ?dajov zo s?boru
    bool loadData(bool, const std::string&);

    // Met?da na z?skanie v?etk?ch na??tan?ch obc?
    const std::vector<Obec>& getObce() const;
    // Met?da na z?skanie v?etk?ch na??tan?ch obc? bez const
    std::vector<Obec>& getObcebezconst();

    std::string removeBrackets(const std::string& str);

    int spocitajCelkovoObyvatelov( std::vector<Obec>& obce);



private:
    bool prvySubor;
    int pocetNacitaniaSuborov;
    std::string filename;   // N?zov s?boru
    std::vector<Obec> obce; // Zoznam na??tan?ch obc?
};

#endif // CITAC_H
