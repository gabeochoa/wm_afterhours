// intent -> CSS. Shared by index.html (one screen, interactive) and sheet.html
// (every screen, for a static side-by-side contact sheet).
//
// Anything not expressible is left unset so the browser's own default applies;
// forcing a guess here would manufacture diffs.

const JUSTIFY = { FlexStart:'flex-start', FlexEnd:'flex-end', Center:'center',
                  SpaceBetween:'space-between', SpaceAround:'space-around' };
const ALIGN   = { FlexStart:'flex-start', FlexEnd:'flex-end', Center:'center',
                  Stretch:'stretch' };

// vw/vh here are the captured viewport, not the browser window.
function applySize(el, size, axis, vw, vh) {
  const prop = axis === 'x' ? 'width' : 'height';
  const v = size.value;
  switch (size.dim) {
    case 'Pixels':        el.style[prop] = v + 'px'; break;
    case 'Percent':       el.style[prop] = (v * 100) + '%'; break;
    case 'ScreenPercent': el.style[prop] = (v * (axis === 'x' ? vw : vh)) + 'px'; break;
    case 'Children':      el.style[prop] = 'fit-content'; break;
    // Expand is flex-grow, but only along the parent's main axis; the caller
    // fixes up the cross axis once it knows the direction.
    case 'Expand':        el.style['flex-grow'] = v; break;
    case 'Text':          el.style[prop] = 'fit-content'; break;
  }
}

function buildNode(n, vw, vh, parentDir, showLabels, parentRect) {
  const el = document.createElement('div');
  el.className = 'node';
  el.dataset.id = n.id;
  el.dataset.name = n.name || '';

  const row = n.flex_direction === 'Row';
  el.style.display = 'flex';
  el.style.flexDirection = row ? 'row' : 'column';
  el.style.justifyContent = JUSTIFY[n.justify_content] || 'flex-start';
  el.style.alignItems = ALIGN[n.align_items] || 'flex-start';
  el.style.flexWrap = n.flex_wrap === 'NoWrap' ? 'nowrap' : 'wrap';
  if (n.gap) el.style.gap = n.gap + 'px';

  const p = n.padding, m = n.margin;
  el.style.padding = `${p.top}px ${p.right}px ${p.bottom}px ${p.left}px`;
  el.style.margin  = `${m.top}px ${m.right}px ${m.bottom}px ${m.left}px`;

  applySize(el, n.desired.x, 'x', vw, vh);
  applySize(el, n.desired.y, 'y', vw, vh);

  // flex-grow only grows the main axis. An Expand on the cross axis means
  // "fill the parent" there, which is stretch, not grow.
  if (n.desired.x.dim === 'Expand' && parentDir === 'column') el.style.alignSelf = 'stretch';
  if (n.desired.y.dim === 'Expand' && parentDir === 'row')    el.style.alignSelf = 'stretch';
  if (n.self_align && n.self_align !== 'Auto')
    el.style.alignSelf = ALIGN[n.self_align] || 'auto';

  if (n.absolute) {
    // Replayed, not re-solved: an absolute position is a coordinate the caller
    // handed in, so there is no second opinion for CSS to have. Offset against
    // the parent because CSS left/top is parent-relative while the dump's
    // relative_pos has already been walked up to screen space.
    el.style.position = 'absolute';
    el.style.left = (n.rect.x - parentRect.x) + 'px';
    el.style.top  = (n.rect.y - parentRect.y) + 'px';
  } else {
    el.style.position = 'relative';
  }

  if (showLabels && n.label) {
    const s = document.createElement('span');
    s.className = 'lbl';
    s.textContent = n.label;
    el.appendChild(s);
  }
  for (const c of n.children)
    el.appendChild(buildNode(c, vw, vh, row ? 'row' : 'column', showLabels, n.rect));
  return el;
}

// Fill `stage` with the CSS re-solve of `tree`. Returns the root element.
function mountMock(stage, tree, showLabels) {
  stage.innerHTML = '';
  const root = buildNode(tree.tree[0], tree.viewport.width, tree.viewport.height,
                         'column', showLabels, { x: 0, y: 0 });
  root.style.position = 'absolute';
  root.style.left = '0'; root.style.top = '0';
  stage.appendChild(root);
  return root;
}

// Walk the dump and the DOM in lockstep, comparing afterhours' rect to the one
// CSS produced for the same node.
function collectDiffs(stageEl, tree) {
  const stage = stageEl.getBoundingClientRect();
  const scale = (stage.width / tree.viewport.width) || 1;
  const out = [];

  const walk = (n, el, underScroll, underText) => {
    if (!el) return;
    const b = el.getBoundingClientRect();
    const css = { x:(b.left - stage.left)/scale, y:(b.top - stage.top)/scale,
                  width:b.width/scale, height:b.height/scale };
    out.push({
      name: n.name || `#${n.id}`,
      ah: n.rect, css,
      dx: css.x - n.rect.x, dy: css.y - n.rect.y,
      dw: css.width - n.rect.width, dh: css.height - n.rect.height,
      desired: `${n.desired.x.dim}/${n.desired.y.dim}`,
      dir: n.flex_direction, justify: n.justify_content,
      absolute: n.absolute,
      // Why CSS is allowed to disagree here, if it is:
      //   scroll  - the mock has no clipping, so it shows content afterhours hid
      //   text    - browser font metrics are not raylib's, so Text/Children drift
      underScroll, underText,
    });
    const scrolls = underScroll || n.clips || n.scrolls;
    const texty = underText || n.desired.x.dim === 'Text' || n.desired.y.dim === 'Text';
    const kids = [...el.children].filter(c => c.classList.contains('node'));
    n.children.forEach((c, i) => walk(c, kids[i], scrolls, texty));
  };
  walk(tree.tree[0], stageEl.firstElementChild, false, false);
  return out;
}

const worseThan = (d, tol) =>
  Math.max(Math.abs(d.dx), Math.abs(d.dy), Math.abs(d.dw), Math.abs(d.dh)) > tol;
