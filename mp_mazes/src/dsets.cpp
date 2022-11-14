#include "dsets.h"

void DisjointSets::addelements(int num) {
    sets.insert(sets.end(), num, -1);
}

int DisjointSets::find(int elem) {
    if (elem >= (int)sets.size()) return -1;
    if (sets[elem] < 0) return elem;
    int root = find(sets[elem]);
    sets[elem] = root;
    return root;
}

void DisjointSets::setunion(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);
    if (rootA == rootB) {
        return;
    } else if (sets[rootA] <= sets[rootB])  {
        sets[rootA] += sets[rootB];
        sets[rootB] = a;
    } else {
        sets[rootB] += sets[rootA];
        sets[rootA] = b;
    }
}

int DisjointSets::size(int elem) {
    return sets[find(elem)] * -1;
}
