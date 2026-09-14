import assert from 'node:assert/strict';
import fs from 'node:fs';
import vm from 'node:vm';

const context = vm.createContext({document: {
  createElement: () => ({style: {}, dataset: {}, children: [],
    appendChild(child) { this.children.push(child); },
  }),
}});
vm.runInContext(fs.readFileSync(new URL('./mock.js', import.meta.url), 'utf8'), context);

const edges = value => ({top: value, right: value, bottom: value, left: value});
const size = {dim: 'Pixels', value: 130, strictness: 1};
for (const margin of [0, 2.4, 8, 14, -6]) {
  context.node = {
    id: 1, flex_direction: 'Column', flex_wrap: 'NoWrap', absolute: true,
    padding: edges(0), margin: edges(margin), children: [],
    desired: {x: size, y: size}, rect: {x: 186, y: 211, width: 130, height: 130},
  };
  context.parent = {x: 172, y: 197};
  const el = vm.runInContext("buildNode(node, 1280, 720, 'column', false, parent, false)", context);
  assert.equal(parseFloat(el.style.left) + margin + context.parent.x, context.node.rect.x);
  assert.equal(parseFloat(el.style.top) + margin + context.parent.y, context.node.rect.y);
  assert.equal(el.style.margin, `${margin}px ${margin}px ${margin}px ${margin}px`);
}
console.log('Absolute replay preserves captured origins with zero, positive, and negative CSS margins.');

context.node = {
  id: 2, flex_direction: 'Column', flex_wrap: 'NoWrap', absolute: false,
  padding: edges(0), margin: edges(0), children: [], label: 'One two\nThree',
  text_alignment: 'Left', text_overflow: 'Wrap',
  desired: {x: size, y: {dim: 'Text', value: 0, strictness: 1}},
  rect: {x: 0, y: 0, width: 130, height: 40},
  text: {font: 'Example', file: 'example.ttf', size: 18, line_height: 20,
    spacing: 1, inset_x: 5, inset_y: 5, offset_x: 0, offset_y: 0, explicit_size: true},
};
for (const visible of [false, true]) {
  context.visible = visible;
  const el = vm.runInContext("buildNode(node, 1280, 720, 'column', visible, parent, false)", context);
  assert.equal(el.children.length, 1, 'Hiding labels must preserve their layout contribution');
  const label = el.children[0];
  assert.equal(label.style.position, 'relative', 'Text height must include in-flow text');
  assert.equal(label.style.lineHeight, '20px');
  assert.equal(label.style.whiteSpace, 'pre-wrap');
  assert.equal(label.style.fontFamily, '"Example"');
  assert.equal(label.style.visibility, visible ? 'visible' : 'hidden');
}
console.log('Text-sized labels preserve font metrics and layout when hidden.');

context.node.corner_radius = 12;
context.node.roundness = 0.5;
context.node.corners = [true, true, true, true];
const rounded = vm.runInContext("buildNode(node, 1280, 720, 'column', true, parent, false)", context);
assert.equal(rounded.style.borderRadius, '12px 12px 12px 12px');
context.diffs = [
  {dx: 8, dy: 0, dw: 0, dh: 0, pdx: 8, pdy: 0},
  {dx: 0, dy: 0, dw: 0, dh: 20, pdx: 0, pdy: 0, underText: true},
  {dx: 0, dy: 0, dw: 5, dh: 0, pdx: 0, pdy: 0, absolute: true},
];
const summary = vm.runInContext('summarizeDiffs(diffs, 2)', context);
assert.equal(summary.differing, 3);
assert.equal(summary.candidates, 1);
assert.equal(summary.text, 1);
assert.equal(summary.replayed, 1);
console.log('Pixel radius takes precedence; model limits and inherited offsets are reported separately.');
