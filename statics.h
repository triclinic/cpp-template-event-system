#pragma once

template<bool condition, class trueType, class falseType>
struct static_if
{
	typedef trueType type;
};

template<class trueType, class falseType>
struct static_if<false, trueType, falseType>
{
	 typedef falseType type;
};
