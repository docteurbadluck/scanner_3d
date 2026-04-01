#pragma once

class IAccelerometer
{
	public:
		virtual ~IAccelerometer() = default;
		virtual float	read_magnitude() = 0;
};
