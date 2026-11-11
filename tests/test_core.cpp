
/**
 * @file test_core.cpp
 * @author TV1
 * @brief Tests core utilities of the application.
 */

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

int main() {
    testBasicOperations();
    testDeepCopy();
    testRemoveAndClear();
    testInvalidInputs();
    testDifferentTypes();

    std::cout << "All DynamicArray tests passed!\n";

    return 0;
}
