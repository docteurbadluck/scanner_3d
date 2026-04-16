import './components/MetricsGrid.js';
import { fetchJson, fmt, pct } from './data.js';

const grid      = document.getElementById('metrics-grid');
const statusMsg = document.getElementById('status-msg');

async function loadMetrics() {
    const data = await fetchJson('../data/hardware_metrics.json');
    grid.successRate = pct(data.reconstruction_success_rate);
    grid.scanTime    = fmt(data.scan_completion_time_seconds, 's');
    grid.procTime    = fmt(data.processing_time_seconds,      's');
    grid.lastScan    = data.last_scan ?? 'n/a';
}

async function loadAll() {
    try {
        await loadMetrics();
        statusMsg.textContent = '';
    } catch {
        statusMsg.textContent = 'No scan data yet.';
    }
}

function scheduleRefresh() { setInterval(loadAll, 30_000); }

document.addEventListener('DOMContentLoaded', () => { loadAll(); scheduleRefresh(); });
