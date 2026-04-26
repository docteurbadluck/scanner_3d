import { LitElement, html } from 'https://esm.sh/lit';
import '/js/YellowButton.js';

class TakePhotoButton extends LitElement {
    static properties = { label: {}, disabled: { type: Boolean }, result: {} };
    createRenderRoot() { return this;}
    constructor() {super(); this.disabled = true; this.result = ''; }

    _onAction() {
        this.dispatchEvent(new CustomEvent('take-photo'));
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

customElements.define('take-photo-button', TakePhotoButton);