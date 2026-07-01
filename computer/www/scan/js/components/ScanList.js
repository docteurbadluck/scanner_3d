import { LitElement, html } from 'https://esm.sh/lit';
import '/js/YellowButton.js';

const STATUS_BADGE = {
    in_progress: html`<span class="badge badge-warn">In Progress</span>`,
    failed:      html`<span class="badge badge-err">Failed</span>`,
    done:        html`<span class="badge badge-ok">Done</span>`,
};

class ScanList extends LitElement {
    static properties = { scans: { type: Array } };
    createRenderRoot() { return this; }
    constructor() { super(); this.scans = []; }

    _onContinue(name) {
        this.dispatchEvent(new CustomEvent('continue-scan', { detail: name, bubbles: true }));
    }

    _renderScan(scan) {
        const badge = STATUS_BADGE[scan.status] ?? html`<span class="badge badge-unknown">${scan.status}</span>`;
        const progress = `${scan.position_index * 10 + scan.shot_index}/40`;
        return html`
            <div class="flex items-center justify-between gap-3 py-2 border-b border-slate-700">
                <span class="font-mono text-sm flex-1 text-left">${scan.name}</span>
                <span class="text-xs text-slate-400">${progress}</span>
                ${badge}
                ${scan.status === 'failed'
                    ? html`<yellow-button @action=${() => this._onContinue(scan.name)}>Continue</yellow-button>`
                    : ''}
            </div>
        `;
    }

    render() {
        if (!this.scans.length) {
            return html`<p class="text-slate-400 text-sm">No scans yet.</p>`;
        }
        return html`<div class="mt-2">${this.scans.map(s => this._renderScan(s))}</div>`;
    }
}

customElements.define('scan-list', ScanList);
