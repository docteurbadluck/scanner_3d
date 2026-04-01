#pragma once

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
		state _current_state;

	public:
		System();
		~System() = default;

		state get_state() const;
		void  ready();
		void  processing();
		void  sending();
		void  reset();
};
