#pragma once

#include <vector>
#include <random>

#include "node.hpp"

class tree_generator {
private:
    std::vector<node> tree;
    std::random_device dev;
    std::mt19937_64 gen;

    void gen_subtree(uint32_t v, int sz) {
        int l = std::uniform_int_distribution<int>(0, sz - 1)(gen),
            r = sz - 1 - l;
        if (l) {
            tree[v].l = tree.size(); tree.emplace_back();
            tree.back().p = v;
            gen_subtree(tree.size() - 1, l);
        }
        if (r) {
            tree[v].r = tree.size(); tree.emplace_back();
            tree.back().p = v;
            gen_subtree(tree.size() - 1, r);
        }
    }

public:
    tree_generator() : gen(dev()) {tree.emplace_back(); tree.emplace_back();};

    std::vector<node> generate_random_tree(int n) {
        gen_subtree(1, n);
        return this->tree;
    }
};