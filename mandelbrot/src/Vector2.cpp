#include "Vector2.h"

#define PI 3.14159265358979323846

#pragma region constructors

/// <summary>
/// Vector2 constructor.
/// </summary>
Vector2::Vector2() : x{ 0.0 }, y{ 0.0 }
{

}

/// <summary>
/// Vector2 destructor.
/// </summary>
/// <param name="t_x">X</param>
/// <param name="t_y">Y</param>
Vector2::Vector2(double t_x, double t_y) : x{ static_cast<double>(t_x) }, y{ static_cast<double>(t_y) }
{

}

/// <summary>
/// Vector2 constructor.
/// </summary>
/// <param name="t_vector">SFML Vector2i</param>
Vector2::Vector2(sf::Vector2i t_vector) : x{ static_cast<double>(t_vector.x) }, y{ static_cast<double>(t_vector.y) }
{

}

/// <summary>
/// Vector2 constructor.
/// </summary>
/// <param name="t_vector">SFML Vector2f</param>
Vector2::Vector2(sf::Vector2f t_vector) : x{ static_cast<double>(t_vector.x) }, y{ static_cast<double>(t_vector.y) }
{

}

/// <summary>
/// Vector2 constructor.
/// </summary>
/// <param name="t_vector">SFML Vector2u</param>
Vector2::Vector2(sf::Vector2u t_vector) : x{ static_cast<double>(t_vector.x) }, y{ static_cast<double>(t_vector.y) }
{

}

#pragma endregion

/// <summary>
/// Destructor for the Vector2 class.
/// </summary>
Vector2::~Vector2()
{

}

#pragma region methods

/// <summary>
/// Length.
/// </summary>
/// <returns>The length of the given vector.</returns>
double Vector2::length() const
{
	const double result = std::sqrt(x * x + y * y);
	return result;
}

/// <summary>
/// Length squared.
/// </summary>
/// <returns>The length of the vector squared.</returns>
double Vector2::lengthSquared() const
{
	const double result = x * x + y * y;
	return result;
}

/// <summary>
/// Unit.
/// </summary>
/// <returns>The given vector with a magnitude of 1.</returns>
Vector2 Vector2::unit() const
{
	double a = x / (std::sqrt(x * x + y * y));
	double b = y / (std::sqrt(x * x + y * y));
	return Vector2(a, b);
}

/// <summary>
/// Normalise.
/// </summary>
void Vector2::normalise()
{
	x = x / (std::sqrt(x * x + y * y));
	y = y / (std::sqrt(x * x + y * y));
}

/// <summary>
/// Rotate.
/// </summary>
/// <param name="t_angleRadians">The angle to rotate the given Vector2.</param>
/// <returns>A Vector2 rotated by the given angle.</returns>
Vector2 Vector2::rotateBy(float t_angleRadians)
{
	double a = cos(t_angleRadians) * x - sin(t_angleRadians) * y;
	double b = sin(t_angleRadians) * x + cos(t_angleRadians) * y;
	return Vector2(a, b);
}

/// <summary>
/// Angle between.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The angle between the two given vectors.</returns>
double Vector2::angleBetween(Vector2 t_vector) const
{
	double dotAB = (x * t_vector.x) + (y * t_vector.y);
	double magA = std::sqrt((x * x) + (y * y));
	double magB = std::sqrt((t_vector.x * t_vector.x) + (t_vector.y * t_vector.y));
	double magTotal = magA * magB;
	double angle = dotAB / magTotal;
	const double result = acos(angle) * 180.0 / PI;
	return result;
}

/// <summary>
/// Dot product.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The dot product of the two given vectors.</returns>
double Vector2::dot(Vector2 t_vector) const
{
	const double dotProduct = (x * t_vector.x) + (y * t_vector.y);
	return dotProduct;
}

/// <summary>
/// Projection.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>A vector parallel to the given vector.</returns>
Vector2 Vector2::projection(const Vector2 t_vector) const
{
	double dotAB = (x * t_vector.x) + (y * t_vector.y);
	double magB = std::sqrt((x * x) + (y * y));
	double division = dotAB / (magB * magB);
	double a = division * x;
	double b = division * y;
	return Vector2(a, b);
}

