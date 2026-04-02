#pragma once

class IDiskChecker
{
	public:
		virtual ~IDiskChecker() = default;
		virtual bool hasEnoughSpace() = 0;
};
