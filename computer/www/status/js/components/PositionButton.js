import { LitElement, html } from 'https://esm.sh/lit';
import '/js/YellowButton.js';

class PositionButton extends LitElement {
    static properties = { label: {}, command: {}, disabled: { type: Boolean }, result: {} };
    createRenderRoot() { return this;}
    constructor() {super(); this.disabled = true; this.result = ''; }

    _onAction() {
        this.dispatchEvent(new CustomEvent('position-action', { detail: this.command }));
    }

    render() {
        return html`
        <yellow-button .disabled=${this.disabled} @action=${this._onAction}>
                ${this.label}
            </yellow-button>
            <div class="mt-2 text-sm min-h-[1.1rem] opacity-80">${this.result}</div>
        `;
    }
}

customElements.define('position-button', PositionButton);
