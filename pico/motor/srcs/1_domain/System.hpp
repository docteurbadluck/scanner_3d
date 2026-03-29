#include <string>

enum state
{
    LISTENING,
    INTERPRETING,
    EXECUTING
};

struct Commands
{
    static const int nbr_command = 2;
    const std::string valid_command[2] = {"PING", "GET_STATUS"};
}; 


class System
{
    private :
    state   _current_state;
    Commands    _commands;
    std::string _command_to_execute;

    public :
        System();
        ~System() = default;
        void    set_command_to_execute(const std::string & command);
        state   get_state() const;
        void    command_received();
        void    command_interpreted(bool correct);
        bool    result_command(int res_code);
        Commands get_commands() const;
};


