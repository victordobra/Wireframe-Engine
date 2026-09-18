#pragma once

#include "Core/Math/General/VecUtils.hpp"
#include "Core/Types/Defines.hpp"
#include <istream>
#include <ostream>
#include <string>
#include <vector>

namespace wfe {
	/// @brief A struct containing information regarding a TTF font.
	struct TTFFont {
		/// @brief A struct describing a component contour in a glyph.
		struct Contour {
			/// @brief The vector of points, alternating between on-curve and off-curve points.
			std::vector<Vec2f> points;
		};
		/// @brief A struct describing a character glyph.
		struct Glyph {
			/// @brief A vector containing the minimum coordinates of the current glyph.
			Vec2f minCoords;
			/// @brief A vector containing the maximum coordinates of the current glyph.
			Vec2f maxCoords;
			/// @brief The width of the glyph's left side bearing
			float leftSideBearing;
			/// @brief The width of the glyph's right side bearing
			float rightSideBearing;

			/// @brief The vector of shapes defining the glyph.
			std::vector<Contour> contours;
		};

		/// @brief The font's name.
		std::wstring fontName;
		/// @brief The font's style.
		std::wstring fontStyle;
		/// @brief The font's version.
		std::wstring fontVersion;
		/// @brief The font vendor's ID.
		uint32_t vendorID;

		/// @brief The distance from the baseline to the highest ascender.
		float ascent;
		/// @brief The distance from the baseline to the lowest descender.
		float descent;
		/// @brief The typographical line gap's size.
		float lineGap;

		/// @brief The offset of a subscript glyph.
		Vec2f subscriptOffset;
		/// @brief The size of a subscript glyph.
		Vec2f subscriptSize;
		/// @brief The offset of a superscript glyph.
		Vec2f superscriptOffset;
		/// @brief The size of a superscript glyph.
		Vec2f superscriptSize;
		/// @brief The Y position of the top of a strikeout stroke.
		float strikeoutPosition;
		/// @brief The height of a strikeout.
		float strikeoutHeight;
		/// @brief The Y position of the top of an underline stroke.
		float underlinePosition;
		/// @brief The height of an underline.
		float underlineHeight;

		/// @brief The map of glyph indices tied to all ASCII characters.
		size_t glyphMapping[256];
		/// @brief The vector of all glyphs defined in the font.
		std::vector<Glyph> glyphs;
	};

	/// @brief Reads a TTF font file's data.
	/// @param stream The input stream of the file. Must be binary.
	/// @param font A reference to the struct containing information regarding the file's font.
	/// @return True if the file was read successfully, otherwise false.
	bool ReadTTFFile(std::istream& stream, TTFFont& font);
	/// @brief Writes a TTF font file containing the given font data.
	/// @param stream The output stream of the file. Must be binary.
	/// @param font A struct containing information regarding the font to write.
	void WriteTTFFile(std::ostream& stream, const TTFFont& font);
}