/// <summary>
/// Rejection.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>A vector orthogonal to the given vector.</returns>
Vector2 Vector2::rejection(const Vector2 t_vector) const
{
	double dotAB = (x * t_vector.x) + (y * t_vector.y);
	double magB = std::sqrt((x * x) + (y * y));
	double division = dotAB / (magB * magB);
	double a = t_vector.x - (division * x);
	double b = t_vector.y - (division * y);
	return Vector2(a, b);
}

/// <summary>
/// Find the distance between two vectors / points.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The distance between the two given vectors.</returns>
double Vector2::distance(const Vector2 t_vector) const
{
	double a = (t_vector.x - x) * (t_vector.x - x);
	double b = (t_vector.y - y) * (t_vector.y - y);

	return std::sqrt(a + b);
}

/// <summary>
/// Truncate.
/// </summary>
/// <param name="t_vector">The vector to truncate.</param>
/// <param name="t_max">The length to truncate the vector to.</param>
/// <returns>The truncated vector.</returns>
Vector2 Vector2::truncate(Vector2 t_vector, float const t_max)
{
	if (t_vector.length() > t_max)
	{
		t_vector = t_vector.unit();
		t_vector = t_vector * t_max;
	}

	return t_vector;
}

/// <summary>
/// To string.
/// </summary>
/// <returns>The vector as a string in the form [x, y].</returns>
const std::string Vector2::toString()
{
	const std::string output = "[" + std::to_string(x) + "," + std::to_string(y) + "]";
	return output;
}

#pragma endregion

#pragma region operators

/// <summary>
/// Overload of the plus operator.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator+(const Vector2 t_vector) const
{
	return Vector2(x + t_vector.x, y + t_vector.y);
}

/// <summary>
/// Overload of the minus operator.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator-(const Vector2 t_vector) const
{
	return Vector2(x - t_vector.x, y - t_vector.y);
}

/// <summary>
/// Overload of the multiplication operator (double).
/// </summary>
/// <param name="t_scalar">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator*(const double t_scalar) const
{
	double a = x * t_scalar;
	double b = y * t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the multiplication operator (float).
/// </summary>
/// <param name="t_scalar">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator*(const float t_scalar) const
{
	double a = x * t_scalar;
	double b = y * t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the multiplication operator (int).
/// </summary>
/// <param name="t_scalar">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator*(const int t_scalar) const
{
	double a = x * t_scalar;
	double b = y * t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the multiply-equals operator (float).
/// </summary>
/// <param name="t_scalar"></param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator*=(const float t_scalar)
{
	x *= t_scalar;
	y *= t_scalar;
	return Vector2(x, y);
}

/// <summary>
/// Overload of the division operator (double).
/// </summary>
/// <param name="t_scalar">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator/(const double t_scalar) const
{
	double a = x / t_scalar;
	double b = y / t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the division operator (float).
/// </summary>
/// <param name="t_scalar">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator/(const float t_scalar) const
{
	double a = x / t_scalar;
	double b = y / t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the division operator (int).
/// </summary>
/// <param name="t_divisor">The scalar value.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator/(const int t_scalar) const
{
	double a = x / t_scalar;
	double b = y / t_scalar;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the division operator (Vector2).
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator/(const Vector2 t_vector) const
{
	double a = x / t_vector.x;
	double b = y / t_vector.y;
	return Vector2(a, b);
}

/// <summary>
/// Overload of the plus-equals operator.
/// </summary>
/// <param name="t_scalar">The 2nd vector.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator+=(const Vector2 t_vector)
{
	x += t_vector.x;
	y += t_vector.y;
	return Vector2(x, y);
}

/// <summary>
/// Overload of the minus-equals operator.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator-=(const Vector2 t_vector)
{
	x -= t_vector.x;
	y -= t_vector.y;
	return Vector2(x, y);
}

/// <summary>
/// Overload of the unary negative operator.
/// </summary>
/// <returns>The resulting vector.</returns>
Vector2 Vector2::operator-()
{
	return Vector2(x * -1, y * -1);
}

/// <summary>
/// Overload of the equality operator.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>True if equal.</returns>
bool Vector2::operator==(const Vector2 t_vector) const
{
	const bool result{ x == t_vector.x && y == t_vector.y };
	return result;
}

/// <summary>
/// Overload of the inequality operator.
/// </summary>
/// <param name="t_vector">The 2nd vector.</param>
/// <returns>True if not equal.</returns>
bool Vector2::operator!=(const Vector2 t_vector) const
{
	const bool result{ x != t_vector.x || y != t_vector.y };
	return result;
}

#pragma endregion
