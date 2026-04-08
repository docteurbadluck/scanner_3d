#pragma once
#include <string>
#include <vector>

enum state
{
    LISTENING,
    INTERPRETING,
    EXECUTING
};

struct Commands
{
    const std::vector<std::string> valid_command = {"PING", "PONG", "GET_STATUS",
                                                    "ARM_UP", "ARM_DOWN",
                                                    "CAM_A", "CAM_B", "CAM_C", "CAM_D",
                                                    "PLATE_NEXT", "INITIAL_POS"};
};


class System
{
private:
    state       _current_state;
    Commands    _commands;
    std::string _command_to_execute;

public:
    System();
    ~System() = default;

    void
                          setCommandToExecute(const std::string &command);
    [[nodiscard]] std::string getCommandToExecute() const;

    state                     getState() const;
    [[nodiscard]] std::string getStateString() const;
    void                      commandReceived();
    void                      commandInterpreted(bool correct);
    bool                      resultCommand(int res_code);
    Commands                  getCommands() const;
};
