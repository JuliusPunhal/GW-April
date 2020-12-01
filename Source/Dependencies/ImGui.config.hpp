#pragma once

#include "April/Utility/DrawStructures.h"


#define IM_VEC2_CLASS_EXTRA													\
		ImVec2( April::XY const& xy ) noexcept : x{ xy.x }, y{ xy.y } {}	\
		operator April::XY() const noexcept { return April::XY{ x, y }; }	\
		ImVec2( April::WH const& wh ) noexcept : x{ wh.w }, y{ wh.h } {}	\
		operator April::WH() const noexcept { return April::WH{ x, y }; }

#define IM_VEC4_CLASS_EXTRA									\
		ImVec4( April::RGBA const& c )  noexcept			\
			: x{ c.r }, y{ c.g }, z{ c.b }, w{ c.a } {} 	\
		operator April::RGBA() const noexcept				\
		{ return April::RGBA{ x, y, z, w }; }
