const audit = window.designAudit;
const byId = id => document.getElementById(id);
const storageKey = 'wm-design-audit-queue-v1';
let queue = new Set();
try {
  const saved = JSON.parse(localStorage.getItem(storageKey) || '[]');
  if (Array.isArray(saved)) queue = new Set(saved.filter(key => typeof key === 'string'));
} catch {
  byId('notice').textContent = 'Browser storage is unavailable. Export your queue before closing this page.';
}
let active, visibleScreens = [], view = 'current', comparison = '', actualSize = false;
const searchable = new Map(audit.screens.map(screen => [screen.id,
  [screen.id.replaceAll('_', ' '), screen.id, screen.summary,
    ...screen.findings.flatMap(item => [item.area, item.observation, item.change])].join(' ').toLowerCase()
]));
const queueKey = (screen, item) => `${screen.id}:${item.sha256 || screen.sha256}:${item.id}`;
const title = screen => screen.id.replaceAll('_', ' ');
const allFindings = screen => screen.findings.concat(screen.legacy.flatMap(capture =>
  capture.additional_findings.map(item => ({...item, id: `${capture.id}-${item.id}`, legacy: capture.id,
    baseline: capture.baseline, sha256: capture.sha256}))));

function node(tag, text, className) {
  const element = document.createElement(tag);
  if (text !== undefined) element.textContent = text;
  if (className) element.className = className;
  return element;
}

function updateURL(finding) {
  const params = new URLSearchParams({screen: active.id, view});
  if (comparison) params.set('target', comparison);
  if (finding) params.set('finding', finding);
  history.replaceState(null, '', `#${params}`);
}

function filterScreens(preferred) {
  const words = byId('search').value.toLowerCase().trim().split(/\s+/).filter(Boolean);
  const scope = byId('scope').value;
  visibleScreens = audit.screens.filter(screen => {
    if (scope === 'mock' && !screen.has_mock) return false;
    if (scope === 'legacy' && !screen.legacy.length) return false;
    if (scope === 'short' && screen.findings.length >= 25) return false;
    if (scope === 'queued' && !allFindings(screen).some(item => queue.has(queueKey(screen, item)))) return false;
    return words.every(word => searchable.get(screen.id).includes(word));
  });
  const options = visibleScreens.map(screen => {
    const option = node('option', `${title(screen)} · ${screen.findings.length}`);
    option.value = screen.id;
    return option;
  });
  byId('screen-list').replaceChildren(...options);
  byId('screen-count').textContent = `${options.length} of ${audit.screens.length} screens`;
  byId('screen-content').hidden = !options.length;
  byId('no-screens').hidden = !!options.length;
  if (!options.length) return;
  const chosen = visibleScreens.find(screen => screen.id === preferred)
    || visibleScreens.find(screen => screen.id === active?.id) || visibleScreens[0];
  selectScreen(chosen.id);
}

function targetsFor(screen) {
  const targets = [];
  if (screen.evidence) targets.push({id: 'reviewed', label: 'Before design updates', path: screen.evidence});
  if (screen.has_mock) targets.push({id: 'mock', label: 'Web mock'});
  if (screen.reference) targets.push({id: 'source', label: 'Original source image', path: screen.reference});
  targets.push(...(screen.examples || []));
  for (const capture of screen.legacy) targets.push({
    id: capture.id, label: `Older: ${capture.id.replaceAll('_', ' ')}`, path: capture.baseline, capture
  });
  return targets;
}

function selectScreen(id) {
  active = audit.screens.find(screen => screen.id === id);
  if (!active) return;
  byId('screen-list').value = active.id;
  byId('screen-title').textContent = title(active);
  byId('summary').textContent = active.summary;
  document.title = `${title(active)} · Baseline design review`;
  const targets = targetsFor(active);
  if (!targets.some(target => target.id === comparison)) comparison = targets[0]?.id || '';
  byId('target').replaceChildren(...targets.map(target => {
    const option = node('option', target.label);
    option.value = target.id;
    return option;
  }));
  if (!targets.length) {
    const option = node('option', 'No comparison available');
    option.value = '';
    byId('target').append(option);
    view = 'current';
  }
  byId('target').disabled = !targets.length;
  byId('target').value = comparison;
  byId('full-image').href = active.baseline;
  byId('open-mock').hidden = !active.has_mock;
  byId('open-mock').href = `mocks.html#screen=${active.id}&view=mock`;
  const index = visibleScreens.indexOf(active);
  byId('previous').disabled = index <= 0;
  byId('next').disabled = index >= visibleScreens.length - 1;
  renderPreview();
  renderFindings();
  updateURL();
}

