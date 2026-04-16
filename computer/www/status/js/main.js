import './components/StatusSection.js';
import './components/PingSection.js';
import './components/LogSection.js';
import { isInProgress, isPicoInProgress, start, startPico, cancel, cancelPico } from './ping.js';
import { createMessageHandler, connect } from './ws.js';

const statusSection = document.getElementById('status-section');
const pingSection   = document.getElementById('ping-section');
const logSection    = document.getElementById('log-section');

const piRef   = { set result(v) { pingSection.piResult   = v; } };
const picoRef = { set result(v) { pingSection.picoResult = v; } };

const refs = { statusSection, pingSection, logSection, piRef, picoRef };

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
        return;
    }
    if (!isInProgress())     pingSection.piDisabled   = !isPingAllowed();
    if (!isPicoInProgress()) pingSection.picoDisabled = !isPingPicoAllowed();
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
