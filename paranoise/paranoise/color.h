#pragma once
#ifndef PARANOISE_COLOR_H
#define PARANOISE_COLOR_H

#include "basetypes.h"
#include "interpolation.h"
#include <map>


namespace paranoise { namespace util {
	using namespace interpolation;

	ALIGN(32) union color_rgb
	{
		uint8 components[4];
		uint32 integral;
		struct { uint8 a, r, g, b; };

		color_rgb(uint8 r, uint8 g, uint8 b, uint8 a = 255)
			: r(r), g(g), b(b), a(a)
		{}
	};

	//ALIGN(32) union color_hsv
	//{
	//	uint8 components[4];
	//	struct { uint8 a, r, g, b; };

	//	color_hsv(uint8 r, uint8 g, uint8 b, uint8 a = 255)
	//		: r(r), g(g), b(b), a(a)
	//	{}
	//};
		
	struct gradient1D
	{
		std::map<float, color_rgb> colors;

		gradient1D() = default;
		gradient1D(std::initializer_list<std::pair<const float, color_rgb>> init) : colors(init)
		{}

		color_rgb getColor(const float value) const
		{
			auto v = paranoise::parallel::clamp(value, -1.0f, 1.0f);

			auto i0 = colors.lower_bound(v);
			
			/*if (v0 == v1)
				return v1->second;*/

			auto k0 = i0->first;
			auto c0 = i0->second;

			if (i0 == colors.end())
				return color_rgb(0xFF, 0, 0xFF, 0xFF);
			
			i0++;

			if (i0 == colors.end())
				return c0;

			auto k1 = i0->first;
			auto c1 = i0->second;

			auto alpha = (v - k0) / (k1 - k0);

			color_rgb result = {
				(uint8) lerp<float>(c0.r, c1.r, v),
				(uint8) lerp<float>(c0.g, c1.g, v),
				(uint8) lerp<float>(c0.b, c1.b, v),
				(uint8) lerp<float>(c0.a, c1.a, v)
			};

			return result;
		}
	};
}}

#endif