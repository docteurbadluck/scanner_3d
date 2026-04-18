import { LitElement, html, css } from 'https://esm.sh/lit';

class YellowButton extends LitElement {
    static styles = css`
        :host { display: inline-block; }
        a, button {
            display: inline-block;
            padding: 0.5rem 1.5rem;
            background: #facc15;
            color: #0f172a;
            font-weight: 700;
            border-radius: 9999px;
            text-decoration: none;
            border: none;
            cursor: pointer;
            font-size: 1rem;
            font-family: inherit;
            transition: transform 0.15s;
        }
        a:hover, button:not(:disabled):hover { transform: scale(1.05); }
        button:disabled { opacity: 0.45; cursor: not-allowed; }
    `;

    static properties = { href: {}, disabled: { type: Boolean } };

    constructor() { super(); this.href = ''; this.disabled = false; }

    _onClick() { this.dispatchEvent(new CustomEvent('action', { bubbles: true, composed: true })); }

    render() {
        return this.href
            ? html`<a href="${this.href}"><slot></slot></a>`
            : html`<button ?disabled=${this.disabled} @click=${this._onClick}><slot></slot></button>`;
    }
}

customElements.define('yellow-button', YellowButton);
