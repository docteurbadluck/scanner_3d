import { cancel, cancelPico, elapsedMs } from './ping.js';

const WS_PORT = 8081;

function handlePiConnection(data, refs, onUpdate) {
    refs.statusSection.piState = data.connected ? 'connected' : 'disconnected';
    if (!data.connected) onUpdate(false);
}

function handlePong(refs) {
    const ms = elapsedMs();
    cancel(refs.piRef, ms !== null ? `PONG — ${ms} ms` : 'PONG');
}

function handlePongPico(data, refs) {
    const msg = data.ms !== undefined ? `PONG — ${data.ms} ms (Pi→Pico)` : 'PONG';
    cancelPico(refs.picoRef, msg);
}

function dispatch(data, refs, onUpdate) {
    if (data.type === 'debug') { refs.logSection.appendLog(data.raw); return; }
    if (data.type === 'pi_connection') { handlePiConnection(data, refs, onUpdate); return; }
    if (data.type === 'pico_status') refs.statusSection.picoState = data.state;
    if (data.type === 'status' || data.type === 'state') refs.statusSection.systemState = data.state;
    const isPong = data.type === 'response' && data.kind === 'PONG' && data.command !== 'PING_PICO';
    if (isPong) handlePong(refs);
    if (data.type === 'response' && data.command === 'PING_PICO') handlePongPico(data, refs);
    onUpdate(true);
}

export function createMessageHandler(refs, onUpdate) {
    return (evt) => dispatch(JSON.parse(evt.data), refs, onUpdate);
}

export function connect(onMessage, onClose) {
    const ws = new WebSocket(`ws://${location.hostname}:${WS_PORT}`);
    ws.onmessage = onMessage;
    ws.onclose = onClose;
    return ws;
}
