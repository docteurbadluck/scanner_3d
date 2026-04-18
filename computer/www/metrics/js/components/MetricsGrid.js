import { LitElement, html } from 'https://esm.sh/lit';
import './MetricCard.js';

class MetricsGrid extends LitElement {
    static properties = {
        successRate: {}, scanTime: {}, procTime: {}, lastScan: {},
    };

    createRenderRoot() { return this; }

    constructor() {
        super();
        this.successRate = '—'; this.scanTime = '—';
        this.procTime    = '—'; this.lastScan = '—';
    }

    render() {
        return html`
            <div class="grid grid-cols-2 gap-4 mt-4">
                <metric-card label="Reconstruction Success" .value=${this.successRate}></metric-card>
                <metric-card label="Scan Time"              .value=${this.scanTime}></metric-card>
                <metric-card label="Processing Time"        .value=${this.procTime}></metric-card>
                <metric-card label="Last Scan"              .value=${this.lastScan}></metric-card>
            </div>`;
    }
}

customElements.define('metrics-grid', MetricsGrid);
