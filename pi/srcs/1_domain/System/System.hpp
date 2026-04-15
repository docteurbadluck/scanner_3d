#pragma once
#include <string>
#include <vector>

struct Commands
{
	const std::vector<std::string> valid_command = {
		"PING", "PONG", "GET_STATUS", "GET_PICO_STATUS", "START_CAPTURE", "PING_PICO"
	};
};

enum state
{
	INITIALIZATION,
	READY,
	PROCESSING,
	SENDING
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

		state                     get_state() const;
		[[nodiscard]] std::string getStateString() const;
		const Commands&           getCommands() const;
		void                      setCommandToExecute(const std::string &command);
		[[nodiscard]] std::string getCommandToExecute() const;
		void                      ready();
		void                      processing();
		void                      sending();
		void                      reset();
};
