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
