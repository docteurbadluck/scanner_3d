const TIMEOUT_MS = 5000;

let _ts = null,     _timeout = null;
let _picoTs = null, _picoTimeout = null;

export const isInProgress     = () => _timeout !== null;
export const isPicoInProgress = () => _picoTimeout !== null;
export const elapsedMs        = () => _ts !== null ? Date.now() - _ts : null;

export function cancel(el, msg) {
    clearTimeout(_timeout);
    _timeout = null;
    _ts = null;
    el.result = msg;
}

export function cancelPico(el, msg) {
    clearTimeout(_picoTimeout);
    _picoTimeout = null;
    _picoTs = null;
    el.result = msg;
}

export function start(ws, el, onTimeout) {
    _ts = Date.now();
    el.result = '…';
    ws.send(JSON.stringify({ type: 'command', command: 'PING' }));
    _timeout = setTimeout(onTimeout, TIMEOUT_MS);
}

export function startPico(ws, el, onTimeout) {
    _picoTs = Date.now();
    el.result = '…';
    ws.send(JSON.stringify({ type: 'command', command: 'PING_PICO' }));
    _picoTimeout = setTimeout(onTimeout, TIMEOUT_MS);
}
