import { LitElement, html } from 'https://esm.sh/lit';
import './PingButton.js';

class PingSection extends LitElement {
    static properties = {
        piDisabled:   { type: Boolean },
        picoDisabled: { type: Boolean },
        piResult:     {},
        picoResult:   {},
    };
    createRenderRoot() { return this; }
    constructor() {
        super();
        this.piDisabled = this.picoDisabled = true;
        this.piResult   = this.picoResult   = '';
    }

    render() {
        return html`
            <ping-button label="Ping Pi"
                ?disabled=${this.piDisabled}
                result=${this.piResult}
                @ping=${() => this.dispatchEvent(new CustomEvent('ping-pi'))}>
            </ping-button>
            <div class="mt-3">
                <ping-button label="Ping Pico"
                    ?disabled=${this.picoDisabled}
                    result=${this.picoResult}
                    @ping=${() => this.dispatchEvent(new CustomEvent('ping-pico'))}>
                </ping-button>
            </div>
        `;
    }
}

customElements.define('ping-section', PingSection);
