import './components/MetricsGrid.js';
import { fetchJson, fmt, parseCoverage } from './data.js';

const grid        = document.getElementById('metrics-grid');
const statusMsg   = document.getElementById('status-msg');
const lastUpdated = document.getElementById('last-updated');

async function loadProductivity() {
    const data = await fetchJson('../data/productivity.json');
    grid.leadTime   = fmt(data.lead_time_avg_minutes,        'min');
    grid.deployFreq = fmt(data.deployment_frequency_per_day, '/ day');
    grid.ttr        = fmt(data.time_to_restore_avg_minutes,  'min');
    grid.cfr        = fmt(data.change_failure_rate_pct,      '%');
    lastUpdated.textContent = data.computed_at ?? 'n/a';
}

async function loadCoverages() {
    const [computer, pico, pi] = await Promise.allSettled([
        fetchJson('../data/computer_srcs_coverage.json'),
        fetchJson('../data/pico_coverage.json'),
        fetchJson('../data/pi_coverage.json'),
    ]);
    grid.coverageComputer = computer.status === 'fulfilled' ? parseCoverage(computer.value) : 'n/a';
    grid.coveragePico     = pico.status     === 'fulfilled' ? parseCoverage(pico.value)     : 'n/a';
    grid.coveragePi       = pi.status       === 'fulfilled' ? parseCoverage(pi.value)       : 'n/a';
}

async function loadAll() {
    try {
        await loadProductivity();
        statusMsg.textContent = '';
    } catch {
        statusMsg.textContent = 'No data \u2014 run aggregate.py first.';
    }
    await loadCoverages();
}

function scheduleRefresh() { setInterval(loadAll, 60_000); }

document.addEventListener('DOMContentLoaded', () => { loadAll(); scheduleRefresh(); });
