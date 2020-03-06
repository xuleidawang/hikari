#ifndef VECTORS_H_DEF
#define VECTORS_H_DEF

#include <cmath>
#include <iostream>

namespace hikari {
	const double PI = 4* atanf(1);
	const double EPSILON = 1e-6;

	struct Vector3{
		double x;
		double y;
		double z;
		//constructors
		Vector3(double _x=0.0, double _y=0.0, double _z=0.0) : x(_x), y(_y), z(_z) {};

		void        set(double x, double y, double z);
		double       length() const;
		double       distance(const Vector3& vec) const;
		double       angle(const Vector3& vec) const;        // angle between two vectors
		Vector3&    normalize();
		double       dot(const Vector3& vec) const;
		Vector3     cross(const Vector3& vec) const;
		bool        equal(const Vector3& vec, double e) const; // compare with epsilon
		static Vector3 Min(const Vector3 &p1, const Vector3 &p2) {
			return Vector3(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
						   std::min(p1.z, p2.z));
		}

		static Vector3 Max(const Vector3 &p1, const Vector3 &p2) {
			return Vector3(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
						   std::max(p1.z, p2.z));
		}


		Vector3     operator-() const;
		Vector3     operator+(const Vector3& rhs) const;
		Vector3     operator-(const Vector3& rhs) const;
		Vector3&    operator+=(const Vector3& rhs);
		Vector3&    operator-=(const Vector3& rhs);
		Vector3     operator*(const double scale) const;
		Vector3     operator*(const Vector3& rhs) const;    // multiplay each element
		Vector3&    operator*=(const double scale);
		Vector3&    operator*=(const Vector3& rhs);
		Vector3     operator/(const double scale) const;
		Vector3&    operator/=(const double scale);
		bool        operator==(const Vector3& rhs) const;
		bool        operator!=(const Vector3& rhs) const;
		bool        operator<(const Vector3& rhs) const;
		double       operator[](int index) const;
		double&      operator[](int index);


		friend Vector3 operator*(const double a, const Vector3 vec);
		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
	};

	inline Vector3 Vector3::operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 Vector3::operator+(const Vector3& rhs) const {
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3 Vector3::operator-(const Vector3& rhs) const {
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline Vector3& Vector3::operator+=(const Vector3& rhs) {
		x += rhs.x; y += rhs.y; z += rhs.z; return *this;
	}

	inline Vector3& Vector3::operator-=(const Vector3& rhs) {
		x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this;
	}

	inline Vector3 Vector3::operator*(const double a) const {
		return Vector3(x*a, y*a, z*a);
	}

	inline Vector3 Vector3::operator*(const Vector3& rhs) const {
		return Vector3(x*rhs.x, y*rhs.y, z*rhs.z);
	}

	inline Vector3& Vector3::operator*=(const double a) {
		x *= a; y *= a; z *= a; return *this;
	}

	inline Vector3& Vector3::operator*=(const Vector3& rhs) {
		x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this;
	}

	inline Vector3 Vector3::operator/(const double a) const {
		return Vector3(x / a, y / a, z / a);
	}

	inline Vector3& Vector3::operator/=(const double a) {
		x /= a; y /= a; z /= a; return *this;
	}

	inline bool Vector3::operator==(const Vector3& rhs) const {
		return (x == rhs.x) && (y == rhs.y) && (z == rhs.z);
	}

	inline bool Vector3::operator!=(const Vector3& rhs) const {
		return (x != rhs.x) || (y != rhs.y) || (z != rhs.z);
	}

	inline bool Vector3::operator<(const Vector3& rhs) const {
		if (x < rhs.x) return true;
		if (x > rhs.x) return false;
		if (y < rhs.y) return true;
		if (y > rhs.y) return false;
		if (z < rhs.z) return true;
		if (z > rhs.z) return false;
		return false;
	}

	inline double Vector3::operator[](int index) const {
		return (&x)[index];
	}

	inline double& Vector3::operator[](int index) {
		return (&x)[index];
	}

	inline void Vector3::set(double x, double y, double z) {
		this->x = x; this->y = y; this->z = z;
	}

	inline double Vector3::length() const {
		return sqrtf(x*x + y*y + z*z);
	}

	inline double Vector3::distance(const Vector3& vec) const {
		return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y) + (vec.z - z)*(vec.z - z));
	}

	inline double Vector3::angle(const Vector3& vec) const {
		// return angle between [0, 180]
		double l1 = this->length();
		double l2 = vec.length();
		double d = this->dot(vec);
		double angle = acosf(d / (l1 * l2)) / 3.141592f * 180.0f;
		return angle;
	}

	inline Vector3& Vector3::normalize() {
		double xxyyzz = x*x + y*y + z*z;
		double invLength = 1.0f / sqrtf(xxyyzz);
		x *= invLength;
		y *= invLength;
		z *= invLength;
		return *this;
	}

	inline double Vector3::dot(const Vector3& rhs) const {
		return (x*rhs.x + y*rhs.y + z*rhs.z);
	}