function image(path, alt, className) {
  const element = node('img', undefined, className);
  element.src = path;
  element.alt = alt;
  element.addEventListener('error', () => {
    byId('image-note').textContent = `Image failed to load: ${path}. Keep this page beside the repository folders.`;
  });
  return element;
}

function renderPreview() {
  const preview = byId('preview');
  const target = targetsFor(active).find(item => item.id === comparison);
  preview.replaceChildren();
  preview.classList.toggle('actual-size', actualSize);
  preview.classList.toggle('comparing', view === 'compare');
  byId('compare-controls').hidden = view !== 'compare';
  document.querySelectorAll('[data-view]').forEach(button => {
    button.setAttribute('aria-pressed', String(button.dataset.view === view));
    button.disabled = button.dataset.view !== 'current' && !target;
  });
  if (view === 'current' || !target) {
    preview.append(image(active.baseline, `Committed C++ baseline: ${title(active)}`));
  } else if (target.id === 'mock') {
    const frame = node('iframe');
    frame.title = `Interactive web mock: ${title(active)}`;
    frame.src = `mocks.html?embed=1#screen=${active.id}&view=mock`;
    frame.tabIndex = view === 'compare' ? -1 : 0;
    preview.append(frame);
  } else {
    preview.append(image(target.path, target.label));
  }
  if (view === 'compare') {
    preview.append(image(active.baseline, `Current C++: ${title(active)}`, 'baseline-overlay'));
    const divider = node('div', undefined, 'divider');
    divider.append(node('span', '↔'));
    preview.append(divider, node('span', 'Current C++', 'tag left'), node('span', target.label, 'tag right'));
    updateReveal();
  }
  const note = active.evidence ? 'Findings describe the image before design updates. Compare both versions; completion is tracked in todo.md.'
    : active.has_mock ? 'Web mock available. Compare matching states before judging a mismatch.'
    : 'No handcrafted web mock has been authored for this screen. This image is the C++ baseline.';
  byId('image-note').textContent = target?.capture && view !== 'current' ? target.capture.summary : note;
}

function updateReveal() {
  const percent = Number(byId('reveal').value);
  const overlay = byId('preview').querySelector('.baseline-overlay');
  const divider = byId('preview').querySelector('.divider');
  if (overlay) overlay.style.clipPath = `inset(0 ${100 - percent}% 0 0)`;
  if (divider) divider.style.left = `${percent}%`;
  byId('reveal-value').value = `${percent}%`;
  byId('reveal').setAttribute('aria-valuetext', `${percent}% current C++`);
}

function renderFindings() {
  const kind = byId('kind').value, priority = byId('priority').value;
  const findings = allFindings(active).filter(item =>
    (kind === 'all' || item.kind === kind) && (priority === 'all' || item.priority === priority) &&
    (!byId('queued-only').checked || queue.has(queueKey(active, item))));
  const cards = findings.map(item => {
    const queued = queue.has(queueKey(active, item));
    const card = node('article', undefined, `finding${queued ? ' queued' : ''}`);
    card.id = `finding-${item.id}`;
    const head = node('div', undefined, 'finding-head');
    const link = node('a', `#${item.id}`);
    link.href = `#${new URLSearchParams({screen: active.id, view, target: comparison, finding: item.id})}`;
    const priorityBadge = node('span', item.priority, `badge ${item.priority}`);
    const kindBadge = node('span', item.kind === 'defect' ? 'Visible defect' : 'Proposal', `badge ${item.kind}`);
    const label = node('label'), checkbox = node('input');
    checkbox.type = 'checkbox';
    checkbox.checked = queued;
    checkbox.setAttribute('aria-label', `Queue ${active.id} ${item.id}: ${item.area}`);
    checkbox.addEventListener('change', () => {
      const key = queueKey(active, item);
      if (checkbox.checked) queue.add(key); else queue.delete(key);
      card.classList.toggle('queued', checkbox.checked);
      try { localStorage.setItem(storageKey, JSON.stringify([...queue])); }
      catch { byId('notice').textContent = 'Queue could not be saved in this browser. Export it before closing.'; }
      updateTotals();
      if (byId('scope').value === 'queued') filterScreens(active.id);
      else if (byId('queued-only').checked) renderFindings();
    });
    label.append(checkbox, 'Queue');
    head.append(link, priorityBadge, kindBadge, label);
    card.append(head, node('h4', item.area), node('p', item.observation), node('p', item.change, 'change'));
    if (item.legacy) card.append(node('p', `Additional capture: ${item.legacy}`, 'count'));
    return card;
  });
  byId('findings').replaceChildren(...cards);
  const all = allFindings(active).length;
  byId('finding-count').textContent = `${findings.length} of ${all} findings`;
  byId('no-findings').hidden = !!cards.length;
}

