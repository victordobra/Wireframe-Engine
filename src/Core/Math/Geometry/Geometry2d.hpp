#pragma once

#include "Core/Math/General/MathUtils.hpp"
#include "Core/Math/General/VecUtils.hpp"
#include "Core/Types/Defines.hpp"
#include <utility>

namespace wfe {
	/// @brief A struct containing the info of a 2D line, represented by the equation ax + by + c = 0.
	struct Line2d {
		/// @brief The a term in the line's equation.
		float a;
		/// @brief The b term in the line's equation.
		float b;
		/// @brief The c term in the line's equation.
		float c;
	};

	/// @brief Generates a 2D line with the given slope, passing through the given point.
	/// @param point The point that the line must pass through.
	/// @param slope The slope of the line.
	/// @return A struct representing the 2D line.
	Line2d GenerateLine(const Vec2f& point, float slope);
	/// @brief Generates a 2D line that passes through the given points.
	/// @param point1 The first point that the line should pass through.
	/// @param point2 The second point that the line should pass through.
	/// @return A struct representing the 2D line.
	Line2d GenerateLine(const Vec2f& point1, const Vec2f& point2);
	/// @brief Gets the given line's slope.
	/// @param line The line whose slope to get.
	/// @return The line's slope, or +/- infinity if the line is vertical.
	float LineGetSlope(const Line2d& line);
	/// @brief Gets the value of the line's expresion for the given point's coordinates.
	/// @param line The line whose expresion to calculate.
	/// @param point The point whose coordinates to use in the expression.
	/// @return The expression's value.
	float LineCalcExpr(const Line2d& line, const Vec2f& point);
	/// @brief Checks if the given point is on the given line.
	/// @param line The line to check in.
	/// @param point The point to check.
	/// @return True if the point is on the line, otherwise false.
	bool IsPointOnLine(const Line2d& line, const Vec2f& point);
	/// @brief Checks if the two lines are parallel.
	/// @param line1 The first line to check.
	/// @param line2 The second line to check.
	/// @return True if the two lines are parallel, otherwise false.
	bool AreParallel(const Line2d& line1, const Line2d& line2);
	/// @brief Gets the intersection of the two lines.
	/// @param line1 The first line to intersect.
	/// @param line2 The second line to intersect.
	/// @return The intersection point. Undefined if the lines are parallel.
	Vec2f GetIntersection(const Line2d& line1, const Line2d& line2);

	/// @brief Gets the given triangle's signed area, whose sign depends on the order of points.
	/// @param point1 The first point of the triangle.
	/// @param point2 The second point of the triangle.
	/// @param point3 The third point of the triangle.
	/// @return The triangle's area, positive if the points are ordered counter-clockwise, negative otherwise.
	float GetTriangleSignedArea(const Vec2f& point1, const Vec2f& point2, const Vec2f& point3);
	/// @brief Gets the given triangle's area.
	/// @param point1 The first point of the triangle.
	/// @param point2 The second point of the triangle.
	/// @param point3 The third point of the triangle.
	/// @return The triangle's area.
	float GetTriangleArea(const Vec2f& point1, const Vec2f& point2, const Vec2f& point3);
	/// @brief Gets the given polygon's signed area, whose sign depends on the order of points.
	/// @param pointCount The number of points in the polygon.
	/// @param points A pointer to the array of points in the polygon.
	/// @return The polygon's area, positive if the points are ordered counter-clockwise, negative otherwise.
	float GetPolygonSignedArea(size_t pointCount, const Vec2f* points);
	/// @brief Gets the given polygon's area.
	/// @param pointCount The number of points in the polygon.
	/// @param points A pointer to the array of points in the polygon.
	/// @return The polygon's area.
	float GetPolygonArea(size_t pointCount, const Vec2f* points);

	/// @brief Calculates the barycentric coordinates for the given point in relation to the given triangle.
	/// @param point The point whose coordinates to get.
	/// @param tri1 The first point of the triangle.
	/// @param tri2 The second point of the triangle.
	/// @param tri3 The third point of the triangle.
	/// @return A three-dimensional vector containing the three barycentric coordinates of the point.
	Vec3f GetBarycentricCoordinates(const Vec2f& point, const Vec2f& tri1, const Vec2f& tri2, const Vec2f& tri3);
	/// @brief Checks if the given point is inside of (or on) the given triangle.
	/// @param point The point to check.
	/// @param tri1 The first point of the triangle.
	/// @param tri2 The second point of the triangle.
	/// @param tri3 The third point of the triangle.
	/// @return True if the point is inside of the triangle, otherwise false.
	bool IsPointInsideTriangle(const Vec2f& point, const Vec2f& tri1, const Vec2f& tri2, const Vec2f& tri3);

	/// @brief Triangulates the given polygon.
	/// @param pointCount The number of points in the polygon.
	/// @param points A pointer to the array of points in the polygon.
	/// @param indices A pointer to the array in which the 3 * (pointCount - 1) indices will be stored.
	void TriangulatePolygon(size_t pointCount, const Vec2f* points, size_t* indices);
}