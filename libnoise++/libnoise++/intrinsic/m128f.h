#pragma once
#ifndef PARANOISE_INTRINSIC_M128F_H
#define PARANOISE_INTRINSIC_M128F_H

#include "base.h"

namespace paranoise { namespace intrinsic {
	union m128i;
	union m128d;

	ALIGN(16) union m128f {
		__m128 val;
		float values[4];

		m128f() = default;
		m128f(const float& rhs)		{ val = _mm_set_ps1(rhs); }

		m128f(const __m128& rhs)	{ val = rhs; }
		m128f(const __m128i& rhs)	{ val = _mm_cvtepi32_ps(rhs); }
		m128f(const __m128d& rhs)	{ val = _mm_cvtpd_ps(rhs); }

		m128f(const m128f&	rhs);
		m128f(const m128i&	rhs);
		m128f(const m128d&	rhs);

		/*m128f &operator =(const m128i &rhs);
		m128f &operator =(const m128f &rhs);
		m128f &operator =(const m128d &rhs);*/

		explicit operator float*() { return values; }
	};

	inline m128f operator +(const m128f& a, const m128f& b) { return _mm_add_ps		(a.val, b.val); }
	inline m128f operator -(const m128f& a, const m128f& b) { return _mm_sub_ps		(a.val, b.val); }
	inline m128f operator *(const m128f& a, const m128f& b) { return _mm_mul_ps		(a.val, b.val); }
	inline m128f operator /(const m128f& a, const m128f& b) { return _mm_div_ps		(a.val, b.val); }

	inline m128f operator >(const m128f& a, const m128f& b) { return _mm_cmpgt_ps	(a.val, b.val); }
	inline m128f operator <(const m128f& a, const m128f& b) { return _mm_cmplt_ps	(a.val, b.val); }	
	inline m128f operator==(const m128f& a, const m128f& b) { return _mm_cmpeq_ps	(a.val, b.val); }

	inline m128f operator &(const m128f& a, const m128f& b) { return _mm_and_ps		(a.val, b.val); }
	inline m128f operator ~(const m128f& a)					{ return _mm_andnot_ps	(a.val, a.val); }

	inline m128f		abs(const m128f& a) {
		static const __m128 sign_mask = _mm_set_ps1(-0.f); // -0.f = 1 << 31
		return _mm_andnot_ps(sign_mask, a.val);
	}

	inline m128f min(const m128f& a, const m128f& b)		{ return _mm_min_ps		(a.val, b.val); }
	inline m128f max(const m128f& a, const m128f& b)		{ return _mm_max_ps		(a.val, b.val); }	

	inline m128f sqrt(const m128f& a)						{ return _mm_sqrt_ps	(a.val); }
}}

#endif