import '/js/PageCard.js';
import './components/StatusSection.js';
import './components/PingSection.js';
import './components/LogSection.js';
import './components/TakePhoto.js';

import * as Ping from './ping.js';
import * as Ws from './ws.js';

const statusSection     = document.getElementById('status-section');
const pingSection       = document.getElementById('ping-section');
const logSection        = document.getElementById('log-section');
const takePhotoButton   = document.getElementById('take-photo-button');
takePhotoButton.label   = 'Take Photo';

const piRef        = { set result(v) { pingSection.piResult     = v; } };
const picoRef      = { set result(v) { pingSection.picoResult   = v; } };
const takePhotoRef = { set result(v) { takePhotoButton.result   = v; } };

const refs = { statusSection, pingSection, logSection, piRef, picoRef, takePhotoRef };

function isTakePhotoAllowed() {
    return statusSection.piState === 'connected'
        && statusSection.systemState === 'READY';
}

function isPingAllowed() {
    return statusSection.piState === 'connected'
        && statusSection.systemState === 'READY';
}

function isPingPicoAllowed() {
    return statusSection.piState === 'connected' && statusSection.picoState === 'LISTENING';
}

function updateButtons(piConnected = true) {
    if (!piConnected) {
        statusSection.systemState = statusSection.picoState = null;
        pingSection.piDisabled = pingSection.picoDisabled = true;
        takePhotoButton.disabled = true;
        return;
    }
    if (!Ping.isInProgress())     pingSection.piDisabled   = !isPingAllowed();
    if (!Ping.isPicoInProgress()) pingSection.picoDisabled = !isPingPicoAllowed();
    takePhotoButton.disabled = !isTakePhotoAllowed();
}

let ws = null;

function onWsClose() {
    statusSection.piState = statusSection.picoState = statusSection.systemState = null;
    pingSection.piDisabled = pingSection.picoDisabled = true;
    setTimeout(reconnect, 3000);
}

function reconnect() {
    ws = Ws.connect(Ws.createMessageHandler(refs, updateButtons), onWsClose);
}

takePhotoButton.addEventListener('take-photo', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    takePhotoButton.disabled = true;
    ws.send(JSON.stringify({ type: 'command', command: 'TAKE_PHOTO' }));
});

pingSection.addEventListener('ping-pi', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    pingSection.piDisabled = true;
    Ping.start(ws, piRef, () => { Ping.cancel(piRef, 'timeout'); updateButtons(); });
});

pingSection.addEventListener('ping-pico', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    pingSection.picoDisabled = true;
    Ping.startPico(ws, picoRef, () => { Ping.cancelPico(picoRef, 'timeout'); updateButtons(); });
});

reconnect();
