#include "Geometry2d.hpp"
#include <vector>

namespace wfe {
	// Public functions
	Line2d GenerateLine(const Vec2f& point, float slope) {
		return {
			.a = slope,
			.b = -1.0f,
			.c = -slope * point.x + point.y
		};
	}
	Line2d GenerateLine(const Vec2f& point1, const Vec2f& point2) {
		return {
			.a = point2.y - point1.y,
			.b = point1.x - point2.x,
			.c = point2.x * point1.y - point1.x * point2.y
		};
	}
	float LineGetSlope(const Line2d& line) {
		return -line.a / line.b;
	}
	float LineCalcExpr(const Line2d& line, const Vec2f& point) {
		return line.a * point.x + line.b * point.y + line.c;
	}
	bool IsPointOnLine(const Line2d& line, const Vec2f& point) {
		return LineCalcExpr(line, point) == 0.0f;
	}
	bool AreParallel(const Line2d& line1, const Line2d& line2) {
		return line1.a * line2.b == line2.a * line1.b;
	}
	Vec2f GetIntersection(const Line2d& line1, const Line2d& line2) {
		return {
			.x = (line2.c * line1.b - line1.c * line2.b) / (line1.a * line2.b - line2.a * line1.b),
			.y = (line2.c * line1.a - line1.c * line2.a) / (line2.a * line1.b - line1.a * line2.b)
		};
	}

	float GetTriangleSignedArea(const Vec2f& point1, const Vec2f& point2, const Vec2f& point3) {
		return (point1.x * point2.y + point2.x * point3.y + point3.x * point1.y - point1.x * point3.y - point2.x * point1.y - point3.x * point2.y) * 0.5f;
	}
	float GetTriangleArea(const Vec2f& point1, const Vec2f& point2, const Vec2f& point3) {
		return Abs(GetTriangleSignedArea(point1, point2, point3));
	}
	float GetPolygonSignedArea(size_t pointCount, const Vec2f* points) {
		// Exit if there aren't enough points
		if(pointCount < 3)
			return 0.0f;

		// Loop through all polygon lines, adding the signed area below them
		float area = 0.0f;
		for(size_t i = 0; i != pointCount - 1; ++i)
			area += points[i].x * points[i + 1].y - points[i + 1].x * points[i].y;
		area += points[pointCount - 1].x * points[0].y - points[0].x * points[pointCount - 1].y;
		
		return area * 0.5f;
	}
	float GetPolygonArea(size_t pointCount, const Vec2f* points) {
		return Abs(GetPolygonSignedArea(pointCount, points));
	}

	Vec3f GetBarycentricCoordinates(const Vec2f& point, const Vec2f& tri1, const Vec2f& tri2, const Vec2f& tri3) {
		// Get the position vectors relative to the first point of the triangle
		Vec2f vec = point - tri1, base1 = tri2 - tri1, base2 = tri3 - tri1;

		// Get the coefficients of the two base vectors 
		float coeff1 = (vec.x * base2.y - vec.y * base2.x) / (base1.x * base2.y - base1.y * base2.x);
		float coeff2 = (vec.x * base1.y - vec.y * base1.x) / (base2.x * base1.y - base2.y * base1.x);

		return { 1.0f - coeff1 - coeff2, coeff1, coeff2 };
	}
	bool IsPointInsideTriangle(const Vec2f& point, const Vec2f& tri1, const Vec2f& tri2, const Vec2f& tri3) {
		// Check if all of the barycentric coordinates are positive
		Vec3f coordinates = GetBarycentricCoordinates(point, tri1, tri2, tri3);
		return coordinates.x >= 0.0f && coordinates.y >= 0.0f && coordinates.z >= 0.0f;
	}

	void TriangulatePolygon(size_t pointCount, const Vec2f* points, size_t* indices) {
		// Exit the function if there are too few points
		if(pointCount < 3)
			return;

		// Check if the polygon is a triangle
		if(pointCount == 3) {
			// Write the three indices and exit the function
			indices[0] = 0;
			indices[1] = 1;
			indices[2] = 2;

			return;
		}

		// Check if the polygon is a quad
		if(pointCount == 4) {
			// Get the lengths of the two diagonals, if they are valid
			float diag1SqrLength;
			Line2d diag1Line = GenerateLine(points[0], points[2]);

			if(LineCalcExpr(diag1Line, points[1]) * LineCalcExpr(diag1Line, points[3]) < 0.0f) {
				diag1SqrLength = VecSqrMagnitude(points[2] - points[0]);
			} else {
				diag1SqrLength = FLOAT_INFINITY;
			}

			float diag2SqrLength;
			Line2d diag2Line = GenerateLine(points[1], points[3]);

			if(LineCalcExpr(diag2Line, points[0]) * LineCalcExpr(diag2Line, points[2]) < 0.0f) {
				diag2SqrLength = VecSqrMagnitude(points[3] - points[1]);
			} else {
				diag2SqrLength = FLOAT_INFINITY;
			}

			// Divide the quad by the shortest diagonal
			if(diag1SqrLength < diag2SqrLength) {
				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 2;

				indices[3] = 0;
				indices[4] = 2;
				indices[5] = 3;
			} else {
				indices[0] = 0;
				indices[1] = 1;
				indices[2] = 3;

				indices[3] = 1;
				indices[4] = 2;
				indices[5] = 3;
			}

			return;
		}

		// Calculate the polygon's signed area
		float totalSignedArea = GetPolygonSignedArea(pointCount, points);

		// Allocate and populate the index linked list
		static thread_local std::vector<std::pair<size_t, size_t>> indexList;
		indexList.resize(pointCount);
		size_t indexListSize = pointCount;

		for(size_t i = 1; i != pointCount - 1; ++i) {
			indexList[i].first = i - 1;
			indexList[i].second = i + 1;
		}

		indexList[0].first = pointCount - 1;
		indexList[0].second = 1;

		indexList[pointCount - 1].first = pointCount - 2;
		indexList[pointCount - 1].second = 0;

		// Use the ear-cutting algorithm to triangulate the polygon
		size_t indexTop = 0;

		for(size_t i = 0; indexListSize != 3; i = indexList[i].second) {
			// Get the three indices and their corresponding positions
			size_t ind1 = indexList[i].first;
			size_t ind2 = i;
			size_t ind3 = indexList[i].second;

			Vec2f pos1 = points[ind1], pos2 = points[ind2], pos3 = points[ind3];

			// Check if the triangle is inside of the poligon
			float triangleSignedArea = GetTriangleSignedArea(pos1, pos2, pos3);
			if(totalSignedArea * triangleSignedArea <= 0.0f)
				continue;
			
			// Check if any other point from the polygon is inside of the triangle
			bool valid = true;
			for(size_t j = 0; j != pointCount && valid; ++j) {
				if(j == ind1 || j == ind2 || j == ind3)
					continue;
				
				valid = !IsPointInsideTriangle(points[j], pos1, pos2, pos3);
			}

			// Check if the current triangle is valid
			if(valid) {
				// Add the triangle to the index list
				indices[indexTop++] = ind1;
				indices[indexTop++] = ind2;
				indices[indexTop++] = ind3;

				// Remove the current index from the list
				indexList[ind1].second = ind3;
				indexList[ind3].first = ind1;
				--indexListSize;

				// Check if there is only one triangle left
				if(indexListSize == 3) {
					indices[indexTop++] = ind1;
					indices[indexTop++] = ind3;
					indices[indexTop++] = indexList[ind3].second;
				}
			}
		}
	}
}