//
// Vectorized and parallelized version of libnoise using the gorynych SIMD toolkit
// Copyright (C) 2015-2016 Sergej Zuyev (sergej.zuyev - at - zz-systems.net)
//
// Original libnoise: 
// Copyright (C) 2003, 2004 Jason Bevins
// The developer's email is jlbezigvins@gmzigail.com (for great email, take
// off every 'zig'.)
//
// This library is free software; you can redistribute it and/or modify it
// under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or (at
// your option) any later version.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
// License (COPYING.txt) for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation,
// Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#pragma once

#include "../dependencies.h"

namespace zzsystems { namespace solowej { namespace modules {
	using namespace gorynych;
	
	using namespace math;

	MODULE(rotate)
	{
	public:
		mat3x3<vreal> rot;
		vec3<float>  angles;

		rotate() : rotate({ 0.0f, 0.0f, 0.0f }) {}		

		rotate(const vec3<float> &angles) : 
			cloneable(1), 
			angles(angles)
		{
			build_rot_matrix(angles);
		}

		rotate(const rotate<vreal, vint> &rhs) :
			cloneable(rhs), 
			rot(rhs.rot), 
			angles(rhs.angles)
		{}

		const json& operator <<(const json &source) override
		{
			if (source["angles"] != nullptr && source["angles"].is_object())
			{
				angles.x = source.value("x", 0.0f);
				angles.y = source.value("y", 0.0f);
				angles.z = source.value("z", 0.0f);
			}

			build_rot_matrix(angles);
			return source;
		}

		MODULE_PROPERTY(source, 0)

		vreal operator()(const vec3<vreal>& coords) const override
		{
			return get_source()(rot * coords);
		}
	private:
		void build_rot_matrix(const vec3<float>& angles)
		{
			auto cos = vec3<vreal>(::cos(angles.x), ::cos(angles.y), ::cos(angles.z));
			auto sin = vec3<vreal>(::sin(angles.x), ::sin(angles.y), ::sin(angles.z));

			rot._0 = vec3<vreal>{ sin.x * sin.y * sin.z + cos.y * cos.z,	cos.x * sin.z,		sin.y * cos.z - cos.y * sin.x * sin.z };
			rot._1 = vec3<vreal>{ sin.y * sin.x * cos.z - cos.y * sin.z,	cos.x * cos.z,		-cos.y * sin.x * cos.z - sin.y * sin.z };
			rot._2 = vec3<vreal>{ -sin.y * cos.x,							sin.x,				cos.y * cos.x };
		}
	};
}}}