import { LitElement, html } from 'https://esm.sh/lit';
import './MetricCard.js';

class MetricsGrid extends LitElement {
    static properties = {
        leadTime: {}, deployFreq: {}, ttr: {}, cfr: {},
        coverageComputer: {}, coveragePico: {}, coveragePi: {},
    };

    createRenderRoot() { return this; }

    constructor() {
        super();
        this.leadTime = '—'; this.deployFreq = '—';
        this.ttr = '—';      this.cfr = '—';
        this.coverageComputer = '—'; this.coveragePico = '—'; this.coveragePi = '—';
    }

    render() {
        return html`
            <div class="grid grid-cols-2 gap-4 mt-4">
                <metric-card label="Lead Time"           .value=${this.leadTime}></metric-card>
                <metric-card label="Deploy Frequency"    .value=${this.deployFreq}></metric-card>
                <metric-card label="Time to Restore"     .value=${this.ttr}></metric-card>
                <metric-card label="Change Failure Rate" .value=${this.cfr}></metric-card>
                <metric-card label="Test Coverage Computer" .value=${this.coverageComputer}
                    detailHref="../coverage/computer/index.html"></metric-card>
                <metric-card label="Test Coverage Pico"  .value=${this.coveragePico}
                    detailHref="../coverage/pico/index.html"></metric-card>
                <metric-card label="Test Coverage Pi"    .value=${this.coveragePi}
                    detailHref="../coverage/pi/index.html"></metric-card>
            </div>`;
    }
}

customElements.define('metrics-grid', MetricsGrid);
