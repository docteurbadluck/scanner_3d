import { LitElement, html } from 'https://esm.sh/lit';

class MetricCard extends LitElement {
    static properties = { label: {}, value: {}, detailHref: {} };

    createRenderRoot() { return this; }

    constructor() {
        super();
        this.label     = '';
        this.value     = '—';
        this.detailHref = '';
    }

    render() {
        return html`
            <div class="metric-card">
                <div class="metric-label">${this.label}</div>
                <div class="metric-value">${this.value}</div>
                ${this.detailHref ? html`
                    <a class="detail-link" href="${this.detailHref}">Détails →</a>
                ` : ''}
            </div>`;
    }
}

customElements.define('metric-card', MetricCard);
