
/**
 * @file test_core.cpp
 * @author TV1
 * @brief Tests core utilities of the application.
 */

#include "core/Date.h"
#include "core/DynamicArray.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

void testBasicOperations() {
    DynamicArray<int> arr(2);

    assert(arr.empty());
    assert(arr.size() == 0);

    arr.add(10);
    arr.add(20);
    arr.add(30);

    assert(arr.size() == 3);
    assert(arr.getSize() == 3);
    assert(arr.get(0) == 10);
    assert(arr[1] == 20);
    assert(arr.get(2) == 30);

    arr[0] = 100;
    assert(arr.get(0) == 100);

    const DynamicArray<int>& constArr = arr;
    assert(constArr.get(0) == 100);
    assert(constArr[1] == 20);
}

void testDeepCopy() {
    DynamicArray<int> original;
    original.add(10);
    original.add(20);

    // Copy constructor
    DynamicArray<int> copied(original);
    copied[0] = 99;

    assert(original[0] == 10);
    assert(copied[0] == 99);

    // Copy assignment
    DynamicArray<int> assigned;
    assigned = original;
    assigned[1] = 88;

    assert(original[1] == 20);
    assert(assigned[1] == 88);

    // Self-assignment
    DynamicArray<int>& sameArray = assigned;
    assigned = sameArray;
    assert(assigned[1] == 88);
}

void testRemoveAndClear() {
    DynamicArray<int> arr;

    for (int i = 1; i <= 5; ++i) {
        arr.add(i);
    }

    arr.remove(0);
    assert(arr[0] == 2);

    arr.remove(1);
    assert(arr[1] == 4);

    arr.remove(arr.size() - 1);
    assert(arr.size() == 2);

    arr.clear();
    assert(arr.empty());
    assert(arr.size() == 0);

    arr.add(100);
    assert(arr[0] == 100);
}

void testInvalidInputs() {
    bool caught = false;

    try {
        DynamicArray<int> invalid(0);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    DynamicArray<int> arr;
    arr.add(10);

    caught = false;
    try {
        arr.get(-1);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        arr.remove(5);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
}

void testDifferentTypes() {
    DynamicArray<std::string> names;

    names.add("Cash");
    names.add("Bank");

    assert(names.size() == 2);
    assert(names[0] == "Cash");
    assert(names[1] == "Bank");
}

void testDateValidation() {
    Date today;
    assert(today.isValid());

    Date leapDate(29, 2, 2024);
    assert(leapDate.isValid());
    assert(leapDate.isLeapYear());

    Date normalDate(28, 2, 2026);
    assert(normalDate.isValid());
    assert(!normalDate.isLeapYear());

    Date centuryLeap(1, 1, 2000);
    assert(centuryLeap.isLeapYear());

    Date centuryNormal(1, 1, 1900);
    assert(!centuryNormal.isLeapYear());

    bool caught = false;

    try {
        Date invalid(29, 2, 2026);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        Date invalid(31, 4, 2026);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        Date invalid(1, 13, 2026);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    caught = false;
    try {
        Date invalid(1, 1, 0);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
}

void testDateComparison() {
    Date first(10, 11, 2026);
    Date second(15, 11, 2026);
    Date same(10, 11, 2026);
    Date nextYear(1, 1, 2027);

    assert(first == same);
    assert(first != second);
    assert(first < second);
    assert(first <= same);
    assert(second > first);
    assert(second >= first);
    assert(first < nextYear);
    assert(nextYear > second);

    assert(first.getDay() == 10);
    assert(first.getMonth() == 11);
    assert(first.getYear() == 2026);
}


int main() {
    testBasicOperations();
    testDeepCopy();
    testRemoveAndClear();
    testInvalidInputs();
    testDifferentTypes();

    testDateValidation();
    testDateComparison();

    std::cout << "All core tests passed!\n";

    return 0;
}
