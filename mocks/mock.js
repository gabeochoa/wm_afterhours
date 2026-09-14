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
    // Expand is handled by the caller, which knows the parent's direction:
    // flex-grow only grows the MAIN axis, so setting it here for a cross-axis
    // Expand silently grew the wrong dimension.
    case 'Expand':        break;
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

const loadedFonts = new Map();

async function loadMockFonts(tree) {
  const jobs = [];
  const prepare = t => {
    if (!t?.file) return;
    if (!loadedFonts.has(t.font)) {
      const url = '../resources/fonts/' + encodeURIComponent(t.file);
      const face = new FontFace(JSON.stringify(t.font), `url(${JSON.stringify(url)})`);
      loadedFonts.set(t.font, face.load().then(loaded => document.fonts.add(loaded)));
    }
    jobs.push(loadedFonts.get(t.font));
  };
  const visit = n => {
    prepare(n.text);
    (n.text?.spans || []).forEach(prepare);
    n.children.forEach(visit);
  };
  tree.tree.forEach(visit);
  await Promise.all(jobs);
}

function appendLabel(el, n, showLabels) {
  if (!n.label) return;
  const label = document.createElement('span');
  label.className = 'lbl';
  label.textContent = n.label;
  label.style.visibility = showLabels ? 'visible' : 'hidden';
  const t = n.text;
  el.mockText = t;
  if (!t) {
    el.dataset.textModel = 'missing-font-metadata';
    el.appendChild(label);
    return;
  }
  const intrinsic = n.children.length === 0 && [n.desired.x.dim, n.desired.y.dim]
    .some(dim => dim === 'Text' || dim === 'Children');
  el.dataset.textModel = t.explicit_size || intrinsic ? 'font-metrics' : 'auto-fit';
  if (!t.file) el.dataset.textModel = 'missing-font-file';
  if (t.spans?.length) {
    label.textContent = '';
    for (const run of t.spans) {
      const span = document.createElement('span');
      span.textContent = run.text;
      span.style.fontFamily = JSON.stringify(run.font);
      label.appendChild(span);
    }
  }
  label.style.position = intrinsic ? 'relative' : 'absolute';
  label.style.inset = 'auto';
  label.style.display = 'block';
  label.style.flexShrink = '0';
  label.style.boxSizing = 'border-box';
  label.style.fontFamily = JSON.stringify(t.font);
  label.style.fontSize = t.size + 'px';
  label.style.lineHeight = t.line_height + 'px';
  label.style.fontKerning = 'none';
  label.style.fontVariantLigatures = 'none';
  label.style.letterSpacing = t.spacing + 'px';
  label.style.whiteSpace = n.text_overflow === 'Wrap' && t.explicit_size ? 'pre-wrap' : 'pre';
  label.style.overflowWrap = 'normal';
  label.style.wordBreak = 'normal';
  label.style.textAlign = {Left:'left', Right:'right', Center:'center'}[n.text_alignment] || 'center';
  label.style.padding = `0 ${t.inset_x}px`;
  label.style.overflow = 'hidden';
  if (n.text_overflow === 'Ellipsis') label.style.textOverflow = 'ellipsis';
  if (intrinsic) {
    label.style.alignSelf = 'stretch';
    label.style.maxWidth = '100%';
    label.style.left = t.offset_x + 'px';
    label.style.top = t.offset_y + 'px';
  } else {
    label.style.left = t.offset_x + 'px';
    label.style.right = '0';
    label.style.top = `calc(50% + ${t.offset_y}px)`;
    label.style.transform = 'translateY(-50%)';
    label.style.maxHeight = '100%';
  }
  el.appendChild(label);
}

function fitAutoLabels(root) {
  const canvas = document.createElement('canvas');
  const measure = canvas.getContext('2d');
  for (const el of root.querySelectorAll('[data-text-model="auto-fit"]')) {
    const label = el.querySelector(':scope > .lbl');
    const t = el.mockText;
    if (!label || !t) continue;
    const width = Math.max(0, el.clientWidth - 2 * t.inset_x - t.offset_x);
    const height = Math.max(0, el.clientHeight - 2 * t.inset_y);
    const ratio = t.size > 0 ? t.line_height / t.size : 1;
    const lines = label.textContent.split('\n');
    let low = t.minimum_size ?? 16, high = Math.min(height, 200);
    while (high - low > .5) {
      const size = (low + high) / 2;
      measure.font = `${size}px ${JSON.stringify(t.font)}`;
      measure.fontKerning = 'none';
      const widest = Math.max(...lines.map(line => measure.measureText(line).width +
        Math.max(0, Array.from(line).length - 1) * t.spacing));
      if (size * ratio * lines.length <= height &&
          (label.style.textOverflow === 'ellipsis' || widest <= width)) low = size;
      else high = size;
    }
    label.style.fontSize = low + 'px';
    label.style.lineHeight = low * ratio + 'px';
  }
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
  // border-box instead floors the box at the padding, so a 10px box with 25.6px
  // of side padding comes out 51.2 wide. Drop the padding in that case: the
  // content area is zero either way, and the box size is what is being compared.
  const p = { ...n.padding }, m = n.margin;
  const stated = (size, screen) =>
    size.dim === 'Pixels' ? size.value
      : size.dim === 'ScreenPercent' ? size.value * screen
      : null; // Percent needs the parent's resolved size; not worth it yet
  const statedX = stated(n.desired.x, vw), statedY = stated(n.desired.y, vh);
  if (statedX !== null && p.left + p.right >= statedX)
    p.left = p.right = 0;
  if (statedY !== null && p.top + p.bottom >= statedY)
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
  const mainSize = parentDir === 'row' ? n.desired.x : n.desired.y;
  if (mainSize.dim === 'Expand') el.style.flexGrow = mainSize.value;
  if (n.desired.x.dim === 'Expand' && parentDir === 'column') el.style.alignSelf = 'stretch';
  if (n.desired.y.dim === 'Expand' && parentDir === 'row')    el.style.alignSelf = 'stretch';
  if (n.self_align && n.self_align !== 'Auto')
    el.style.alignSelf = ALIGN[n.self_align] || 'auto';

  // raylib roundness is a fraction of the short side, halved; CSS wants px.
  if (n.corners && (n.corner_radius !== undefined || n.roundness)) {
    const r = n.corner_radius ?? n.roundness * Math.min(n.rect.width, n.rect.height) / 2;
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
    el.style.left = (n.rect.x - parentRect.x - m.left) + 'px';
    el.style.top  = (n.rect.y - parentRect.y - m.top) + 'px';
  } else {
    el.style.position = 'relative';
  }

  appendLabel(el, n, showLabels);
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
  fitAutoLabels(root);
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
      underScroll, underText: texty, textModel: el.dataset.textModel || null, pdx, pdy,
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

function summarizeDiffs(diffs, tolerance) {
  const bad = diffs.filter(d => worseThan(d, tolerance));
  return {
    total: diffs.length, differing: bad.length,
    text: bad.filter(d => d.underText).length,
    scroll: bad.filter(d => d.underScroll).length,
    replayed: diffs.filter(d => d.absolute).length,
    candidates: bad.filter(d => !d.underText && !d.underScroll &&
      (Math.abs(d.dw) > tolerance || Math.abs(d.dh) > tolerance ||
       (!d.absolute && (Math.abs(d.dx - d.pdx) > tolerance || Math.abs(d.dy - d.pdy) > tolerance)))).length,
  };
}
