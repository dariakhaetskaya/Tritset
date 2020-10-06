//
// Created by rey on 10/3/20.
//

#ifndef LAB1_TRIT_H
#define LAB1_TRIT_H
#include <climits>
#include <iostream>
#include <vector>

enum Trit{ False = 0 , Unknown = 1, True = 3};

class TritSet
{
private:

    std::vector<size_t> array;
    size_t capacity;

public:

    class ProxyTrit // proxy class
    {
    private:
        TritSet& tritRef;
        size_t index;
    public:
        ProxyTrit(TritSet& rSet, size_t idx);
        ProxyTrit operator=(Trit t);
        explicit operator Trit()const;
        ~ProxyTrit() = default;
    };


    // TritSet Methods

    friend std::ostream& operator<< (std::ostream &out, const  ProxyTrit& pTrit); // outputs trits value in text
    explicit TritSet(int size); // constructor
    TritSet(const TritSet& set); // copy constructor
    ~TritSet(); // set destructor
    size_t getsize() const; // logical size getter
    void resize(size_t index); // resize a set
    size_t getCapacity() const; // max size getter
    ProxyTrit operator[](size_t index); // get a trit by it's index
    TritSet& operator=(const TritSet& set); // assigment operator
    TritSet& operator&(TritSet& set); // AND logical operation
    TritSet& operator|(TritSet& set); // OR logical operation
    TritSet& operator~(); // NOT logical operation
    void shrink(); // all the unknown trits from the tail

private:
    void updateTrit(size_t index, Trit t); // set a particular trit to value
    void setBit(size_t index, bool val); // set a particular bit to value
    bool getBit(size_t index) const ; // get a particular bit from int
    Trit getTrit(size_t index) const; // get a particular trit from int
    void makeEqual(TritSet& a, TritSet& b); // makes sizes of two sets a size of the larger one. Added trits are Unknown
    static size_t getIntIdx(size_t index); // calculates index of an int which contains (index)th trit
    void printBitwise(); // for debugging purposes
};

#endif //LAB1_TRIT_H
