#pragma once

namespace sokoboom::utilities
{
	inline int trunc(float x)
	{
		return int(x);
	}

	// todo: replace with std::unreachable()
	[[noreturn]]
	inline void unreachable()
	{
#ifdef _MSC_VER
		__assume(false);
#else
#	error "unreachable not implemented"
#endif
	}
}