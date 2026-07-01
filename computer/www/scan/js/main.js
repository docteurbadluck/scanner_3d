import '/js/PageCard.js';
import './components/NewScanForm.js';
import './components/ScanList.js';
import * as Ws from './ws.js';

const form     = document.getElementById('new-scan-form');
const scanList = document.getElementById('scan-list');
const refs     = { form, scanList };

let ws = null;

function onWsClose() {
    setTimeout(reconnect, 3000);
}

function reconnect() {
    ws = Ws.connect(Ws.createMessageHandler(refs), onWsClose);
}

form.addEventListener('start-scan', (e) => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    form.disabled = true;
    ws.send(JSON.stringify({ type: 'start_scan', name: e.detail }));
});

scanList.addEventListener('continue-scan', (e) => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    ws.send(JSON.stringify({ type: 'continue_scan', name: e.detail }));
});

reconnect();
