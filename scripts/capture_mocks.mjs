import fs from 'node:fs/promises';
import path from 'node:path';
import {fileURLToPath, pathToFileURL} from 'node:url';
import {parseArgs} from 'node:util';

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const {values} = parseArgs({options: {
  url: {type: 'string', default: pathToFileURL(path.join(root, 'mocks.html')).href},
  port: {type: 'string', default: '9322'},
  output: {type: 'string', default: path.join(root, 'output/screen-audit/mock-targets')},
  screen: {type: 'string'},
  comparisons: {type: 'boolean', default: false},
}});
const version = await (await fetch(`http://127.0.0.1:${values.port}/json/version`)).json();

async function connect(url) {
  const socket = new WebSocket(url);
  await new Promise((resolve, reject) => {
    socket.addEventListener('open', resolve, {once: true});
    socket.addEventListener('error', reject, {once: true});
  });
  let nextId = 0;
  const pending = new Map();
  const errors = [];
  socket.addEventListener('message', event => {
    const message = JSON.parse(event.data);
    if (message.id) {
      const {resolve, reject} = pending.get(message.id);
      pending.delete(message.id);
      message.error ? reject(message.error) : resolve(message.result);
    } else if (message.method === 'Runtime.exceptionThrown') {
      errors.push(message.params.exceptionDetails);
    }
  });
  return {socket, errors, call(method, params = {}) {
    return new Promise((resolve, reject) => {
      const id = ++nextId;
      pending.set(id, {resolve, reject});
      socket.send(JSON.stringify({id, method, params}));
    });
  }};
}

const browser = await connect(version.webSocketDebuggerUrl);
const {targetId} = await browser.call('Target.createTarget', {url: 'about:blank'});
let page;
try {
  const tabs = await (await fetch(`http://127.0.0.1:${values.port}/json/list`)).json();
  page = await connect(tabs.find(tab => tab.id === targetId).webSocketDebuggerUrl);
  const evaluate = async expression => {
    const result = await page.call('Runtime.evaluate', {expression, returnByValue: true, awaitPromise: true});
    if (result.exceptionDetails) throw new Error(JSON.stringify(result.exceptionDetails));
    return result.result.value;
  };
  await page.call('Runtime.enable');
  await page.call('Emulation.setDeviceMetricsOverride', {width: 1280, height: 720, deviceScaleFactor: 1, mobile: false});
  await page.call('Page.navigate', {url: values.url});
  let ready = false;
  for (let tries = 0; tries < 100; tries++) {
    ready = await evaluate('Boolean(window.mockScreens?.length && window.show)');
    if (ready) break;
    await new Promise(resolve => setTimeout(resolve, 100));
  }
  if (!ready) throw new Error('Mock gallery did not load');
  await evaluate(`document.querySelector('.gallery-bar').style.display='none';document.querySelector('.gallery-footer').style.display='none';document.getElementById('workspace').style.padding='0';fit();document.fonts.ready.then(()=>true)`);
  const screens = await evaluate('window.mockScreens.map(({id,title,source,reference})=>({id,title,source,hasReference:Boolean(reference)}))');
  const selected = values.screen ? screens.filter(screen => screen.id === values.screen) : screens;
  if (!selected.length) throw new Error(`Unknown screen: ${values.screen}`);
  await fs.mkdir(values.output, {recursive: true});
  const failures = [];
  for (const screen of selected) {
    await evaluate(`show(${JSON.stringify(screen.id)});document.fonts.ready.then(()=>true)`);
    await evaluate('new Promise(resolve=>requestAnimationFrame(()=>requestAnimationFrame(resolve)))');
    const bounds = await evaluate(`(()=>{const r=document.getElementById('stage').getBoundingClientRect();return {x:r.x,y:r.y,width:r.width,height:r.height,scale:1}})()`);
    if (bounds.width !== 1280 || bounds.height !== 720) throw new Error('Mock canvas is not 1280×720');
    const capture = await page.call('Page.captureScreenshot', {format: 'png', clip: bounds});
    await fs.writeFile(path.join(values.output, `${screen.id}.png`), Buffer.from(capture.data, 'base64'));
    if (values.comparisons) {
      for (const view of screen.hasReference ? ['original', 'current'] : ['current']) {
        const loaded = await evaluate(`(async()=>{show(${JSON.stringify(screen.id)},'${view}');const image=document.querySelector('.gallery-image');if(!image.complete)await new Promise(r=>{image.onload=r;image.onerror=r});return image.naturalWidth>0})()`);
        if (!loaded) failures.push(`${screen.id}: ${view} image failed`);
      }
    }
  }
  await fs.writeFile(path.join(values.output, 'screens.json'), JSON.stringify(screens, null, 2) + '\n');
  if (page.errors.length || failures.length) throw new Error(JSON.stringify({errors: page.errors, failures}));
  console.log(`${selected.length} mock canvases captured at 1280×720; ${values.comparisons ? 'all comparison images loaded; ' : ''}no JavaScript errors.`);
} finally {
  await browser.call('Target.closeTarget', {targetId});
  page?.socket.close();
  browser.socket.close();
}
