#pragma once
#include "1_domain/System.hpp"
#include "3_interface/IMotorDC.hpp"

class ControllerDC
{
	private :
		IMotorDC	&_motor;
		Pos			_current_pos;	
	public :
		ControllerDC(IMotorDC &Motor);
		~ControllerDC() = default;

		bool	join_initial_pos();
		bool	join_pos(Pos); 
		Pos		get_pos() const;
};
