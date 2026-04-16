import { LitElement, html } from 'https://esm.sh/lit';

const BADGE_CLASS = {
    connected:    'badge-ok',
    disconnected: 'badge-err',
    LISTENING:    'badge-ok',
    INTERPRETING: 'badge-warn',
    EXECUTING:    'badge-warn',
    READY:        'badge-ok',
    PROCESSING:   'badge-warn',
    SENDING:      'badge-warn',
};

class StatusRow extends LitElement {
    static properties = { label: {}, state: {} };
    createRenderRoot() { return this; }
    constructor() { super(); this.state = null; }

    render() {
        const cls  = BADGE_CLASS[this.state] ?? 'badge-unknown';
        const text = this.state ?? 'unknown';
        return html`
            <div class="flex justify-between items-center gap-6">
                <span class="opacity-70 text-sm">${this.label}</span>
                <span class="badge ${cls}">${text}</span>
            </div>
        `;
    }
}

customElements.define('status-row', StatusRow);
