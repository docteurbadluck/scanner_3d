#include "1_domain/System.hpp"
#include "3_interface/IServoMotor.hpp"


class HandController_UC
{
	private :
		IServoMotor &_servo;
		Pos_hand	_current_pos;

	public : 
		HandController_UC(IServoMotor &servo);
		~HandController_UC() = default;

		bool		join_initial_pos();
		bool		join_pos(Pos_hand pos); 
		Pos_hand	get_pos() const;
};
