#include "common_utils.h"

std::string powerToString(LemmingPower power) {
	switch (power)
	{
	case NONE:
		return "WALKER";
	case DIGGER:
		return "DIGGER";
	case BASHER:
		return "BASHER";
	case CLIMBER:
		return "CLIMBER";
	case BUILDER:
		return "BUILDER";
	case BLOCKER:
		return "BLOCKER";
	case WALKER:
		return "WALKER";
	case EXPLOADER:
		return "EXPLOADER";
	case UMBRELLA:
		return "FLOATER";
	default:
		return "NONE";
	}
}