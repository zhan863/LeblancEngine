//======================================================================
//
//	MJP's DX11 Sample Framework
//  http://mynameismjp.wordpress.com/
//
//  All code and content licensed under Microsoft Public License (Ms-PL)
//
//======================================================================
#pragma once
#include "LeblancEngine/BasicInclude/LeblancPCH.h"

template <class T>
class Limits
{
public:
	static bool                isEqual(T a, T b) { return isZero(a - b); }
	static bool                isZero(T a) { return (a == 0); }
	static bool                isNaN(T a) { return  false; }
	static bool                isInf(T) { return !_finite(a); }

	static size_t              maxVal(const T& a, const T& b) { return (a > b) ? a : b; }
	static size_t              minVal(const T& a, const T& b) { return (a < b) ? a : b; }
	static T                   maximum();
	static T                   minimum();
	static T                   one() { return 1; }
};

template<> class Limits <char>
{
public:
	static char                 maximum() { return CHAR_MAX; }
	static char                 minimum() { return CHAR_MIN; }
	static bool                 isEqual(char a, char b) { return a == b; }
};

template<> class Limits <uint8_t>
{
public:
	static uint8_t             maximum() { return UCHAR_MAX; }
	static uint8_t             minimum() { return 0; }
};

template<> class Limits <int16_t>
{
public:
	static int16_t             maximum() { return SHRT_MAX; }
	static int16_t             minimum() { return SHRT_MIN; }
};

template<> class Limits <uint16_t>
{
public:
	static uint16_t             maximum() { return USHRT_MAX; }
	static uint16_t             minimum() { return 0; }
};

template<> class Limits <int32_t>
{
public:
	static int32_t             maximum() { return INT_MAX; }
	static int32_t             minimum() { return INT_MIN; }
	static int32_t             maxVal(const int32_t& a, const int32_t& b) { return (a > b) ? a : b; }
	static int32_t             minVal(const int32_t& a, const int32_t& b) { return (a < b) ? a : b; }
	static bool                isZero(int32_t a) { return (a == 0); }
	static bool                isEqual(int32_t a, int32_t b) { return a == b; }
};

template<> class Limits <int64_t>
{
public:
	static int64_t             maximum() { return _I64_MAX; }
	static int64_t             minimum() { return _I64_MIN; }
	static int64_t             maxVal(const int64_t& a, const int64_t& b) { return (a > b) ? a : b; }
	static int64_t             minVal(const int64_t& a, const int64_t& b) { return (a < b) ? a : b; }
	static bool                isZero(int64_t a) { return (a == 0); }
	static bool                isEqual(int64_t a, int64_t b) { return a == b; }
};

template<> class Limits <uint64_t>
{
public:
	static uint64_t            maximum() { return _UI64_MAX; }
	static uint64_t            minimum() { return 0; }
	static uint64_t            maxVal(const uint64_t& a, const uint64_t& b) { return (a > b) ? a : b; }
	static uint64_t            minVal(const uint64_t& a, const uint64_t& b) { return (a < b) ? a : b; }
	static bool                isZero(uint64_t a) { return (a == 0); }
	static bool                isEqual(uint64_t a, uint64_t b) { return a == b; }
};

template<> class Limits <uint32_t>
{
public:
	static uint32_t            maximum() { return UINT_MAX; }
	static uint32_t            minimum() { return 0; }
	static bool                isZero(uint32_t a) { return (a == 0); }
	static bool                isEqual(uint32_t a, uint32_t b) { return a == b; }
};

template<> class Limits <long>
{
public:
	static long                maximum() { return LONG_MAX; }
	static long                minimum() { return LONG_MIN; }
	static bool                isZero(long a) { return (a == 0); }
	static long                one() { return 1; }
	static bool                isEqual(long a, long b) { return a == b; }
};

template<> class Limits <unsigned long>
{
public:
	static unsigned long       maximum() { return ULONG_MAX; }
	static unsigned long       minimum() { return 0; }
	static bool                isZero(unsigned long a) { return (a == 0); }
	static bool                isEqual(unsigned long a, unsigned long b) { return a == b; }
};

