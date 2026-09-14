import createModule from './tree_renderer.js'

const tree = document.getElementById('tree');
const Module = await createModule();

let svg = null;
let view_box = null;
let on_reset = null;

const apply_transform = () => svg.setAttribute(
    'viewBox', `${view_box.x} ${view_box.y} ${view_box.w} ${view_box.h}`
);

const count = document.getElementById('count');

function init(n) {
    if (!n || n < 1) return;
    tree.innerHTML = Module.render_tree(n);
    svg = tree.querySelector('svg');
    const [x, y, w, h] = svg.getAttribute('viewBox').trim().split(/\s+/).map(Number);
    if (!on_reset) on_reset = {x, y, w, h};
    if (!view_box) view_box = {...on_reset};
    apply_transform();
}

function reset() {
    view_box = {...on_reset};
    apply_transform();
}

const redraw = () => init(count.valueAsNumber);

document.getElementById('draw').addEventListener('click', redraw);
document.getElementById('reset').addEventListener('click', reset);
count.addEventListener('keydown', e => { if (e.key === 'Enter') redraw(); });

redraw();

tree.addEventListener('wheel', e => {
    e.preventDefault();

    const k = Math.exp(e.deltaY * .001);
    const p = new DOMPoint(e.clientX, e.clientY).matrixTransform(svg.getScreenCTM().inverse());

    view_box.x = p.x - (p.x - view_box.x) * k;
    view_box.y = p.y - (p.y - view_box.y) * k;
    view_box.w *= k;
    view_box.h *= k;

    apply_transform();
});

let drag = null;

tree.addEventListener('pointerdown', e => {
    if (e.button != 0) return;
    const inv = svg.getScreenCTM().inverse();
    drag = { x0 : e.clientX, y0 : e.clientY, view_box0 : {...view_box}, sx : inv.a, sy : inv.d};
    tree.setPointerCapture(e.pointerId);
    tree.classList.add('dragging');
});

tree.addEventListener('pointermove', e => {
    if (!drag) return;
    view_box.x = drag.view_box0.x - (e.clientX - drag.x0) * drag.sx;
    view_box.y = drag.view_box0.y - (e.clientY - drag.y0) * drag.sy;
    apply_transform(); 
})

for (const type of ['pointerup', 'pointercancel']) {
    tree.addEventListener(type, e => {
        if (!drag) return;
        drag = null;
        tree.releasePointerCapture(e.pointerId);
        tree.classList.remove('dragging');
    })
}