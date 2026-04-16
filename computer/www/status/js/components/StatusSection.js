import { LitElement, html } from 'https://esm.sh/lit';
import './StatusRow.js';

class StatusSection extends LitElement {
    static properties = { piState: {}, systemState: {}, picoState: {} };
    createRenderRoot() { return this; }
    constructor() { super(); this.piState = this.systemState = this.picoState = null; }

    render() {
        return html`
            <div class="flex flex-col gap-3 mb-6">
                <status-row label="Pi"       .state=${this.piState}></status-row>
                <status-row label="Pi State" .state=${this.systemState}></status-row>
                <status-row label="Pico"     .state=${this.picoState}></status-row>
            </div>
        `;
    }
}

customElements.define('status-section', StatusSection);
