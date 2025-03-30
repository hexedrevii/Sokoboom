#pragma once

namespace sokoboom::utilities
{
	constexpr int trunc(float x) noexcept
	{
		return int(x);
	}

	// todo: replace with std::unreachable()
	[[noreturn]]
	constexpr void unreachable() noexcept
	{
#ifdef _MSC_VER
		__assume(false);
#else
#	error "unreachable not implemented"
#endif
	}
}