function updateTotals() {
  const queued = audit.screens.reduce((sum, screen) => sum + allFindings(screen).filter(item => queue.has(queueKey(screen, item))).length, 0);
  const data = audit.metadata;
  byId('totals').textContent = `${data.screens} screens · ${data.findings + data.legacy_findings} findings · ${data.legacy_captures} older captures · ${queued} queued`;
}

function move(direction) {
  const index = visibleScreens.indexOf(active) + direction;
  if (index < 0 || index >= visibleScreens.length) return;
  selectScreen(visibleScreens[index].id);
}

function restore() {
  const params = new URLSearchParams(location.hash.slice(1));
  const requested = params.get('screen');
  view = ['current', 'target', 'compare'].includes(params.get('view')) ? params.get('view') : 'current';
  comparison = params.get('target') || '';
  if (requested && !visibleScreens.some(screen => screen.id === requested)) {
    byId('search').value = '';
    byId('scope').value = 'all';
  }
  const finding = params.get('finding');
  if (finding) {
    byId('kind').value = 'all';
    byId('priority').value = 'all';
    byId('queued-only').checked = false;
  }
  active = audit.screens.find(screen => screen.id === requested);
  filterScreens(requested);
  if (!finding) return;
  const card = byId(`finding-${finding}`);
  if (card) { card.scrollIntoView({block: 'center'}); updateURL(finding); }
}

byId('search').addEventListener('input', () => filterScreens(active?.id));
byId('scope').addEventListener('change', () => filterScreens(active?.id));
byId('screen-list').addEventListener('change', event => selectScreen(event.target.value));
byId('previous').addEventListener('click', () => move(-1));
byId('next').addEventListener('click', () => move(1));
byId('target').addEventListener('change', event => { comparison = event.target.value; renderPreview(); updateURL(); });
byId('zoom').addEventListener('click', () => {
  actualSize = !actualSize;
  byId('zoom').setAttribute('aria-pressed', String(actualSize));
  byId('zoom').textContent = actualSize ? 'Fit to window' : 'View at 100%';
  byId('preview').classList.toggle('actual-size', actualSize);
});
document.querySelectorAll('[data-view]').forEach(button => button.addEventListener('click', () => {
  view = button.dataset.view; renderPreview(); updateURL();
}));
for (const id of ['kind', 'priority', 'queued-only']) byId(id).addEventListener('change', renderFindings);
byId('reveal').addEventListener('input', updateReveal);
function drag(event) {
  const rect = byId('preview').getBoundingClientRect();
  byId('reveal').value = Math.round(Math.max(0, Math.min(100, (event.clientX - rect.left) / rect.width * 100)));
  updateReveal();
}
byId('preview').addEventListener('pointerdown', event => {
  if (view !== 'compare' || event.button !== 0) return;
  byId('preview').setPointerCapture(event.pointerId);
  drag(event);
});
byId('preview').addEventListener('pointermove', event => {
  if (byId('preview').hasPointerCapture(event.pointerId)) drag(event);
});
byId('preview').addEventListener('pointerup', event => {
  if (byId('preview').hasPointerCapture(event.pointerId)) byId('preview').releasePointerCapture(event.pointerId);
});
document.addEventListener('keydown', event => {
  if (event.target.closest('input,select,textarea,[contenteditable]')) return;
  if (event.key === '[' || event.key === ']') { event.preventDefault(); move(event.key === '[' ? -1 : 1); }
});
byId('export').addEventListener('click', () => {
  const findings = audit.screens.flatMap(screen => allFindings(screen)
    .filter(item => queue.has(queueKey(screen, item)))
    .map(item => ({screen: screen.id, baseline: screen.evidence || screen.baseline, sha256: screen.sha256, ...item})));
  const blob = new Blob([JSON.stringify({audit_date: audit.metadata.date, findings}, null, 2) + '\n'], {type: 'application/json'});
  const url = URL.createObjectURL(blob), link = node('a');
  link.href = url; link.download = 'wm-design-review-queue.json'; link.click();
  setTimeout(() => URL.revokeObjectURL(url), 1000);
  byId('notice').textContent = `Exported ${findings.length} queued findings.`;
});
addEventListener('hashchange', restore);
updateTotals();
restore();
