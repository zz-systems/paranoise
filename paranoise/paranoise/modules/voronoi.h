#pragma once
#ifndef PARANOISE_MODULES_VORONOI
#define PARANOISE_MODULES_VORONOI

#include <limits>
#include "../noisegenerators.h"
#include "../parallel/all.h"

namespace paranoise { namespace module {
	using namespace generators;
	using namespace parallel;
	struct voronoi_settings
	{
		float	frequency, 
				displacement;
		int seed;
		bool enableDistance;

		voronoi_settings(float frequency = 1.0, float displacement = 2.0, int seed = 0, bool enableDistance = false)
			: frequency(frequency), displacement(displacement), seed(seed), enableDistance(enableDistance)
		{}
	};

	
	SIMD_ENABLE(TReal, TInt)
	inline TReal voronoi(const Vector3<TReal>& coords, const voronoi_settings& settings)
	{
		auto seed		= Vector3<TInt>(settings.seed, settings.seed + 1, settings.seed + 2);
		auto _coords	= coords * Vector3<TReal>(settings.frequency);

		
		Vector3<TInt> cube {
			(TInt)_coords.x - sel(_coords.x > 0, consts<TInt>::zero(), consts<TInt>::one()),
			(TInt)_coords.y - sel(_coords.y > 0, consts<TInt>::zero(), consts<TInt>::one()),
			(TInt)_coords.z - sel(_coords.z > 0, consts<TInt>::zero(), consts<TInt>::one())
		};

		TReal value, absDist, minDist = static_cast<TInt>(std::numeric_limits<int>::max());

		Vector3<TReal> candidate = { 0, 0, 0 };
		Vector3<TReal> cur, pos, dist;

		// Inside each unit cube, there is a seed point at a random position.  Go
		// through each of the nearby cubes until we find a cube with a seed point
		// that is closest to the specified position.
		
		for (int zoff = -2; zoff <= 2; zoff++)
		{
			cur.z = static_cast<TReal>(cube.z + zoff);			

			for (int yoff = -2; yoff <= 2; yoff++)
			{
				cur.y = static_cast<TReal>(cube.y + yoff);				

				for (int xoff = -2; xoff <= 2; xoff++)
				{
					cur.x = static_cast<TReal>(cube.x + xoff);
										
					pos.x = cur.x + ValueNoise3D<TReal, TInt>(cur, seed.x);
					pos.y = cur.y + ValueNoise3D<TReal, TInt>(cur, seed.y);
					pos.z = cur.z + ValueNoise3D<TReal, TInt>(cur, seed.z);

					dist = pos - _coords;
					absDist = dot(dist, dist);

					//select_candidate(absDist, pos, minDist, candidate);
					auto mask = absDist < minDist;

					candidate.x = sel(mask, pos.x, candidate.x);
					candidate.y = sel(mask, pos.y, candidate.y);
					candidate.z = sel(mask, pos.z, candidate.z);

					minDist = sel(mask, absDist, minDist);
				}
			}
		}

		if (settings.enableDistance)
		{
			auto diff = candidate - _coords;
			auto absDiff = paranoise::parallel::sqrt(dot(diff, diff));

			value = fmsub(absDiff, consts<TReal>::sqrt3(), consts<TReal>::one());
		}

		auto noise = ValueNoise3D<TReal, TInt>(candidate, seed.x);

		// return settings.displacement * noise + value
		return fmadd(settings.displacement, noise, value);
	}	

	SIMD_ENABLE_F(TReal)
	inline void select_candidate(const TReal &absDist, const Vector3<TReal> &pos, TReal &minDist, Vector3<TReal> &candidate)
	{
		auto mask	= absDist < minDist;
		minDist		= sel(mask, absDist, minDist);
		candidate.x = sel(mask, pos.x, candidate.x);
		candidate.y = sel(mask, pos.y, candidate.y);
		candidate.z = sel(mask, pos.z, candidate.z);
	}

	template<>
	inline void select_candidate(const float &absDist, const Vector3<float> &pos, float &minDist, Vector3<float> &candidate)
	{
		auto mask	= absDist < minDist;
		minDist		= sel(mask, absDist, minDist);
		candidate	= sel(mask, pos, candidate);
	}

}}
#endif