template<> class Limits <float>
{
public:
	static float               maximum() { return FLT_MAX; }
	static float               minimum() { return FLT_MIN; }
	static float               epsilon() { return 1e-6f; }
	static bool                isZero(float a) { return (fabsf(a) < epsilon()); }
	static bool                isEqual(float a, float b) { return Limits<float>::isZero(a - b); }
	static float               one() { return 1.0f; }
};

template<> class Limits <double>
{
public:
	static double              maximum() { return DBL_MAX; }
	static double              minimum() { return DBL_MIN; }
	static double              epsilon() { return DBL_EPSILON; }
	static bool                isZero(double a) { return (abs(a) < epsilon()); }
	static bool                isEqual(double a, double b) { return isZero(a - b); }
	static double              one() { return 1.0; }
};

template <typename T>
class Vector3
{
public:
	T x;
	T y;
	T z;

	Vector3<T>(T all = 0) : x(all), y(all), z(all) {}
	Vector3<T>(T nx, T ny, T nz) : x(nx), y(ny), z(nz) {}
	Vector3<T>(T nx, T ny) : x(nx), y(ny), z(0) {}
	Vector3<T>(const Vector3<T>& other) { memcpy(&x, &other.x, sizeof(T) * 3); }

	Vector3<T>& operator = (const Vector3<T>& other)
	{
		if (this != &other)
		{
			memcpy(&x, &other.x, sizeof(T) * 3);
		}
		return *this;
	}

	static Vector3<T> baseTypeMax() { return Vector3<T>(Limits<T>::maximum()); }

	static Vector3<T> baseTypeMin() { return Vector3<T>(Limits<T>::minimum()); }

	inline T& operator[] (unsigned int i) { return (&x)[i]; }
	inline T operator[] (unsigned int i) const { return (&x)[i]; }
	inline T length() const { return Ctr::sqrt(x*x + y * y + z * z); }
	inline T lengthSquared() const { return (x*x + y * y + z * z); }
	inline T distance(const Vector3<T>& b) const { return Ctr::sqrt(distanceSquared(b)); }

	inline T distanceSquared(const Vector3<T>& b) const
	{
		T nx = x - b.x;
		T ny = y - b.y;
		T nz = z - b.z;
		return nx * nx + ny * ny + nz * nz;
	}

	inline bool operator == (const Vector3<T>& other) const
	{
		if (Limits<T>::isEqual(other.x, x) &&
			Limits<T>::isEqual(other.y, y) &&
			Limits<T>::isEqual(other.z, z))
		{
			return true;
		}
		return false;
	}

	bool operator < (const Vector3<T>& other) const
	{
		if (x < other.x && y < other.y && z < other.z)
		{
			return true;
		}
		return false;
	}

	bool operator != (const Vector3<T>& other) const
	{
		if (Limits<T>::isEqual(other.x, x)
			&& Limits<T>::isEqual(other.y, y)
			&& Limits<T>::isEqual(other.z, z))
		{
			return false;
		}
		return true;
	}

	bool                       isZero() const
	{
		return Limits<T>::isZero(x*x + y * y + z * z);
	}

	bool isUnit() const
	{
		return Limits<T>::isZero(1.0f - x * x - y * y - z * z);
	}

	inline void set(T nx, T ny, T nz)
	{
		x = nx; y = ny; z = nz;
	}

	inline void expandUnit()
	{
		x = (x * 2.0f) - 1.0f;
		y = (y * 2.0f) - 1.0f;
		z = (z * 2.0f) - 1.0f;
	}

	inline void compressUnit()
	{
		x = (x + 1.0f) * 0.5f;
		y = (y + 1.0f) * 0.5f;
		z = (z + 1.0f) * 0.5f;
	}

	void normalize()
	{
		T lengthsq = lengthSquared();
		if (Limits<T>::isEqual(lengthsq, (T)(0)))
		{
			x = 0;
			y = 0;
			z = 0;
		}
		else
		{
			T factor = (T)(1.0) / (T)sqrtf(lengthsq);
			x *= factor;
			y *= factor;
			z *= factor;
		}
	}

	Vector3<T> normalized() const
	{
		Vector3<T> out = *this;
		out.normalize();
		return out;
	}

	static const uint32_t dimensions() { return 3; }

	inline bool isZeroLength() const
	{
		T sqlen = (x * x) + (y * y) + (z * z);
		return (sqlen < (pow(1e-06, 2)));
	}

