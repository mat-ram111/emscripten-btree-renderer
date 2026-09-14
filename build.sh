set -e
em++ src/*.cpp -O3 -lembind -sMODULARIZE=1 -sEXPORT_ES6=1 -o web/tree_renderer.js