# Description

This web app generates a random binary tree, the size of which is specified by the user, and implements the Reingold-Tilford algorithm to render it. It does so in C++ compiled to Webassembly through Emscripten, allowing for much faster render times than standard Typescript or Javascript would allow. The C++ renderer outputs an SVG string, which the Javascript UI code appends to the DOM. This UI code also takes care of basic panning and zooming, as well as for the user to specify the number of nodes in the tree.

This project is primarily intended to explore the Webassembly/Emscripten framework for performant graphical web apps.
Also, I could not find any good implementations of Reingold-Tilford, and I think the one I wrote (src/renderer.hpp) is a fairly concise and faithful implementation of the original algorithm described here: https://reingold.co/tidier-drawings.pdf

# How to Run

The WASM files are precompiled, so simply run
`python3 -m http.server -d web`
in the terminal, then open http://localhost:8000.

However, the C++ core can be recompiled by running build.sh at the project root. This requires the instiallation of the Emscripten SDK.