	Vector3<T> operator + (const Vector3<T>& other) const
	{
		return Vector3<T>(x + other.x, y + other.y, z + other.z);
	}

	friend Vector3<T>& operator += (Vector3<T>& a, const Vector3<T>& b)
	{
		a.x += b.x; a.y += b.y; a.z += b.z;
		return a;
	}

	Vector3<T> operator - (const Vector3<T>& b) const { return Vector3(x - b.x, y - b.y, z - b.z); }
	friend Vector3<T>& operator -= (Vector3<T>& a, const Vector3<T>& b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }

	Vector3<T> operator - () const { return Vector3(-x, -y, -z); }

	Vector3<T> operator * (const Vector3<T>& scalar) const { return Vector3<T>(scalar.x*x, scalar.y*y, scalar.z*z); }

	Vector3<T> operator * (T scalar) { return Vector3<T>(scalar*x, scalar*y, scalar*z); }

	friend Vector3<T> operator * (T scalar, const Vector3<T>& a) { return Vector3<T>(scalar*a.x, scalar*a.y, scalar*a.z); }

	friend Vector3<T> operator * (const Vector3<T>& a, T scalar) { return Vector3<T>(scalar*a.x, scalar*a.y, scalar*a.z); }

	Vector3<T>& operator *= (T scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }

	Vector3<T> operator / (T scalar) { return Vector3<T>(x / scalar, y / scalar, z / scalar); }

	Vector3<T>& operator /= (T scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }

	T dot(const Vector3<T>& vector) const { return (x*vector.x + y * vector.y + z * vector.z); }

	friend T dot(const Vector3<T>& a, const Vector3<T>& b) { return (a.x*b.x + a.y*b.y + a.z*b.z); }

	Vector3 cross(const Vector3<T>& b) const { return Vector3<T>(y*b.z - z * b.y, z*b.x - x * b.z, x*b.y - y * b.x); }

	friend Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) { return Vector3<T>(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x); }

	friend std::ostream& operator << (std::ostream &s, const Vector3<T>& v) { return (s << "x = " << v.x << " y = " << v.y << " z = " << v.z); }
};

typedef Vector3<typename int> Vector3i;
typedef Vector3<typename float> Vector3f;


template <typename T>
class Vector2
{
public:
	T x;
	T y;

	Vector2<T>(T nx, T ny) : x(nx), y(ny) {}
	Vector2<T>(T all = 0) : x(all), y(all) {}
	Vector2<T>(const Vector2<T>& other) { memcpy(&x, &other.x, sizeof(T) * 2); }

	inline T& operator[] (size_t i) { return (&x)[i]; }

	inline T operator[] (size_t i) const { return (&x)[i]; }

	static uint32_t dimensions() { return 2; }
	static Vector2<T> baseTypeMax() { return Vector2<T>(Limits<T>::maximum()); }
	static Vector2<T> baseTypeMin() { return Vector2<T>(Limits<T>::minimum()); }

