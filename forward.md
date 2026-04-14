Plan: Forward Pico Commands via type: "pico"
Context
Aujourd'hui le browser ne peut envoyer que des commandes Pi (type: "command" validées contre Commands.valid). Les commandes moteur Pico (ARM_UP, PLATE_NEXT, etc.) sont inconnues du serveur et rejetées.

L'objectif : ajouter un message {"type":"pico","command":"ARM_UP"} qui bypass la validation Pi et est forwardé directement au Pico via UART — mais uniquement si le Pi est en état READY (la machine à états protège naturellement contre la concurrence avec START_CAPTURE).

Les commandes partagées (PING) peuvent aller au Pi (type:"command") ou au Pico (type:"pico").

Flow complet
Browser → {"type":"pico","command":"ARM_UP"}
  → server.py (pas de Commands.valid check)
  → Pi WebSocket {"type":"pico","command":"ARM_UP"}
  → CommandReceptor_UC : voit type="pico" → skip _isValidCommand
                         → sys.setIsPicoForward(true)
  → main.cpp : sys.processing() → executor.execute(sys)
  → ExecuteOrder_UC : sys.isPicoForward() == true → _forwardToPico("ARM_UP")
  → Pico_Driver::forwardCommand("ARM_UP")
      writeLine("ARM_UP\n")
      boucle readResponse() jusqu'à type != "state"   ← pattern de getPicoStatus()
      retourne {"type":"response","kind":"DONE","command":"ARM_UP"}
  → _sender.sendRaw(response)
  → Computer → Browser
Fichiers à modifier / créer
Computer (Python) — 4 fichiers
1. computer/srcs/_1_domain/messages.py — ajouter PicoForwardMessage

@dataclass(frozen=True)
class PicoForwardMessage:
    command: str
    def to_json(self) -> str:
        return json.dumps({"type": "pico", "command": self.command})
2. computer/srcs/_2_usecases/forward_command_to_pi_uc.py — ajouter forward_pico_command_to_pi

async def forward_pico_command_to_pi(pi: IPiTransport, command: str) -> None:
    await pi.send(PicoForwardMessage(command).to_json())
3. computer/srcs/_2_usecases/handle_incoming_uc.py — ajouter handler type: "pico"

Nouveau _handle_pico() : identique à _handle_command() mais sans CommandMessage.build() (pas de validation Commands.valid). Appelle forward_pico_command_to_pi.

Ajouter dans handle_incoming() :

elif msg_type == "pico":
    await _handle_pico(connection, data, pi, pi_provider)
4. computer/tests/unit/test_usecases.py — ajouter 3 tests :

type:"pico" + Pi disponible → ACK envoyé, commande forwardée telle quelle
type:"pico" + Pi indisponible → ErrorMessage
type:"pico" avec commande inconnue du Pi (ARM_UP) → ACK (pas de rejet)
Pi (C++) — 12 fichiers
5. pi/srcs/1_domain/System/System.hpp — ajouter membre + méthodes

bool _isPicoForward = false;
// dans public:
void setIsPicoForward(bool v);
[[nodiscard]] bool isPicoForward() const;
6. pi/srcs/1_domain/System/accessors/setters.cpp — ajouter setter

void System::setIsPicoForward(bool v) { _isPicoForward = v; }
7. pi/srcs/1_domain/System/accessors/getters.cpp — ajouter getter

bool System::isPicoForward() const { return _isPicoForward; }
8. pi/srcs/1_domain/System/reset.cpp — reset le flag

// ajouter : _isPicoForward = false;
9. pi/srcs/2_usecases/CommandReceptor_UC/CommandReceptor_UC.hpp — ajouter membre

bool _isPicoForward = false;
10. pi/srcs/2_usecases/CommandReceptor_UC/internal/_setMessageFrom.cpp — extraire type

void CommandReceptor_UC::_setMessageFrom(const std::string &raw)
{
    const std::string type       = JsonMessage::extractStringField(raw, "type");
    const std::string jsonCommand = JsonMessage::extractStringField(raw, "command");
    _isPicoForward = (type == "pico");
    _message = jsonCommand.empty() ? raw : jsonCommand;
}
11. pi/srcs/2_usecases/CommandReceptor_UC/waitAndInterprete.cpp — skip validation si pico

bool CommandReceptor_UC::waitAndInterprete(System &sys)
{
    _setMessageFrom(_computer.waitForOrder());
    if (!_isPicoForward && !_isValidCommand(sys.getCommands()))
    {
        _logCommand(false);
        _sender.sendInvalidCmd();
        return false;
    }
    sys.setCommandToExecute(_message);
    sys.setIsPicoForward(_isPicoForward);
    _logCommand(true);
    _sender.sendState(sys);
    return true;
}
12. pi/srcs/3_interface/IPico.hpp — ajouter méthode virtuelle

virtual std::string forwardCommand(const std::string &cmd) = 0;
13. pi/srcs/4_drivers/Pico_Driver/Pico_Driver.hpp — déclarer override

std::string forwardCommand(const std::string &cmd) override;
14. Nouveau : pi/srcs/4_drivers/Pico_Driver/forwardCommand.cpp — implémentation Même pattern que getPicoStatus() : boucle jusqu'à type != "state".

std::string Pico_Driver::forwardCommand(const std::string &cmd)
{
    if (!_writeLine(cmd))
        return JsonMessage::makeError("write_failed");
    std::string response;
    do {
        response = _readResponse();
    } while (JsonMessage::extractStringField(response, "type") == "state");
    return response;
}
15. pi/srcs/2_usecases/ExecuteOrder_UC/ExecuteOrder_UC.hpp — déclarer méthode privée

void _forwardToPico(const std::string &cmd);
16. Nouveau : pi/srcs/2_usecases/ExecuteOrder_UC/internal/_forwardToPico.cpp

void ExecuteOrder_UC::_forwardToPico(const std::string &cmd)
{
    const std::string response = _pico.forwardCommand(cmd);
    _sender.sendRaw(response);
}
17. pi/srcs/2_usecases/ExecuteOrder_UC/execute.cpp — brancher sur isPicoForward

void ExecuteOrder_UC::execute(System &sys)
{
    const std::string cmd = sys.getCommandToExecute();
    if (sys.isPicoForward())
    {
        _forwardToPico(cmd);
        return;
    }
    int res = _dispatch(cmd);
    if (res == -1)
        _sender.sendInvalidCmd();
    else if (cmd != "GET_PICO_STATUS")
        _sender.sendResponse(cmd, res == 0, sys);
}
Vérification
Tests Python : python3 -m pytest computer/tests/unit/ -q — doit passer 37/37
Build Pi : make -f scripts/remote.mk build-pi — doit compiler sans erreur
Test manuel depuis pico_serial.py (bypass serveur) : ARM_UP répond DONE
Test end-to-end : browser envoie {"type":"pico","command":"ARM_UP"} via WS:8081, vérifie réponse {"type":"response","kind":"DONE","command":"ARM_UP"} reçue
Fichiers critiques (référence)
Fichier	Rôle
pi/srcs/4_drivers/Pico_Driver/accessors/getters.cpp	Pattern getter existant à reproduire
pi/srcs/4_drivers/Pico_Driver/getPicoStatus.cpp	Pattern boucle readResponse à réutiliser
computer/srcs/_2_usecases/handle_incoming_uc.py	Handler type:"command" à dupliquer pour "pico"
computer/tests/unit/test_usecases.py