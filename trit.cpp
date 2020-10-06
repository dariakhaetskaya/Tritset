//
// Created by rey on 10/3/20.
//
#include"trit.h"
#include <bitset>
#include <stdexcept>

const size_t TRIT_SIZE = 2;
const size_t UNKNOWN_INT = 6148914691236517205; // int which is 01010101...
typedef enum {OUT_OF_RANGE = -1, OK = 1} Tstate; // Gvar which track the state because we shouldn't throw exceptions
Tstate state = OK;

size_t TritSet::getIntIdx(size_t index)
{
    size_t intIdx = index / (sizeof(size_t) * CHAR_BIT);

    // if we still have trits in remain
    if (index - intIdx * sizeof(size_t) * CHAR_BIT >= 0){
        intIdx++;
    }

    return intIdx;
}

TritSet::TritSet(int size) // constructor
{
    int arraySize = getIntIdx(TRIT_SIZE * size);
    array = std::vector<size_t> (arraySize, UNKNOWN_INT);
    capacity = size;
}

TritSet::TritSet(const TritSet& set) {
    array = set.array;
    capacity = set.capacity;
}

TritSet::~TritSet(){
    array.clear();
}

TritSet::ProxyTrit::ProxyTrit(TritSet& rSet, size_t idx)
        :tritRef{rSet}
{
    index = idx;
}

size_t TritSet::getCapacity() const
{
    return capacity;
}

TritSet::ProxyTrit TritSet::operator[](size_t index)
{
    return ProxyTrit{*this, index};
}

void TritSet::resize(size_t index)
{
    array.resize(getIntIdx(index * TRIT_SIZE));
    capacity = index;
}

TritSet::ProxyTrit TritSet::ProxyTrit::operator=(Trit tritVal)
{
    if (index >= tritRef.capacity){
        if(tritVal == Unknown){
            state = OUT_OF_RANGE; // we tell the next function that the index is out of range
                                  // and we do not want to resize the set because new trit is unknown
        } else {
            tritRef.resize(index);
        }
        //throw std::out_of_range("index is out of range"); //it should be here but it's not
    }

    tritRef.updateTrit(index, tritVal);
    return ProxyTrit{tritRef, index};
}

void TritSet::updateTrit(size_t index, Trit tritVal)
{
    if (state == OUT_OF_RANGE){ // here we "catch the exception" do not change
                                // anything and get the state back to OK
        state = OK;
        return;
    }
    index *= TRIT_SIZE; // index in bits, not trits
    switch (tritVal)
    {
        case False: // false is 0 = 00
        {
            setBit(index, false);
            setBit(index + 1, false);
            break;
        }

        case True: // true is 3 = 11
        {
            setBit(index, true);
            setBit(index + 1, true);
            break;
        }

        case Unknown: // unknown is 1 = 01
        {
            setBit(index, false);
            setBit(index + 1, true);
            break;
        }
    }
}

void TritSet::setBit(size_t index, bool val)
{
    size_t tritIdx = index % (sizeof(size_t) * CHAR_BIT);
    size_t mask = 1;
    mask = mask << (sizeof(size_t) * CHAR_BIT - 1);  // shift 1 to the beginning of binary set
    mask = mask >> tritIdx; // shift it to the current trit's position

    size_t intIdx = getIntIdx(index) - 1; // getting index of an int which contains the trit we want to get

    if (val){
        array[intIdx] |= mask; // setting the bit to 1
    } else {
        array[intIdx] &= ~mask; // setting the bit to 0
    }
}

bool TritSet::getBit(size_t index) const
{
    size_t tritIdx = index % (sizeof(size_t) * CHAR_BIT);
    size_t mask = 1;
    mask = mask << (sizeof(size_t) * CHAR_BIT - 1); // shift 1 to the beginning of binary set
    mask = mask >> tritIdx; // shift if to the current trit's position

    size_t intIdx = getIntIdx(index) - 1; // getting index of an int which contains the trit we want to get
    size_t tmp = array[intIdx]; // getting the copy of int so we do not change the value inside of it

    tmp &= mask; // getting the bit
    return tmp != 0; // if it was 1 than this expression will be true and vise versa
}

Trit TritSet::getTrit(size_t index) const
{
    index *= TRIT_SIZE;
    bool firstBit = getBit(index); // getting two bits out of int
    bool secondBit = getBit(index + 1);

    if (firstBit && secondBit){ // 11 = True
        return True;
    } else if (!firstBit && !secondBit){ // 00 = False
        return False;
    } else { // 01 = Unknown
        return Unknown;
    }
}

TritSet::ProxyTrit::operator Trit()const
{
    Trit t = tritRef.getTrit(index);
    return t;
}

std::ostream& operator<< (std::ostream &out,const TritSet::ProxyTrit& pTrit)
{
    switch(Trit(pTrit)){
        case False:
        {
            out << "false";
            break;
        }
        case True:
        {
            out << "true";
            break;
        }
        case Unknown:
        {
            out << "unknown";
            break;
        }
    }
    return out;
}

TritSet& TritSet::operator=(const TritSet& set) // copy constructor
{
    array.resize(set.array.size(), UNKNOWN_INT);
    array = set.array;
    capacity = set.capacity;
    return *this;
}

void TritSet::shrink()
{
    array.resize(getIntIdx(getsize() * TRIT_SIZE));
    capacity = getsize();
}

void TritSet::makeEqual(TritSet& a, TritSet& b)
{
    if (a.getCapacity() == b.getCapacity())
        return;

    if (a.getCapacity() > b.getCapacity())
    {
        a.array.resize(array.size(),UNKNOWN_INT);
        a.capacity = b.capacity;

    } else if (a.getCapacity() < b.getCapacity())
    {
        b.array.resize(array.size(),UNKNOWN_INT);
        b.capacity = a.capacity;
    }
}

TritSet& TritSet::operator&(TritSet& set)
{
    makeEqual(*this, set);

    for (int i = 0; i < array.size(); i++){
        array[i] &= set.array[i];
    }

    return *this;
}

TritSet& TritSet::operator|(TritSet& set)
{
    makeEqual(*this, set);

    for (int i = 0; i < array.size(); i++){
        array[i] |= set.array[i];
    }

    return *this;
}

TritSet& TritSet::operator~()
{
    for (int i = 0; i < array.size(); i++){
        array[i] = ~array[i];
    }
    return *this;
}

void TritSet::printBitwise()
{
    std::cout << std::endl;

    for (int i = 0; i < array.size(); i++) {
        std::bitset<sizeof(size_t) * CHAR_BIT> a(array[i]);
        std::cout << a << std::endl;
    }

    std::cout << std::endl;
}

size_t TritSet::getsize() const
{
    size_t tritSize = 0;

    for (int i = 0; i < capacity; i++){ // iterating through the set and keep index of the last True/False trit
        if (getTrit(i) != Unknown){
            tritSize = i;
        }
    }

    return tritSize;
}