	inline const Vector2<T> & operator -= (const Vector2<T> &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vector2<T> operator-(const Vector2<T> &v) const { return Vector2(x - v.x, y - v.y); }

	inline Vector2<T> operator+(const Vector2<T> &v) const { return Vector2(x + v.x, y + v.y); }

	inline Vector2<T> operator/(T v) const { return Vector2(x / v, y / v); }

	inline void operator*=(T s)
	{
		x *= s;
		y *= s;
	}

	inline Vector2<T> operator-() const { return Vector2(-x, -y); }

	Vector2<T>& operator=(const Vector2<T>& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		return *this;
	}

	inline void operator+=(const Vector2<T>& v)
	{
		x += v.x;
		y += v.y;
	}

	inline void operator*=(const Vector2<T>& v)
	{
		x *= v.x;
		y *= v.x;
	}

	inline bool operator == (const Vector2<T>& other) const
	{
		return (Limits<T>::isEqual(other.x, x) &&
			Limits<T>::isEqual(other.y, y));
	}

	inline bool operator != (const Vector2<T>& other) const
	{
		if (Limits<T>::isEqual(other.x, x) &&
			Limits<T>::isEqual(other.y, y))
		{
			return false;
		}
		return true;
	}

	friend inline Vector2<T> operator*(const Vector2<T>& v, T s)
	{
		Vector2<T> result = *this;
		result *= s;
		return result;
	}

	friend inline Vector2<T> operator*(T s, const Vector2<T>& v)
	{
		Vector2<T> result = *this;
		result *= s;
		return result;
	}

	friend inline Vector2<T> operator*(const Vector2<T>& v1, const Vector2<T>& v2) { return Vector2<T>(v1.x*v2.x, v1.y*v2.y); }
	
	friend std::ostream& operator << (std::ostream &s, const Vector2<T>& v) { return (s << "x = " << v.x << " y = " << v.y); }
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

template <typename T>
class Vector4
{
public:
	T x;
	T y;
	T z;
	T w;

	Vector4<T>() : x(0), y(0), z(0) {};
	Vector4<T>(T nx, T ny, T nz, T nw) : x(nx), y(ny), z(nz), w(nw) {}
	Vector4<T>(T init) : x(init), y(init), z(init), w(init) {}
	Vector4<T>(const Vector4<T>& other) { x = other.x; y = other.y; z = other.z; w = other.w; }
	Vector4<T>& operator = (const Vector4<T>& other)
	{
		if (this == &other)
			return *this;
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	static uint32_t dimensions() { return 4; }

	inline bool operator == (const Vector4<T>& other) const
	{
		// Matrix44 uses memcpy for the product graph.
		// This is more applicable in a general sense.
		if (Limits<T>::isEqual(other.x, x) &&
			Limits<T>::isEqual(other.y, y) &&
			Limits<T>::isEqual(other.z, z) &&
			Limits<T>::isEqual(other.w, w))
		{
			return true;
		}
		return false;
	}

	inline T& operator[] (size_t i) { return (&x)[i]; }
	inline T operator[] (size_t i) const { return (&x)[i]; }

	Vector4<T> operator + (const Vector4<T>& other) const
	{
		return Vector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
	}

	friend Vector4<T>& operator += (Vector4<T>& a,
		const Vector4<T>& b)
	{
		a.x += b.x; a.y += b.y; a.z += b.z; a.w += b.w;
		return a;
	}

	Vector4<T> operator - (const Vector4<T>& b) const
	{
		return Vector4(x - b.x, y - b.y, z - b.z, w - b.w);
	}

	friend Vector4<T>& operator -= (Vector4<T>& a,
		const Vector4<T>& b)
	{
		a.x -= b.x; a.y -= b.y; a.z -= b.z; a.w -= b.w; return a;
	}

	Vector4<T> operator - () const
	{
		return Vector4(-x, -y, -z, w);
	}

	Vector4<T> operator * (T scalar)
	{
		return Vector4<T>(scalar*x, scalar*y, scalar*z, scalar*w);
	}

	friend Vector4<T> operator *  (T scalar, const Vector4<T>& a)
	{
		return Vector4<T>(scalar*a.x, scalar*a.y, scalar*a.z, scalar * a.w);
	}

	Vector4<T>& operator *= (T scalar)
	{
		x *= scalar; y *= scalar; z *= scalar; w *= scalar; return *this;
	}

	Vector4<T> operator / (T scalar)
	{
		return Vector4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	Vector4<T>& operator /= (T scalar)
	{
		x /= scalar; y /= scalar; z /= scalar; w /= scalar; return *this;
	}

	T dot(const Vector4<T>& b) const
	{
		return (x * b.x) + (y * b.y) + (z * b.z) + (w * b.w);
	}

	inline T distance(const Vector4<T>& b) const
	{
		return sqrt(distanceSquared(b));
	}

	inline T distanceSquared(const Vector4<T>& b) const
	{
		T nx = x - b.x;
		T ny = y - b.y;
		T nz = z - b.z;
		T nw = w - b.w;
		return nx * nx + ny * ny + nz * nz + nw * nw;
	}

	static Vector4<T> lerp(const Vector4<T>& from, const Vector4<T>& to, float slerp)
	{
		Vector4<T> result = to - from;
		result *= slerp;
		result += from;
		return result;
	};

	friend std::ostream& operator << (std::ostream &s, const Vector4<T>& v)
	{
		return (s << "x = " << v.x << " y = " << v.y << " z = " << v.z " w = " << v.);
	}
};

typedef Vector4<float> Vector4f;
typedef Vector4<int> Vector4i;