import { LitElement, html } from 'https://esm.sh/lit';
import '/js/YellowButton.js';

class NewScanForm extends LitElement {
    static properties = { disabled: { type: Boolean } };
    createRenderRoot() { return this; }
    constructor() { super(); this.disabled = false; }

    _onStart() {
        const input = this.querySelector('input');
        const name = input ? input.value.trim() : '';
        if (!name) return;
        this.dispatchEvent(new CustomEvent('start-scan', { detail: name, bubbles: true }));
        if (input) input.value = '';
    }

    render() {
        return html`
            <div class="flex gap-2 mb-6 justify-center">
                <input
                    class="rounded-lg px-3 py-2 bg-slate-700 text-slate-100 border border-slate-500 focus:outline-none"
                    type="text" placeholder="Scan name" maxlength="64"
                    @keydown=${(e) => e.key === 'Enter' && this._onStart()}
                />
                <yellow-button .disabled=${this.disabled} @action=${this._onStart}>
                    Start Scan
                </yellow-button>
            </div>
        `;
    }
}

customElements.define('new-scan-form', NewScanForm);
