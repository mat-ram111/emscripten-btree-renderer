#pragma once

#include <vector>
#include <sstream>
#include "node.hpp"

class tree_renderer {
private:
    std::vector<node>& tree;

    std::ostringstream svg;
    const static inline double X = -4000, Y = -1200;
    const static inline double W = 8000, H = 2400;

    const static inline double DX = 30, DY = 60;
    const static inline double NODE_RADIUS = 10;
    const static inline double EDGE_WIDTH = 2;
    const static inline std::string NODE_COLOR = "blue";
    const static inline std::string EDGE_COLOR = "gray";

    void divide_and_conquer(uint32_t v) {
        if (v) {
            divide_and_conquer(tree[v].l); 
            divide_and_conquer(tree[v].r);
            combine(v);
        }
    }

    void combine(uint32_t v) {
        if (!v || v & node::THREAD) return;
        uint32_t ll = tree[v].l, lr = tree[v].l,
                 rl = tree[v].r, rr = tree[v].r;
        double lx = 0.0, rx = 0.0;
        if (ll) tree[ll].cx = -DX / 2; if (rr) tree[rr].cx = DX / 2;

        for (bool stop = !(ll && rr); !stop;) {
            lx += tree[lr].cx; rx += tree[rl].cx;

            double dlt = std::max((DX - (rx - lx)) / 2, 0.0);
            lx -= dlt; rx += dlt;
            tree[tree[v].l].cx -= dlt; tree[tree[v].r].cx += dlt;

            stop = !(tree[lr].r && tree[rl].l);
            if (tree[lr].r && !tree[rr].r) tree[rr].r = node::THREAD | tree[lr].r;
            if (tree[rl].l && !tree[ll].l) tree[ll].l = node::THREAD | tree[rl].l;
            if (!stop) {
                ll = tree[ll].l &~ node::THREAD; lr = tree[lr].r &~ node::THREAD;
                rl = tree[rl].l &~ node::THREAD; rr = tree[rr].r &~ node::THREAD;
            }
        }
        if (tree[v].l && !tree[v].r) tree[v].r = node::THREAD | tree[v].l;
        if (tree[v].r && !tree[v].l) tree[v].l = node::THREAD | tree[v].r;
    }

    void accumulate(uint32_t v) {
        if (!v || v & node::THREAD) return;
        if (tree[v].p) {
            tree[v].cx += tree[tree[v].p].cx;
            tree[v].cy += tree[tree[v].p].cy + DY;
        }
        accumulate(tree[v].l); 
        accumulate(tree[v].r);
    }

    void render_node(uint32_t v) {
        std::vector<uint32_t> children;
        if (tree[v].l && !(tree[v].l & node::THREAD)) children.push_back(tree[v].l);
        if (tree[v].r && !(tree[v].r & node::THREAD)) children.push_back(tree[v].r);
        for (uint32_t c : children) {
            svg << "<line "
                << "x1=\"" << tree[v].cx
                << "\" y1=\"" << tree[v].cy
                << "\" x2=\"" << tree[c].cx
                << "\" y2=\"" << tree[c].cy
                << "\" stroke=\"" << EDGE_COLOR
                <<"\" stroke-width=\"" << EDGE_WIDTH << "\""
            << " />";
        }

        svg << "<circle "
            << "cx=\"" << tree[v].cx
            << "\" cy=\"" << tree[v].cy
            << "\" r=\"" << NODE_RADIUS
            << "\" fill=\"" << NODE_COLOR << "\""
        << " />";
    }
public:
    tree_renderer(std::vector<node>& _tree) : tree(_tree) {};

    std::string render() {
        svg << "<svg display:\"block\" width=\"2000\" height=\"1000\" viewBox=\"-1000 -500 2000 1000\" xmlns=\"http://www.w3.org/2000/svg\">";

        divide_and_conquer(1);
        accumulate(1);
        for (uint32_t v = 1; v < tree.size(); ++v) render_node(v);

        svg << "</svg>";
        return svg.str();
    }
};