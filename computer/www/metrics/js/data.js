export async function fetchJson(url) {
    return fetch(url).then(r => r.json());
}

export function fmt(val, unit) {
    return val != null ? `${val}\u00a0${unit}` : 'n/a';
}

export function pct(val) {
    return val != null ? `${(val * 100).toFixed(1)}\u00a0%` : 'n/a';
}
