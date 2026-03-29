#pragma once

enum Pos
{
	UP,
	DOWN,
	UNKNOWN,
};


class IMotorDC
{
	public :
		virtual ~IMotorDC() = default;
		virtual	bool    go_initial_pos() = 0;
		virtual bool    go_to(Pos pos) = 0; 
};
