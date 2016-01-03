#pragma once
#ifndef PARANOISE_MODULES_SELECT
#define PARANOISE_MODULES_SELECT

#include "../noisegenerators.h"

namespace paranoise { namespace module {
	using namespace paranoise::generators;

	struct rotate_settings
	{
		Matrix3x3<double> rot;
		Vector3<double>  angles;

		rotate_settings() : rotate_settings({ 0, 0, 0 }) {}
		rotate_settings(Vector3<double> angles) : angles(angles)
		{
			auto cos = Vector3<double>( ::cos(angles.x), ::cos(angles.y), ::cos(angles.z) );
			auto sin = Vector3<double>( ::sin(angles.x), ::sin(angles.y), ::sin(angles.z) );

			rot = {
				Vector3<double>{ sin.x * sin.y * sin.z + cos.y * cos.z,	cos.x * sin.z,		sin.y * cos.z - cos.y * sin.x * sin.z  },
				Vector3<double>{ sin.y * sin.x * cos.z - cos.y * sin.z,	cos.x * cos.z,		-cos.y * sin.x * cos.z - sin.y * sin.z },
				Vector3<double>{-sin.y * cos.x,							sin.x,				cos.y * cos.x						   }
			};			
		}
	};

	SIMD_ENABLE(TReal, TInt)
	inline TReal rotate(	const Module<TReal>& source,
							const Vector3<TReal>& coords,
							const rotate_settings& settings)
	{
		return source(settings.rot * coords);
	}
}}
#endif