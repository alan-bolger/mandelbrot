#ifndef VECTOR2_H
#define VECTOR2_H

#include <SFML/Graphics.hpp>
#include <string>

// Last updated on 25.08.21 ~ AJB

class Vector2
{
public:
	Vector2();
	Vector2(double t_x, double t_y);
	Vector2(sf::Vector2i t_vector);
	Vector2(sf::Vector2f t_vector);
	Vector2(sf::Vector2u t_vector);
	~Vector2();

	double length() const;
	double lengthSquared() const;
	Vector2 unit() const;
	void normalise();
	Vector2 rotateBy(float t_angleRadians);
	double angleBetween(const Vector2 t_vector) const;
	double dot(const Vector2 t_vector) const;
	Vector2 projection(const Vector2 t_vector) const;
	Vector2 rejection(const Vector2 t_vector) const;
	double distance(const Vector2 t_vector) const;
	static Vector2 truncate(Vector2 t_vector, float const t_max);
	const std::string toString();

	Vector2 operator+(const Vector2 t_vector) const;
	Vector2 operator-(const Vector2 t_vector) const;
	Vector2 operator*(const double t_scalar) const;
	Vector2 operator*(const float t_scalar) const;
	Vector2 operator*(const int t_scalar) const;
	Vector2 operator*=(const float t_scalar);

	Vector2 operator/(const double t_scalar) const;
	Vector2 operator/(const float t_scalar) const;
	Vector2 operator/(const int t_scalar) const;
	Vector2 operator/(const Vector2 t_vector) const;
	Vector2 operator+=(const Vector2 t_vector);
	Vector2 operator-=(const Vector2 t_vector);
	Vector2 operator-();
	bool operator==(const Vector2 t_vector) const;
	bool operator!=(const Vector2 t_vector) const;

	operator sf::Vector2i() { return sf::Vector2i(static_cast<int>(x), static_cast<int>(y)); };
	operator sf::Vector2f() { return sf::Vector2f(static_cast<float>(x), static_cast<float>(y)); };
	operator sf::Vector2u() { return sf::Vector2u(static_cast<unsigned int>(x), static_cast<unsigned int>(y)); };

	double x;
	double y;
};

#endif // VECTOR2_H
