import { LitElement, html, css } from 'https://esm.sh/lit';
import './YellowButton.js';

const NAV_PAGES = [
    { id: 'home',         name: 'Home',         href: '/' },
    { id: 'status',       name: 'Status',       href: '/status/' },
    { id: 'metrics',      name: 'Metrics',      href: '/metrics/' },
    { id: 'productivity', name: 'Productivity', href: '/productivity/' },
    { id: 'complexity',   name: 'Complexity',   href: '/complexity/' },
];

class PageCard extends LitElement {
    static styles = css`
        :host {
            display: flex;
            align-items: center;
            justify-content: center;
            min-height: 100vh;
            background: #0f172a;
            font-family: sans-serif;
            color: #e2e8f0;
            box-sizing: border-box;
        }
        .card {
            padding: 2.5rem 3rem;
            border-radius: 1.25rem;
            background: rgba(30, 41, 59, 0.6);
            backdrop-filter: blur(8px);
            text-align: center;
            min-width: 340px;
            box-shadow: 0 25px 50px -12px rgba(0, 0, 0, 0.5);
        }
        h1 {
            font-size: 2.25rem;
            font-weight: 700;
            line-height: 1.2;
            margin: 0 0 0.5rem;
        }
        p.subtitle {
            margin: 0 0 1.5rem;
            opacity: 0.85;
            font-size: 0.95rem;
        }
        nav {
            display: flex;
            gap: 1rem;
            justify-content: center;
            flex-wrap: wrap;
            margin-bottom: 1.5rem;
        }
    `;

    static properties = { title: {}, page: {}, subtitle: {} };

    constructor() { super(); this.title = ''; this.page = ''; this.subtitle = ''; }

    _navLinks() {
        return NAV_PAGES
            .filter(p => p.id !== this.page)
            .map(p => html`<yellow-button href="${p.href}">${p.name}</yellow-button>`);
    }

    render() {
        const links = this._navLinks();
        return html`
            <div class="card">
                <h1>${this.title}</h1>
                ${this.subtitle ? html`<p class="subtitle">${this.subtitle}</p>` : ''}
                ${links.length ? html`<nav>${links}</nav>` : ''}
                <slot></slot>
            </div>
        `;
    }
}

customElements.define('page-card', PageCard);
