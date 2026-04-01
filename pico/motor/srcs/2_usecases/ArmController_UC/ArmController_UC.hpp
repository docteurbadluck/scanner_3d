#pragma once
#include "1_domain/System.hpp"
#include "3_interface/IMotorDC.hpp"

class ArmController_UC
{
	private :
		IMotorDC	&_motor;
		Pos			_current_pos;	
	public :
		ArmController_UC(IMotorDC &Motor);
		~ArmController_UC() = default;

		bool	join_initial_pos();
		bool	join_pos(Pos); 
		Pos		get_pos() const;
};
