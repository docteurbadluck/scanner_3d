import { LitElement, html } from 'https://esm.sh/lit';

class PingButton extends LitElement {
    static properties = { label: {}, disabled: { type: Boolean }, result: {} };
    createRenderRoot() { return this; }
    constructor() { super(); this.disabled = true; this.result = ''; }

    _onClick() {
        this.dispatchEvent(new CustomEvent('ping'));
    }

    render() {
        return html`
            <button class="ping-btn" ?disabled=${this.disabled} @click=${this._onClick}>
                ${this.label}
            </button>
            <div class="mt-2 text-sm min-h-[1.1rem] opacity-80">${this.result}</div>
        `;
    }
}

customElements.define('ping-button', PingButton);
