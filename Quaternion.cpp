#include "Quaternion.h"


template <class _T>
Quaternion<_T>::Quaternion()
{
}

template <class _T>
Quaternion<_T>::Quaternion(_T _re, _T _i, _T _j, _T _k)
	: re{ _re }, i{ _i }, j{ _j }, k{ _k }
{
}

template <class _T>
Quaternion<_T>::~Quaternion()
{
}