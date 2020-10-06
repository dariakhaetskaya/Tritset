//
// Created by rey on 10/4/20.
//

#include <gtest/gtest.h>
#include "trit.h"

TEST(BasicTests, TestName) {
    EXPECT_EQ(0, Trit::False);
    EXPECT_EQ(1, Trit::Unknown);
    EXPECT_EQ(3, Trit::True);
}

TEST(BasicTests, doc_tests) {
    TritSet set{1000};
    // length of internal array
    size_t allocLength = set.getCapacity();
    assert(allocLength >= 1000 * 2 / 8 / sizeof(unsigned int));
    // 1000*2 - min bits count
    // 1000*2 / 8 - min bytes count
    // 1000*2 / 8 / sizeof(uint) - min uint[] size

    // не выделяет никакой памяти
    set[1000000000] = Trit::Unknown;
    ASSERT_EQ(allocLength, set.getCapacity());

    //выделение памяти
    set[1000000000] = Trit::True;
    ASSERT_LT(allocLength, set.getCapacity());

    // no memory operations
    allocLength = set.getCapacity();
    set[1000000000] = Trit::Unknown;
    set[1000000] = Trit::False;
    ASSERT_EQ(allocLength, set.getCapacity());

    // освобождение памяти до начального значения или
    // до значения необходимого для хранения последнего установленного трита
    // в данном случае для трита 1000’000
    set.shrink();
    ASSERT_GT(allocLength, set.getCapacity());

}

TEST(BasicTests, set_get_not_and_or) {
    TritSet a{100};
    TritSet b{100};

    // TEST 1: T & F = F

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::True;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::False, Trit(a[i]));
    }

    // TEST 2: F & U = F

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::Unknown;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::False,Trit(a[i]));
    }

    // TEST 3: T & U = U

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::True;
        b[i] = Trit::Unknown;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::Unknown,Trit(a[i]));
    }

    // TEST 4: U & U = U

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::Unknown;
        b[i] = Trit::Unknown;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::Unknown,Trit(a[i]));
    }

    // TEST 5: T & T = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::True;
        b[i] = Trit::True;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True,Trit(a[i]));
    }

    // TEST 6: F & F = F

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::False;
    }

    a = a & b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::False,Trit(a[i]));
    }

    // TEST 7: ~F = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
    }

    a = ~a;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True,Trit(a[i]));
    }

    // TEST 8: ~F = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
    }

    a = ~a;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True,Trit(a[i]));
    }

    // TEST 9: ~U = U

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::Unknown;
    }

    a = ~a;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::Unknown,Trit(a[i]));
    }

    // TEST 10: T | F = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::True;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True, Trit(a[i]));
    }

    // TEST 11: F | U = U

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::Unknown;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::Unknown,Trit(a[i]));
    }

    // TEST 3: T | U = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::True;
        b[i] = Trit::Unknown;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True,Trit(a[i]));
    }

    // TEST 4: U | U = U

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::Unknown;
        b[i] = Trit::Unknown;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::Unknown,Trit(a[i]));
    }

    // TEST 5: T | T = T

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::True;
        b[i] = Trit::True;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::True,Trit(a[i]));
    }

    // TEST 6: F | F = F

    for (int i = 0; i < 100; i++) {
        a[i] = Trit::False;
        b[i] = Trit::False;
    }

    a = a | b;

    for (int i = 0; i < 100; i++) {
        ASSERT_EQ(Trit::False,Trit(a[i]));
    }

}