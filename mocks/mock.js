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

// strictness has no single CSS spelling, so it needs both halves:
//   strictness 1 = "do not resize me"     -> flex-shrink: 0
//   strictness 0 = "resize me freely"     -> flex-shrink: 1, and tax_refund
//                                            also GROWS these to fill slack
// CSS defaults flex-shrink to 1 and afterhours defaults strictness to 1, so
// leaving this unset had the mock shrinking every overflowing row that
// afterhours deliberately let overflow.
//
// `siblingExpands` is the parent's answer for this axis: tax_refund hands all
// the slack to Expand children and returns, so a loose sibling gets nothing
// when an Expand is present. CSS would have split it between them.
function applyStrictness(el, n, parentDir, siblingExpands) {
  const size = parentDir === 'row' ? n.desired.x : n.desired.y;
  if (size.dim === 'Expand')
    return; // already flex-grow; shrink is irrelevant to it here
  const strictness = Math.min(1, Math.max(0, size.strictness));
  el.style.flexShrink = 1 - strictness;
  if (strictness === 0 && !siblingExpands)
    el.style.flexGrow = 1;
}

// True when any in-flow child of `n` uses Expand along `n`'s main axis.
function hasExpandingChild(n, row) {
  const axis = row ? 'x' : 'y';
  return n.children.some((c) => !c.absolute && c.desired[axis].dim === 'Expand');
}

function buildNode(n, vw, vh, parentDir, showLabels, parentRect, siblingExpands) {
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

  // afterhours clamps the CONTENT area to zero when padding exceeds the box
  // (fmaxf(0, computed - padd)) and keeps the box at its stated size. CSS
  // border-box instead floors the box at the padding, so pixels(40) with 25.6px
  // of side padding comes out 51.2 wide. Drop the padding in that case: the
  // content area is zero either way, and the box size is what is being compared.
  const p = { ...n.padding }, m = n.margin;
  if (n.desired.x.dim === 'Pixels' && p.left + p.right >= n.desired.x.value)
    p.left = p.right = 0;
  if (n.desired.y.dim === 'Pixels' && p.top + p.bottom >= n.desired.y.value)
    p.top = p.bottom = 0;
  el.style.padding = `${p.top}px ${p.right}px ${p.bottom}px ${p.left}px`;
  el.style.margin  = `${m.top}px ${m.right}px ${m.bottom}px ${m.left}px`;

  applySize(el, n.desired.x, 'x', vw, vh);
  applySize(el, n.desired.y, 'y', vw, vh);
  applyStrictness(el, n, parentDir, siblingExpands);

  // A flex item's automatic minimum size is its CONTENT size, so CSS refuses to
  // make a 40px box 40px wide once a label does not fit in it. afterhours has no
  // such floor -- pixels(40) is 40 and the text is clipped -- so the floor has
  // to go or every labelled fixed-size element reads as a disagreement.
  el.style.minWidth = '0';
  el.style.minHeight = '0';

  // flex-grow only grows the main axis. An Expand on the cross axis means
  // "fill the parent" there, which is stretch, not grow.
  if (n.desired.x.dim === 'Expand' && parentDir === 'column') el.style.alignSelf = 'stretch';
  if (n.desired.y.dim === 'Expand' && parentDir === 'row')    el.style.alignSelf = 'stretch';
  if (n.self_align && n.self_align !== 'Auto')
    el.style.alignSelf = ALIGN[n.self_align] || 'auto';

  // raylib roundness is a fraction of the short side, halved; CSS wants px.
  if (n.roundness && n.corners) {
    const r = n.roundness * Math.min(n.rect.width, n.rect.height) / 2;
    const [tl, tr, bl, br] = n.corners;
    el.style.borderRadius =
      `${tl ? r : 0}px ${tr ? r : 0}px ${br ? r : 0}px ${bl ? r : 0}px`;
  }

  if (n.paints) el.style.background = 'rgba(120,170,255,.07)';

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
  const childrenExpand = hasExpandingChild(n, row);
  for (const c of n.children)
    el.appendChild(buildNode(c, vw, vh, row ? 'row' : 'column', showLabels,
                             n.rect, childrenExpand));
  return el;
}

// Fill `stage` with the CSS re-solve of `tree`. Returns the root element.
function mountMock(stage, tree, showLabels) {
  stage.innerHTML = '';
  const root = buildNode(tree.tree[0], tree.viewport.width, tree.viewport.height,
                         'column', showLabels, { x: 0, y: 0 }, false);
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

  // pdx/pdy: the parent's own offset. A child that differs by exactly what its
  // parent already differs by has not found anything new -- it is being carried.
  const walk = (n, el, underScroll, underText, pdx, pdy) => {
    if (!el) return;
    // Self-inclusive, unlike underScroll: a shrink-to-fit label IS the node
    // whose width came from font metrics, and it is always a leaf.
    const sizedToText = (d) => d === 'Text' || d === 'Children';
    const texty = underText || ((n.label || '') !== '' &&
                  (sizedToText(n.desired.x.dim) || sizedToText(n.desired.y.dim)));
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
      underScroll, underText: texty, pdx, pdy,
    });
    const scrolls = underScroll || n.clips || n.scrolls;
    const kids = [...el.children].filter(c => c.classList.contains('node'));
    n.children.forEach((c, i) => walk(c, kids[i], scrolls, texty,
                                      css.x - n.rect.x, css.y - n.rect.y));
  };
  walk(tree.tree[0], stageEl.firstElementChild, false, false, 0, 0);
  return out;
}

const worseThan = (d, tol) =>
  Math.max(Math.abs(d.dx), Math.abs(d.dy), Math.abs(d.dw), Math.abs(d.dh)) > tol;