	inline Vector3 Vector3::cross(const Vector3& rhs) const {
		return Vector3(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
	}

	inline bool Vector3::equal(const Vector3& rhs, double epsilon) const {
		return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon && fabs(z - rhs.z) < epsilon;
	}

	inline Vector3 operator*(const double a, const Vector3 vec) {
		return Vector3(a*vec.x, a*vec.y, a*vec.z);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return os;
	}

	struct Vector2{
		double x;
		double y;
		//constructors
		Vector2(double _x=0.0, double _y=0.0) : x(_x), y(_y) {};

		void        set(double x, double y);
		double       length() const;
		double       distance(const Vector2& vec) const;
		double       angle(const Vector2& vec) const;        // angle between two vectors
		Vector2&    normalize();
		double       dot(const Vector2& vec) const;

		bool        equal(const Vector2& vec, double e) const; // compare with epsilon
		static Vector2 Min(const Vector2 &p1, const Vector2 &p2) {
			return Vector2(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
		}

		static Vector2 Max(const Vector2 &p1, const Vector2 &p2) {
			return Vector2(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
		}


		Vector2     operator-() const;
		Vector2     operator+(const Vector2& rhs) const;
		Vector2     operator-(const Vector2& rhs) const;
		Vector2&    operator+=(const Vector2& rhs);
		Vector2&    operator-=(const Vector2& rhs);
		Vector2     operator*(const double scale) const;
		Vector2     operator*(const Vector2& rhs) const;    // multiplay each element
		Vector2&    operator*=(const double scale);
		Vector2&    operator*=(const Vector2& rhs);
		Vector2     operator/(const double scale) const;
		Vector2&    operator/=(const double scale);
		bool        operator==(const Vector2& rhs) const;
		bool        operator!=(const Vector2& rhs) const;
		bool        operator<(const Vector2& rhs) const;
		double       operator[](int index) const;
		double&      operator[](int index);


		friend Vector2 operator*(const double a, const Vector2 vec);
		friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
	};

	inline Vector2 Vector2::operator-() const {
		return Vector2(-x, -y);
	}

	inline Vector2 Vector2::operator+(const Vector2& rhs) const {
		return Vector2(x + rhs.x, y + rhs.y);
	}

	inline Vector2 Vector2::operator-(const Vector2& rhs) const {
		return Vector2(x - rhs.x, y - rhs.y);
	}

	inline Vector2& Vector2::operator+=(const Vector2& rhs) {
		x += rhs.x; y += rhs.y; return *this;
	}

	inline Vector2& Vector2::operator-=(const Vector2& rhs) {
		x -= rhs.x; y -= rhs.y; return *this;
	}

	inline Vector2 Vector2::operator*(const double a) const {
		return Vector2(x*a, y*a);
	}

	inline Vector2 Vector2::operator*(const Vector2& rhs) const {
		return Vector2(x*rhs.x, y*rhs.y);
	}

	inline Vector2& Vector2::operator*=(const double a) {
		x *= a; y *= a;return *this;
	}

	inline Vector2& Vector2::operator*=(const Vector2& rhs) {
		x *= rhs.x; y *= rhs.y; return *this;
	}

	inline Vector2 Vector2::operator/(const double a) const {
		return Vector2(x / a, y / a);
	}

	inline Vector2& Vector2::operator/=(const double a) {
		x /= a; y /= a; return *this;
	}

	inline bool Vector2::operator==(const Vector2& rhs) const {
		return (x == rhs.x) && (y == rhs.y);
	}

	inline bool Vector2::operator!=(const Vector2& rhs) const {
		return (x != rhs.x) || (y != rhs.y);
	}

	inline bool Vector2::operator<(const Vector2& rhs) const {
		if (x < rhs.x) return true;
		if (x > rhs.x) return false;
		if (y < rhs.y) return true;
		if (y > rhs.y) return false;
		return false;
	}

	inline double Vector2::operator[](int index) const {
		return (&x)[index];
	}

	inline double& Vector2::operator[](int index) {
		return (&x)[index];
	}

	inline void Vector2::set(double x, double y) {
		this->x = x; this->y = y;
	}

	inline double Vector2::length() const {
		return sqrtf(x*x + y*y);
	}

	inline double Vector2::distance(const Vector2& vec) const {
		return sqrtf((vec.x - x)*(vec.x - x) + (vec.y - y)*(vec.y - y));
	}

	inline double Vector2::angle(const Vector2& vec) const {
		// return angle between [0, 180]
		double l1 = this->length();
		double l2 = vec.length();
		double d = this->dot(vec);
		double angle = acosf(d / (l1 * l2)) / 3.141592f * 180.0f;
		return angle;
	}

	inline Vector2& Vector2::normalize() {
		double xxyy = x*x + y*y;
		double invLength = 1.0f / sqrtf(xxyy);
		x *= invLength;
		y *= invLength;
		return *this;
	}

	inline double Vector2::dot(const Vector2& rhs) const {
		return (x*rhs.x + y*rhs.y);
	}


	inline bool Vector2::equal(const Vector2& rhs, double epsilon) const {
		return fabs(x - rhs.x) < epsilon && fabs(y - rhs.y) < epsilon;
	}

	inline Vector2 operator*(const double a, const Vector2 vec) {
		return Vector2(a*vec.x, a*vec.y);
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector2& vec) {
		os << "(" << vec.x << ", " << vec.y << ", " << ")";
		return os;
	}
}

#endif
