const WS_PORT = 8081;

function mergeScan(scans, incoming) {
    const idx = scans.findIndex(s => s.name === incoming.name);
    if (idx === -1) return [...scans, incoming];
    return scans.map((s, i) => i === idx ? incoming : s);
}

function dispatch(data, refs) {
    if (data.type === 'scan_list') refs.scanList.scans = data.scans;
    if (data.type === 'scan_state') refs.scanList.scans = mergeScan(refs.scanList.scans, data);
    if (data.type === 'ack' || data.type === 'error') refs.form.disabled = false;
}

export function createMessageHandler(refs) {
    return (evt) => dispatch(JSON.parse(evt.data), refs);
}

export function connect(onMessage, onClose) {
    const ws = new WebSocket(`ws://${location.hostname}:${WS_PORT}`);
    ws.onmessage = onMessage;
    ws.onclose = onClose;
    return ws;
}
