#include <iostream>

#include "tree.hpp"
#include "renderer.hpp"
#include <emscripten/bind.h>

std::string render_tree(int n) {
    std::vector<node> tree = tree_generator().generate_random_tree(n);
    return tree_renderer(tree).render();
}

EMSCRIPTEN_BINDINGS(renderer) {
    emscripten::function("render_tree", &render_tree);
}