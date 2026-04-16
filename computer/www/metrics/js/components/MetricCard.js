import { LitElement, html } from 'https://esm.sh/lit';

class MetricCard extends LitElement {
    static properties = { label: {}, value: {} };

    createRenderRoot() { return this; }

    constructor() {
        super();
        this.label = '';
        this.value = '—';
    }

    render() {
        return html`
            <div class="metric-card">
                <div class="metric-label">${this.label}</div>
                <div class="metric-value">${this.value}</div>
            </div>`;
    }
}

customElements.define('metric-card', MetricCard);
