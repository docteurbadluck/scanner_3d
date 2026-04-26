import '/js/PageCard.js';
import './components/StatusSection.js';
import './components/PingSection.js';
import './components/LogSection.js';
import './components/TakePhoto.js';

import { isInProgress, isPicoInProgress, start, startPico, cancel, cancelPico } from './ping.js';
import { createMessageHandler, connect } from './ws.js';

const statusSection   = document.getElementById('status-section');
const pingSection     = document.getElementById('ping-section');
const logSection      = document.getElementById('log-section');
const takePhotoButton   = document.getElementById('take-photo-button');
takePhotoButton.label   = 'Take Photo';

const piRef        = { set result(v) { pingSection.piResult     = v; } };
const picoRef      = { set result(v) { pingSection.picoResult   = v; } };
const takePhotoRef = { set result(v) { takePhotoButton.result   = v; } };

const refs = { statusSection, pingSection, logSection, piRef, picoRef, takePhotoRef };

function isTakePhotoAllowed() {
    return statusSection.piState === 'connected'
        && statusSection.systemState !== 'PROCESSING'
        && statusSection.systemState !== 'SENDING';
}

function isPingAllowed() {
    return statusSection.piState === 'connected'
        && statusSection.systemState !== 'PROCESSING'
        && statusSection.systemState !== 'SENDING';
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
    if (!isInProgress())     pingSection.piDisabled   = !isPingAllowed();
    if (!isPicoInProgress()) pingSection.picoDisabled = !isPingPicoAllowed();
    takePhotoButton.disabled = !isTakePhotoAllowed();
}

let ws = null;

function onWsClose() {
    statusSection.piState = statusSection.picoState = statusSection.systemState = null;
    pingSection.piDisabled = pingSection.picoDisabled = true;
    setTimeout(reconnect, 3000);
}

function reconnect() {
    ws = connect(createMessageHandler(refs, updateButtons), onWsClose);
}

takePhotoButton.addEventListener('take-photo', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    takePhotoButton.disabled = true;
    ws.send(JSON.stringify({ type: 'command', command: 'TAKE_PHOTO' }));
});

pingSection.addEventListener('ping-pi', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    pingSection.piDisabled = true;
    start(ws, piRef, () => { cancel(piRef, 'timeout'); updateButtons(); });
});

pingSection.addEventListener('ping-pico', () => {
    if (!ws || ws.readyState !== WebSocket.OPEN) return;
    pingSection.picoDisabled = true;
    startPico(ws, picoRef, () => { cancelPico(picoRef, 'timeout'); updateButtons(); });
});

reconnect();
