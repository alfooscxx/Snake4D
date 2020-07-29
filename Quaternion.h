#pragma once

template <class _T>
class Quaternion
{
public:
	Quaternion();
	Quaternion(_T _re, _T _i, _T _j, _T _k);
	~Quaternion();
	_T re, i, j, k;

	friend Quaternion<_T> operator+(const Quaternion<_T>& first, const Quaternion<_T>& second)
	{
		return Quaternion<_T>(first.re + second.re, first.i + second.i, first.j + second.j, first.k + second.k);
	}
	friend Quaternion<_T> operator-(const Quaternion<_T>& first, const Quaternion<_T>& second)
	{
		return first + (-second);
	}
	friend Quaternion<_T> operator*(const Quaternion<_T>& first, const Quaternion<_T>& second)
	{
		const _T x1 = first.re;
		const _T y1 = first.i;
		const _T u1 = first.j;
		const _T v1 = first.k;
		const _T x2 = second.re;
		const _T y2 = second.i;
		const _T u2 = second.j;
		const _T v2 = second.k;
		const _T _re = x1 * x2 - y1 * y2 - u1 * u2 - v1 * v2;
		const _T _i = x1 * y2 + y1 * x2 + u1 * v2 - v1 * u2;
		const _T _j = x1 * u2 + y1 * v2 + u1 * x2 - v1 * y2;
		const _T _k = x1 * v2 + y1 * u2 - u1 * y2 + v1 * x2;
		return Quaternion<_T>(_re, _i, _j, _k);
	}
	friend Quaternion<_T> operator-(const Quaternion<_T>& _q)
	{
		return Quaternion<_T>(-_q.re, -_q.i, -_q.j, -_q.k);
	}
	friend Quaternion<_T>& operator*=(Quaternion<_T>& first, const Quaternion<_T>& second)
	{
		first = first * second;
		return first;
	}
};

