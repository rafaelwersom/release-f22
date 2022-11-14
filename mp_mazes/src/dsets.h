/**
 * @file dsets.h
 * Declaraction of the Disjoint Sets class.
 */

#pragma once

#include <vector>

class DisjointSets
{
    private:
        std::vector<int> sets;

    public:
        DisjointSets() = default;
        void addelements(int num);
        int find(int elem);
        void setunion(int a, int b);
        int size(int elem);

};