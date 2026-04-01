#pragma once

enum Pos_hand
{
	UP_A,
	UP_B,
	DOWN_A,
	DOWN_B,
	UNKNOWN,
};


class IServoMotor
{
	public :
		virtual ~IServoMotor() = default;
		virtual	bool    go_initial_pos() = 0;
		virtual bool    go_to(Pos_hand pos) = 0; 
};
