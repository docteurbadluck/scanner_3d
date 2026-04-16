export async function fetchJson(url) {
    return fetch(url).then(r => r.json());
}

export function fmt(val, unit) {
    return val != null ? `${val}\u00a0${unit}` : 'n/a';
}

export function parseCoverage(data) {
    const val = data?.totals?.percent_covered;
    return val != null ? `${val.toFixed(1)}\u00a0%` : 'n/a';
}
