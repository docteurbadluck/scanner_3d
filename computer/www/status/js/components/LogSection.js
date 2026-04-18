import { LitElement, html } from 'https://esm.sh/lit';

const MAX_LINES = 20;

class LogSection extends LitElement {
    static properties = { _lines: { state: true } };
    createRenderRoot() { return this; }
    constructor() { super(); this._lines = []; }

    appendLog(raw) {
        this._lines = [...this._lines.slice(-(MAX_LINES - 1)), raw];
    }

    updated() {
        const panel = this.querySelector('[data-panel]');
        if (panel) panel.scrollTop = panel.scrollHeight;
    }

    render() {
        return html`
            <div class="flex justify-end mt-4">
                <button class="ping-btn !w-auto !px-4 !py-1 !text-xs"
                    @click=${() => this._lines = []}>Clear logs</button>
            </div>
            <div data-panel class="mt-6 text-left bg-black/30 rounded-lg p-2 max-h-40 overflow-y-auto text-xs font-mono text-slate-400">
                ${this._lines.map(l => html`<p class="log-line">${l}</p>`)}
            </div>
        `;
    }
}

customElements.define('log-section', LogSection);
