#include "TTFParser.hpp"
#include "Core/Math/General/MatUtils.hpp"
#include "Core/Utils/BinaryIO.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cwctype>
#include <vector>

#undef min
#undef max

namespace wfe {
	// Macros
#define WFE_ENCODE_TABLE_TAG_BE(type) (((uint32_t)type[0] << 24) | ((uint32_t)type[1] << 16) | ((uint32_t)type[2] << 8) | (uint32_t)type[3])

	// Structs
	struct Table {
		uint32_t tag;
		uint32_t checksum;
		uint32_t offset;
		uint32_t length;
		std::vector<uint8_t> data;

		uint16_t ReadUint16(uint32_t& offset) const {
			uint16_t val = (uint16_t)(data[offset] << 8) | (uint16_t)data[offset + 1];
			offset += sizeof(uint16_t);
			return val;
		}
		uint32_t ReadUint32(uint32_t& offset) const {
			uint32_t val = ((uint32_t)data[offset] << 24) | ((uint32_t)data[offset + 1] << 16) | ((uint32_t)data[offset + 2] << 8) | (uint32_t)data[offset + 3];
			offset += sizeof(uint32_t);
			return val;
		}
		uint64_t ReadUint64(uint32_t& offset) const {
			uint64_t val = ((uint64_t)data[offset] << 56) | ((uint64_t)data[offset + 1] << 48) | ((uint64_t)data[offset + 2] << 40) | ((uint64_t)data[offset + 3] << 32) | ((uint64_t)data[offset + 4] << 24) | ((uint64_t)data[offset + 5] << 16) | ((uint64_t)data[offset + 6] << 8) | (uint64_t)data[offset + 7];
			offset += sizeof(uint64_t);
			return val;
		}
		void WriteUint16(uint32_t& offset, uint16_t val) {
			// Enlarge the data vector, if required
			if(offset + sizeof(uint16_t) > data.size())
				data.resize(offset + sizeof(uint16_t));

			data[offset++] = (uint8_t)(val >> 8);
			data[offset++] = (uint8_t)val;
		}
		void WriteUint32(uint32_t& offset, uint32_t val) {
			// Enlarge the data vector, if required
			if(offset + sizeof(uint32_t) > data.size())
				data.resize(offset + sizeof(uint32_t));

			data[offset++] = (uint8_t)(val >> 24);
			data[offset++] = (uint8_t)(val >> 16);
			data[offset++] = (uint8_t)(val >> 8);
			data[offset++] = (uint8_t)val;
		}
		void WriteUint64(uint32_t& offset, uint64_t val) {
			// Enlarge the data vector, if required
			if(offset + sizeof(uint64_t) > data.size())
				data.resize(offset + sizeof(uint64_t));

			data[offset++] = (uint8_t)(val >> 56);
			data[offset++] = (uint8_t)(val >> 48);
			data[offset++] = (uint8_t)(val >> 40);
			data[offset++] = (uint8_t)(val >> 32);
			data[offset++] = (uint8_t)(val >> 24);
			data[offset++] = (uint8_t)(val >> 16);
			data[offset++] = (uint8_t)(val >> 8);
			data[offset++] = (uint8_t)val;
		}
		void AssignChecksum() {
			// Resize the data vector to be aligned to the size of uint32_t
			data.resize((data.size() + sizeof(uint32_t) - 1) & ~(sizeof(uint32_t) - 1), 0);

			// Calculate the checksum
			checksum = 0;
			for(uint32_t offset = 0; offset != data.size();)
				checksum += ReadUint32(offset);
		}
	};
	struct TableSet {
		Table cmapTable;
		Table glyfTable;
		Table headTable;
		Table hheaTable;
		Table hmtxTable;
		Table locaTable;
		Table maxpTable;
		Table nameTable;
		Table os2Table;
		Table postTable;
	};
	struct CmapSegment {
		uint16_t endCode;
		uint16_t startCode;
		uint16_t idDelta;
		uint16_t idRangeOffset;
	};

	// Constants
	static const uint32_t TTF_VERSION_1_0 = 0x00010000;
	static const uint32_t HEAD_MAGIC_NUMBER = 0x5f0f3cf5;
	static const uint32_t CHECKSUM_ADJUSTMENT_DEC = 0xb1b0afba;

	static const uint32_t TABLE_TAG_CMAP = WFE_ENCODE_TABLE_TAG_BE("cmap");
	static const uint32_t TABLE_TAG_GLYF = WFE_ENCODE_TABLE_TAG_BE("glyf");
	static const uint32_t TABLE_TAG_HEAD = WFE_ENCODE_TABLE_TAG_BE("head");
	static const uint32_t TABLE_TAG_HHEA = WFE_ENCODE_TABLE_TAG_BE("hhea");
	static const uint32_t TABLE_TAG_HMTX = WFE_ENCODE_TABLE_TAG_BE("hmtx");
	static const uint32_t TABLE_TAG_LOCA = WFE_ENCODE_TABLE_TAG_BE("loca");
	static const uint32_t TABLE_TAG_MAXP = WFE_ENCODE_TABLE_TAG_BE("maxp");
	static const uint32_t TABLE_TAG_NAME = WFE_ENCODE_TABLE_TAG_BE("name");
	static const uint32_t TABLE_TAG_OS2 = WFE_ENCODE_TABLE_TAG_BE("OS/2");
	static const uint32_t TABLE_TAG_POST = WFE_ENCODE_TABLE_TAG_BE("post");

	static const uint8_t ON_CURVE_POINT = 0x01;
	static const uint8_t X_SHORT_VECTOR = 0x02;
	static const uint8_t Y_SHORT_VECTOR = 0x04;
	static const uint8_t REPEAT_FLAG = 0x08;
	static const uint8_t X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR = 0x10;
	static const uint8_t Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR = 0x20;

	static const uint16_t ARG_1_AND_2_ARE_WORDS = 0x0001;
	static const uint16_t ARGS_ARE_XY_VALUES = 0x0002;
	static const uint16_t WE_HAVE_A_SCALE = 0x0008;
	static const uint16_t MORE_COMPONENTS = 0x0020;
	static const uint16_t WE_HAVE_AN_X_AND_Y_SCALE = 0x0040;
	static const uint16_t WE_HAVE_A_TWO_BY_TWO = 0x0080;
	static const uint16_t SCALED_COMPONENT_OFFSET = 0x0800;

	static const float INV_DOT_14 = 1.0f / 16384.0f;
	static const uint16_t WRITE_UNITS_PER_EM = 1024;

	static const uint32_t TTF_HEADER_SIZE = sizeof(uint32_t) + sizeof(uint16_t) * 4;
	static const uint32_t TABLE_RECORD_SIZE = sizeof(uint32_t) * 4;

	static const wchar_t* DEFAULT_FONT_NAME = L"Unknown";
	static const wchar_t* DEFAULT_FONT_STYLE = L"Regular";
	static const wchar_t* DEFAULT_FONT_VERSION = L"Version 1.0";
	static const size_t MAX_POSTSCRIPT_NAME_SIZE = 63;

	// Internal helper functions
	static uint16_t CalcSearchRange(uint16_t count) {
		uint16_t searchRange = 1;
		for(; searchRange <= count; searchRange <<= 1);
		return searchRange;
	}
	static uint16_t CalcEntrySelector(uint16_t count) {
		uint16_t entrySelector = 0;
		for(; 1 << (entrySelector + 1) <= count; ++entrySelector);
		return entrySelector;
	}

	static void ReadGlyphPointCoords(const Table& glyfTable, uint32_t& offset, uint32_t coordInd, uint8_t shortVectorFlag, uint8_t sameOrPositiveFlag, float invUnitsPerEm, const std::vector<uint8_t>& flags, const std::vector<uint16_t>& endPtsOfContours, TTFFont::Glyph& glyph) {
		uint16_t contourIndex = 0, pointIndex = 0;
		int32_t currCoord = 0;

		for(size_t i = 0; i != flags.size(); ++i) {
			// Read the current coordinate
			if(flags[i] & shortVectorFlag) {
				uint8_t delta = glyfTable.data[offset++];

				if(flags[i] & sameOrPositiveFlag) {
					currCoord += (int32_t)delta;
				} else {
					currCoord -= (int32_t)delta;
				}
			} else if(!(flags[i] & sameOrPositiveFlag)) {
				int16_t delta = (int16_t)glyfTable.ReadUint16(offset);
				currCoord += delta;
			}

			float floatCoord = currCoord * invUnitsPerEm;

			// Check if another on-curve/off-curve point must be added
			if(pointIndex && (flags[i - 1] & ON_CURVE_POINT) == (flags[i] & ON_CURVE_POINT)) {
				float prevCoord = glyph.contours[contourIndex].points[pointIndex - 1][coordInd];
				float midCoord = (prevCoord + floatCoord) * 0.5f;
				glyph.contours[contourIndex].points[pointIndex++][coordInd] = midCoord;
			}

			// Add the current coordinate
			glyph.contours[contourIndex].points[pointIndex++][coordInd] = floatCoord;

			// Check if the current contour just ended
			if(i == endPtsOfContours[contourIndex]) {
				// Check if another point must be added
				if(pointIndex & 1) {
					float firstCoord = glyph.contours[contourIndex].points[0][coordInd];
					float midCoord = (firstCoord + floatCoord) * 0.5f;
					glyph.contours[contourIndex].points[pointIndex++][coordInd] = midCoord;
				}

				// Move on to the net contour
				++contourIndex;
				pointIndex = 0;
			}
		}
	}
	static void ReadCompositeGlyphTransform(const Table& glyfTable, uint32_t& offset, uint16_t flags, float invUnitsPerEm, Vec2f& offsetPos, Mat2x2f& transform) {
		// Read the offset position
		if(flags & ARG_1_AND_2_ARE_WORDS) {
			offsetPos.x = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			offsetPos.y = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
		} else {
			offsetPos.x = (int8_t)glyfTable.data[offset++] * invUnitsPerEm;
			offsetPos.y = (int8_t)glyfTable.data[offset++] * invUnitsPerEm;
		}

		// Read the transform matrix
		if(flags & WE_HAVE_A_SCALE) {
			float scale = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;

			transform = { 
				scale, 0.0f,
				0.0f,  scale
			};
		} else if(flags & WE_HAVE_AN_X_AND_Y_SCALE) {
			float scaleX = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;
			float scaleY = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;

			transform = { 
				scaleX, 0.0f,
				0.0f,   scaleY
			};
		} else if(flags & WE_HAVE_A_TWO_BY_TWO) {
			float scale00 = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;
			float scale01 = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;
			float scale10 = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;
			float scale11 = (int16_t)glyfTable.ReadUint16(offset) * INV_DOT_14;

			transform = {
				scale00, scale01,
				scale10, scale11
			};
		} else {
			transform = MAT2X2F_IDENTITY;
		}

		// Scale the offset, if requested
		if(flags & SCALED_COMPONENT_OFFSET)
			offsetPos = offsetPos * transform;
	}

	static bool ReadTableSet(std::istream& stream, uint32_t tableCount, TableSet& tableSet) {
		uint32_t goodTableCount = 0;

		for(uint16_t i = 0; i != tableCount; ++i) {
			// Read the current table's metadata
			uint32_t tag = BinaryReadUint32BE(stream);
			uint32_t checksum = BinaryReadUint32BE(stream);
			uint32_t offset = BinaryReadUint32BE(stream);
			uint32_t length = BinaryReadUint32BE(stream);

			// Check if the current table is a supported type
			Table* readTable = nullptr;
			switch(tag) {
			case TABLE_TAG_CMAP:
				readTable = &tableSet.cmapTable;
				break;
			case TABLE_TAG_GLYF:
				readTable = &tableSet.glyfTable;
				break;
			case TABLE_TAG_HEAD:
				readTable = &tableSet.headTable;
				break;
			case TABLE_TAG_HHEA:
				readTable = &tableSet.hheaTable;
				break;
			case TABLE_TAG_HMTX:
				readTable = &tableSet.hmtxTable;
				break;
			case TABLE_TAG_LOCA:
				readTable = &tableSet.locaTable;
				break;
			case TABLE_TAG_MAXP:
				readTable = &tableSet.maxpTable;
				break;
			case TABLE_TAG_NAME:
				readTable = &tableSet.nameTable;
				break;
			case TABLE_TAG_OS2:
				readTable = &tableSet.os2Table;
				break;
			case TABLE_TAG_POST:
				readTable = &tableSet.postTable;
				break;
			}

			// Check if the current table is valid
			if(readTable) {
				// Increment the good table count
				++goodTableCount;

				// Set the table's header data
				readTable->tag = tag;
				readTable->checksum = checksum;
				readTable->offset = offset;
				readTable->length = length;

				// Save the old pointer location and move to the table's start
				std::streampos oldPos = stream.tellg();
				stream.seekg(offset, std::ios::beg);

				// Read the table's data
				readTable->data.resize(readTable->length);
				stream.read((char*)readTable->data.data(), readTable->length);

				// Move the stream cursor back
				stream.seekg(oldPos, std::ios::beg);
			}
		}

		// Check if all required tables were read
		return goodTableCount == 10;
	}
	static bool ReadHeadTable(const Table& headTable, uint16_t& unitsPerEm, int16_t& indexToLocFormat) {
		// Read all head table data
		uint32_t offset = 0;

		uint32_t headVersion = headTable.ReadUint32(offset);
		offset += sizeof(uint32_t) * 2; // fontRevision, checksumAdjustment
		uint32_t headMagicNumber = headTable.ReadUint32(offset);
		offset += sizeof(uint16_t); // flags
		unitsPerEm = headTable.ReadUint16(offset);
		offset += sizeof(uint64_t) * 2; // createdDate, modifiedDate
		offset += sizeof(int16_t) * 4; // xMin, yMin, xMax, yMax
		offset += sizeof(uint16_t) * 2 + sizeof(int16_t); // maxStyle, lowestRecPPEM, fontDirectionHint
		indexToLocFormat = (int16_t)headTable.ReadUint16(offset);

		// Check the head table's version and magic number
		return headVersion == TTF_VERSION_1_0 && headMagicNumber == HEAD_MAGIC_NUMBER;
	}
	static bool ReadMaxpTable(const Table& maxpTable, TTFFont& font) {
		// Read all maxp table data
		uint32_t offset = 0;

		uint32_t maxpVersion = maxpTable.ReadUint32(offset);
		uint16_t numGlyphs = maxpTable.ReadUint16(offset);
		offset += sizeof(uint16_t) * 12; // maxPoints, maxContours, maxCompositePoints, maxCompositeContours, maxZones, maxTwilightPoints, maxStorage, maxFunctionDefs, maxInstructionDefs, maxStackElements, maxSizeOfInstructions, maxCompositeElements
		uint16_t maxComponentDepth = maxpTable.ReadUint16(offset);

		// Check the table's version and max component depth
		if(maxpVersion != TTF_VERSION_1_0 || maxComponentDepth > 1)
			return false;
		
		// Resize the font's glyph vector and exit the function
		font.glyphs.resize(numGlyphs);
		return true;
	}
	static uint32_t FindCmapUnicodeTableOffset(const Table& cmapTable) {
		// Read the cmap table header
		uint32_t offset = 0;

		uint16_t cmapVersion = cmapTable.ReadUint16(offset);
		uint16_t cmapNumTables = cmapTable.ReadUint16(offset);

		// Check the cmap table's version
		if(cmapVersion != 0)
			return UINT32_T_MAX;

		// Read all mapping tables
		for(uint16_t i = 0; i != cmapNumTables; ++i) {
			uint16_t platformID = cmapTable.ReadUint16(offset);
			uint16_t encodingID = cmapTable.ReadUint16(offset);
			uint32_t tableOffset = cmapTable.ReadUint32(offset);

			if(platformID == 0 && encodingID <= 4) {
				// Read the current table's format
				uint16_t tableFormat = cmapTable.ReadUint16(tableOffset);
				tableOffset -= sizeof(uint16_t);

				if(tableFormat == 4)
					return tableOffset;
			}
		}

		return UINT32_T_MAX;
	}
	static bool ReadCmapUnicodeTable(const Table& cmapTable, TTFFont& font, uint32_t unicodeOffset) {
		// Read the table's header data
		uint32_t offset = unicodeOffset;

		offset += sizeof(uint16_t); // cmapFormat
		uint16_t tableLength = cmapTable.ReadUint16(offset);
		offset += sizeof(uint16_t); // language
		uint16_t segCount = cmapTable.ReadUint16(offset) >> 1;
		offset += sizeof(uint16_t) * 3; // cmapSearchRange, cmapEntrySelector, cmapRangeShift

		// Read all cmap segment descriptors
		std::vector<CmapSegment> segments(segCount);

		for(uint16_t i = 0; i != segCount; ++i)
			segments[i].endCode = cmapTable.ReadUint16(offset);
		offset += sizeof(uint16_t); // reservedPad
		for(uint16_t i = 0; i != segCount; ++i)
			segments[i].startCode = cmapTable.ReadUint16(offset);
		for(uint16_t i = 0; i != segCount; ++i)
			segments[i].idDelta = cmapTable.ReadUint16(offset);
		for(uint16_t i = 0; i != segCount; ++i)
			segments[i].idRangeOffset = cmapTable.ReadUint16(offset);

		// Read the glyph ID array
		std::vector<uint16_t> glyphIdArray((tableLength - (offset - unicodeOffset)) >> 1);
		for(size_t i = 0; i != glyphIdArray.size(); ++i)
			glyphIdArray[i] = cmapTable.ReadUint16(offset);
		
		// Read all glyph IDs
		for(uint16_t i = 0; i != segCount; ++i) {
			CmapSegment segment = segments[i];

			for(uint32_t j = segment.startCode; j <= segment.endCode; ++j) {
				// Get the glyph ID for the current character
				uint16_t glyphId;
				if(segment.idRangeOffset) {
					uint16_t indexOffset = (j - segment.startCode) + (segment.idRangeOffset >> 1);
					uint16_t index = indexOffset + i - segCount;
					if(index >= glyphIdArray.size())
						return false;

					glyphId = glyphIdArray[index];
					if(glyphId)
						glyphId += segment.idDelta;
				} else {
					glyphId = j + segment.idDelta;
				}

				// Convert the current character
				font.glyphMapping.insert({ (wchar_t)j, glyphId });
			}
		}

		return true;
	}
	static void ReadLocaTable(const Table& locaTable, size_t glyphCount, int16_t indexToLocFormat, std::vector<uint32_t>& glyphOffsets) {
		// Resize the glyph offset vector
		glyphOffsets.resize(glyphCount + 1);

		// Read all glyph offsets
		uint32_t offset = 0;

		if(indexToLocFormat == 0) {
			for(size_t i = 0; i != glyphCount + 1; ++i)
				glyphOffsets[i] = (uint32_t)locaTable.ReadUint16(offset) << 1;
		} else {
			for(size_t i = 0; i != glyphCount + 1; ++i)
				glyphOffsets[i] = locaTable.ReadUint32(offset);
		}
	}
	static bool ReadSimpleGlyphs(const Table& glyfTable, const std::vector<uint32_t>& glyphOffsets, float invUnitsPerEm, TTFFont& font) {
		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Skip the current glyph if it has no outline
			if(glyphOffsets[i] == glyphOffsets[i + 1])
				continue;

			// Read the number of contours and gkip the glyph if composite
			uint32_t offset = glyphOffsets[i];
			int16_t numberOfContours = (int16_t)glyfTable.ReadUint16(offset);

			if(numberOfContours == -1)
				continue;

			// Set the glyph's min and max coords
			font.glyphs[i].minCoords.x = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].minCoords.y = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].maxCoords.x = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].maxCoords.y = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			
			// Read the contour end indices
			std::vector<uint16_t> endPtsOfContours(numberOfContours);
			for(uint16_t j = 0; j != numberOfContours; ++j)
				endPtsOfContours[j] = glyfTable.ReadUint16(offset);
			
			// Skip all instructions
			uint16_t instructionLength = glyfTable.ReadUint16(offset);
			offset += instructionLength;

			// Read all coordinate flags
			uint16_t coordCount = endPtsOfContours.back() + 1;

			std::vector<uint8_t> flags;
			flags.reserve(coordCount);
			while(flags.size() != coordCount) {
				uint8_t flag = glyfTable.data[offset++];
				flags.push_back(flag);

				// check if the current flag is repeated
				if(flag & REPEAT_FLAG) {
					flag ^= REPEAT_FLAG;
					uint8_t repeatCount = glyfTable.data[offset++];
					for(uint8_t j = 0; j != repeatCount; ++j)
						flags.push_back(flag);
				}
			}

			// Set the number of points for each contour
			font.glyphs[i].contours.resize(numberOfContours);

			uint32_t contourInd = 0, currGlyphSize = 0;
			for(uint16_t j = 0; j != coordCount; ++j) {
				// Add an additional point to maintain the on-curve/off-curve point alternation
				if(currGlyphSize && (flags[j - 1] & ON_CURVE_POINT) == (flags[j] & ON_CURVE_POINT))
					++currGlyphSize;
				++currGlyphSize;

				if(j == endPtsOfContours[contourInd]) {
					// Add an additional point, if required, resize the point vector and move on to the next point
					currGlyphSize += (currGlyphSize & 1);
					font.glyphs[i].contours[contourInd].points.resize(currGlyphSize);

					++contourInd;
					currGlyphSize = 0;
				}
			}

			// Read all point coordinates
			ReadGlyphPointCoords(glyfTable, offset, 0, X_SHORT_VECTOR, X_IS_SAME_OR_POSITIVE_X_SHORT_VECTOR, invUnitsPerEm, flags, endPtsOfContours, font.glyphs[i]);
			ReadGlyphPointCoords(glyfTable, offset, 1, Y_SHORT_VECTOR, Y_IS_SAME_OR_POSITIVE_Y_SHORT_VECTOR, invUnitsPerEm, flags, endPtsOfContours, font.glyphs[i]);

			// Make sure that every contour starts with an on-curve point
			uint16_t firstInd = 0;

			for(uint16_t j = 0; j != numberOfContours; ++j) {
				// Shift the contour's points left by one if the first point is off-curve
				if(!(flags[firstInd] & ON_CURVE_POINT)) {
					Vec2f firstPoint = font.glyphs[i].contours[j].points.front();
					font.glyphs[i].contours[j].points.erase(font.glyphs[i].contours[j].points.begin());
					font.glyphs[i].contours[j].points.push_back(firstPoint);
				}

				// Set the first ind for the next contour
				firstInd = endPtsOfContours[j] + 1;
			}
		}

		return true;
	}
	static bool ReadCompositeGlyphs(const Table& glyfTable, const std::vector<uint32_t>& glyphOffsets, float invUnitsPerEm, TTFFont& font) {
		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Skip the current glyph if it has no outline
			if(glyphOffsets[i] == glyphOffsets[i + 1])
				continue;

			// Read the number of contours and gkip the glyph if not composite
			uint32_t offset = glyphOffsets[i];
			int16_t numberOfContours = (int16_t)glyfTable.ReadUint16(offset);

			if(numberOfContours != -1)
				continue;

			// Set the glyph's min and max coords
			font.glyphs[i].minCoords.x = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].minCoords.y = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].maxCoords.x = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			font.glyphs[i].maxCoords.y = (int16_t)glyfTable.ReadUint16(offset) * invUnitsPerEm;
			
			// Read all component glyph descriptions
			uint16_t flags, glyphIndex;
			do {
				// Read the flags and the glyph index
				flags = glyfTable.ReadUint16(offset);
				glyphIndex = glyfTable.ReadUint16(offset);

				if(!(flags & ARGS_ARE_XY_VALUES))
					return false;
				
				// Read the composite glyph's transform
				Vec2f offsetPos;
				Mat2x2f transform;
				ReadCompositeGlyphTransform(glyfTable, offset, flags, invUnitsPerEm, offsetPos, transform);
				
				// Add all components from the child glyph to the parent glyph
				for(size_t j = 0; j != font.glyphs[glyphIndex].contours.size(); ++j) {
					font.glyphs[i].contours.push_back(font.glyphs[glyphIndex].contours[j]);

					// Scale and offset all added points
					for(size_t k = 0; k != font.glyphs[i].contours.back().points.size(); ++k)
						font.glyphs[i].contours.back().points[k] = font.glyphs[i].contours.back().points[k] * transform + offsetPos;
				}
			} while(flags & MORE_COMPONENTS);
		}

		return true;
	}
	static bool ReadHheaTable(const Table& hheaTable, float invUnitsPerEm, uint16_t& numberOfHMetrics, TTFFont& font) {
		// Read the hhea table data
		uint32_t offset = 0;

		uint32_t hheaVersion = hheaTable.ReadUint32(offset);
		int16_t ascender = (int16_t)hheaTable.ReadUint16(offset);
		int16_t descender = (int16_t)hheaTable.ReadUint16(offset);
		int16_t lineGap = (int16_t)hheaTable.ReadUint16(offset);
		offset += sizeof(uint16_t) + sizeof(int16_t) * 6; // advanceWidthMax, minLeftSideBearing, minRightSideBearing, xMaxExtent, caretSlopeRise, caretSlopeRun, caretOffset
		offset += sizeof(uint16_t) * 4; // reserved
		int16_t metricDataFormat = (int16_t)hheaTable.ReadUint16(offset);
		numberOfHMetrics = hheaTable.ReadUint16(offset);

		// Check the hhea table's version and format
		if(hheaVersion != TTF_VERSION_1_0 || metricDataFormat != 0)
			return false;
		
		// Set the font's vertical data
		font.ascent = ascender * invUnitsPerEm;
		font.descent = descender * invUnitsPerEm;
		font.lineGap = lineGap * invUnitsPerEm;

		return true;
	}
	static bool ReadHmtxTable(const Table& hmtxTable, uint16_t numberOfHMetrics, float invUnitsPerEm, TTFFont& font) {
		// Read all horizontal metrics
		uint32_t offset = 0;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			uint16_t advanceWidth = (i < numberOfHMetrics) ? hmtxTable.ReadUint16(offset) : 0;
			int16_t lsb = (int16_t)hmtxTable.ReadUint16(offset);

			font.glyphs[i].leftSideBearing = lsb * invUnitsPerEm;
			if(advanceWidth || font.glyphs[i].contours.empty()) {
				font.glyphs[i].rightSideBearing = advanceWidth * invUnitsPerEm - (font.glyphs[i].maxCoords.x - font.glyphs[i].minCoords.x) - font.glyphs[i].leftSideBearing;
			} else {
				font.glyphs[i].rightSideBearing = 0.0f;
			}
		}

		return true;
	}
	static bool ReadNameTable(const Table& nameTable, TTFFont& font) {
		// Read the core name table data
		uint32_t offset = 0;

		uint16_t version = nameTable.ReadUint16(offset);
		uint16_t count = nameTable.ReadUint16(offset);
		uint16_t storageOffset = nameTable.ReadUint16(offset);

		// Check the table's version
		if(version != 0 && version != 1)
			return false;
		
		// Find all relevant string offsets
		uint16_t nameOffset = 0, nameLength = 0;
		uint16_t styleOffset = 0, styleLength = 0;
		uint16_t versionOffset = 0, versionLength = 0;

		for(uint16_t i = 0; i != count; ++i) {
			// read the string entry data
			uint16_t platformID = nameTable.ReadUint16(offset);
			uint16_t encodingID = nameTable.ReadUint16(offset);
			uint16_t languageID = nameTable.ReadUint16(offset);
			uint16_t nameID = nameTable.ReadUint16(offset);
			uint16_t length = nameTable.ReadUint16(offset);
			uint16_t stringOffset = nameTable.ReadUint16(offset);

			// Check if the current encoding is valid
			if((platformID != 0 || encodingID != 3) && (platformID != 3 || encodingID != 1))
				continue;

			// Set the relevant string offset, if required
			if(nameID == 1) {
				nameOffset = storageOffset + stringOffset;
				nameLength = length >> 1;
			} else if(nameID == 2) {
				styleOffset = storageOffset + stringOffset;
				styleLength = length >> 1;
			} else if(nameID == 5) {
				versionOffset = storageOffset + stringOffset;
				versionLength = length >> 1;
			}
		}

		// Read the name, if given
		if(nameLength) {
			offset = nameOffset;
			font.fontName.resize(nameLength);
			for(uint16_t i = 0; i != nameLength; ++i)
				font.fontName[i] = (wchar_t)nameTable.ReadUint16(offset);
		} else {
			font.fontName = DEFAULT_FONT_NAME;
		}

		// Read the style, if given
		if(styleLength) {
			offset = styleOffset;
			font.fontStyle.resize(styleLength);
			for(uint16_t i = 0; i != styleLength; ++i)
				font.fontStyle[i] = (wchar_t)nameTable.ReadUint16(offset);
		} else {
			font.fontStyle = DEFAULT_FONT_STYLE;
		}

		// Read the version, if given
		if(versionLength) {
			offset = versionOffset;
			font.fontVersion.resize(versionLength);
			for(uint16_t i = 0; i != versionLength; ++i)
				font.fontVersion[i] = (wchar_t)nameTable.ReadUint16(offset);
		} else {
			font.fontVersion = DEFAULT_FONT_VERSION;
		}

		return true;
	}
	static bool ReadOS2Table(const Table& os2Table, float invUnitsPerEm, TTFFont& font) {
		// Read all relevant OS2 info
		uint32_t offset = 0;

		offset += sizeof(uint16_t) * 5; // version, xAvgCharWidth, usWeightClass, usWidthClass, fsType
		font.subscriptSize.x = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.subscriptSize.y = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.subscriptOffset.x = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.subscriptOffset.y = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.superscriptSize.x = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.superscriptSize.y = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.superscriptOffset.x = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.superscriptOffset.y = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.strikeoutHeight = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		font.strikeoutPosition = (int16_t)os2Table.ReadUint16(offset) * invUnitsPerEm;
		offset += sizeof(int16_t) + sizeof(uint8_t) * 10 + sizeof(uint32_t) * 4; // sFamilyClass, panose, ulUnicodeRange
		font.vendorID = os2Table.ReadUint32(offset);

		return true;
	}
	static bool ReadPostTable(const Table& postTable, float invUnitsPerEm, TTFFont& font) {
		// Read all relevant post info
		uint32_t offset = 0;

		offset += sizeof(uint32_t) * 2; // version, italicAngle
		font.underlinePosition = (int16_t)postTable.ReadUint16(offset) * invUnitsPerEm;
		font.underlineHeight = (int16_t)postTable.ReadUint16(offset) * invUnitsPerEm;

		return true;
	}

	static void WriteHeadTable(Table& headTable, const TTFFont& font) {
		// Calculate the total bounding coordinates
		int16_t xMin = INT16_T_MAX, yMin = INT16_T_MAX;
		int16_t xMax = INT16_T_MIN, yMax = INT16_T_MIN;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Convert the current glyph's bounds to 16-bit integers
			int16_t xMinGlyph = (int16_t)std::roundf(font.glyphs[i].minCoords.x * WRITE_UNITS_PER_EM);
			int16_t yMinGlyph = (int16_t)std::roundf(font.glyphs[i].minCoords.y * WRITE_UNITS_PER_EM);
			int16_t xMaxGlyph = (int16_t)std::roundf(font.glyphs[i].maxCoords.x * WRITE_UNITS_PER_EM);
			int16_t yMaxGlyph = (int16_t)std::roundf(font.glyphs[i].maxCoords.y * WRITE_UNITS_PER_EM);

			// Update the general bounding box
			xMin = std::min(xMin, xMinGlyph);
			yMin = std::min(yMin, xMinGlyph);
			xMax = std::max(xMax, xMaxGlyph);
			yMax = std::max(yMax, xMaxGlyph);
		}

		// Write the head table's data
		uint32_t offset = 0;

		headTable.WriteUint32(offset, TTF_VERSION_1_0); // headVersion
		headTable.WriteUint32(offset, 0); // fontRevision
		headTable.WriteUint32(offset, 0); // checksumAdjustment
		headTable.WriteUint32(offset, HEAD_MAGIC_NUMBER); // magicNumber
		headTable.WriteUint16(offset, 0); // flags
		headTable.WriteUint16(offset, WRITE_UNITS_PER_EM); // unitsPerEm
		headTable.WriteUint64(offset, 0); // createdDate
		headTable.WriteUint64(offset, 0); // modifiedDate
		headTable.WriteUint16(offset, xMin); // xMin
		headTable.WriteUint16(offset, yMin); // yMin
		headTable.WriteUint16(offset, xMax); // xMax
		headTable.WriteUint16(offset, yMax); // yMax
		headTable.WriteUint16(offset, 0); // macStyle
		headTable.WriteUint16(offset, 0); // lowestRecPPEM
		headTable.WriteUint16(offset, 2); // fontDirectionHint
		headTable.WriteUint16(offset, 1); // indexToLocFormat
		headTable.WriteUint16(offset, 0); // glyphDataFormat
	}
	static void WriteMaxpTable(Table& maxpTable, const TTFFont& font) {
		// Set the maximum number of points and contours
		uint16_t maxPoints = 0, maxContours = 0;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Get the total number of points in the glyph
			uint16_t pointCount = 0;
			for(size_t j = 0; j != font.glyphs[i].contours.size(); ++j)
				pointCount += (uint16_t)font.glyphs[i].contours[j].points.size();
			
			// Update the max values
			maxPoints = std::max(maxPoints, pointCount);
			maxContours = std::max(maxContours, (uint16_t)font.glyphs[i].contours.size());
		}

		// Write the maxp table's data
		uint32_t offset = 0;

		maxpTable.WriteUint32(offset, TTF_VERSION_1_0); // maxpVersion
		maxpTable.WriteUint16(offset, (uint16_t)font.glyphs.size()); // numGlyphs
		maxpTable.WriteUint16(offset, maxPoints); // maxPoints
		maxpTable.WriteUint16(offset, maxContours); // maxContours
		maxpTable.WriteUint16(offset, 0); // maxCompositePoints
		maxpTable.WriteUint16(offset, 0); // maxCompositeContours
		maxpTable.WriteUint16(offset, 1); // maxZones
		maxpTable.WriteUint16(offset, 0); // maxTwilightPoints
		maxpTable.WriteUint16(offset, 0); // maxStorage
		maxpTable.WriteUint16(offset, 0); // maxFunctionDefs
		maxpTable.WriteUint16(offset, 0); // maxInstructionDefs
		maxpTable.WriteUint16(offset, 0); // maxStackElements
		maxpTable.WriteUint16(offset, 0); // maxSizeOfInstructions
		maxpTable.WriteUint16(offset, 0); // maxComponentElements
		maxpTable.WriteUint16(offset, 0); // maxComponentDepth
	}
	static void WriteCmapTable(Table& cmapTable, const TTFFont& font) {
		// Create the glyph mapping vector
		std::vector<std::pair<wchar_t, size_t>> glyphMappings;
		for(const std::pair<const wchar_t, size_t>& mapping : font.glyphMapping)
			glyphMappings.push_back({ mapping.first, mapping.second });
		
		// Sort the glyph mapping vector and calculate the segments
		std::sort(glyphMappings.begin(), glyphMappings.end());
		glyphMappings.push_back({ (wchar_t)0xffff, 0 });

		std::vector<CmapSegment> segments;
		wchar_t startCode = glyphMappings[0].first;
		for(size_t i = 0; i != glyphMappings.size() - 1; ++i) {
			// Check if this is a mapping segment end
			if(i == glyphMappings.size() - 2 || glyphMappings[i].first != glyphMappings[i + 1].first - 1) {
				// Add the segment to the vector
				wchar_t endCode = glyphMappings[i].first;
				segments.push_back({ (uint16_t)endCode, (uint16_t)startCode, 0, 0 });

				// Set the next start code
				startCode = glyphMappings[i + 1].first;
			}
		}

		// Add the final segment
		segments.push_back({ 0xffff, 0xffff, 1, 0 });

		// Set the ID range offsets
		size_t startOffset = 0;
		for(size_t i = 0; i != segments.size() - 1; ++i) {
			segments[i].idRangeOffset = (uint16_t)((startOffset + segments.size() - i) << 1);
			startOffset += (size_t)(segments[i].endCode - segments[i].startCode + 1);
		}

		// Calculate all segment count values
		uint16_t segCountX2 = (uint16_t)(segments.size() << 1);
		uint16_t searchRange = CalcSearchRange((uint16_t)segments.size());
		uint16_t entrySelector = CalcEntrySelector((uint16_t)segments.size());
		uint16_t rangeShift = segCountX2 - searchRange;

		// Write the version and the number of tables
		uint32_t offset = 0;

		cmapTable.WriteUint16(offset, 0); // cmapVersion
		cmapTable.WriteUint16(offset, 1); // numTables

		// Write the Unicode table record
		cmapTable.WriteUint16(offset, 0); // platformID
		cmapTable.WriteUint16(offset, 3); // encodingID
		cmapTable.WriteUint32(offset, offset + sizeof(uint32_t)); // subtableOffset

		// Write the Unicode table's header
		cmapTable.WriteUint16(offset, 4); // format
		cmapTable.WriteUint16(offset, sizeof(uint16_t) * (uint16_t)(8 + segments.size() * 4 + glyphMappings.size() - 1)); // length
		cmapTable.WriteUint16(offset, 0); // language
		cmapTable.WriteUint16(offset, segCountX2); // segCountX2
		cmapTable.WriteUint16(offset, searchRange); // searchRange
		cmapTable.WriteUint16(offset, entrySelector); // entrySelector
		cmapTable.WriteUint16(offset, rangeShift); // rangeShift

		// Write the segment descriptors
		for(size_t i = 0; i != segments.size(); ++i)
			cmapTable.WriteUint16(offset, segments[i].endCode);

		cmapTable.WriteUint16(offset, 0); // reservedPad

		for(size_t i = 0; i != segments.size(); ++i)
			cmapTable.WriteUint16(offset, segments[i].startCode);
		for(size_t i = 0; i != segments.size(); ++i)
			cmapTable.WriteUint16(offset, segments[i].idDelta);
		for(size_t i = 0; i != segments.size(); ++i)
			cmapTable.WriteUint16(offset, segments[i].idRangeOffset);

		// Write the glyph ID array
		for(size_t i = 0; i != glyphMappings.size() - 1; ++i)
			cmapTable.WriteUint16(offset, (uint16_t)glyphMappings[i].second);
	}
	static void WriteGlyphs(Table& locaTable, Table& glyfTable, const TTFFont& font) {
		// Write all glyphs
		uint32_t offset = 0, locaOffset = 0;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Write the current glyph's offset
			locaTable.WriteUint32(locaOffset, offset);

			// Skip the current glyph if it has no contours
			if(font.glyphs[i].contours.empty())
				continue;

			// Write the glyph's header data
			const TTFFont::Glyph& glyph = font.glyphs[i];

			int16_t xMin = (int16_t)std::roundf(glyph.minCoords.x * WRITE_UNITS_PER_EM);
			int16_t yMin = (int16_t)std::roundf(glyph.minCoords.y * WRITE_UNITS_PER_EM);
			int16_t xMax = (int16_t)std::roundf(glyph.maxCoords.x * WRITE_UNITS_PER_EM);
			int16_t yMax = (int16_t)std::roundf(glyph.maxCoords.y * WRITE_UNITS_PER_EM);

			glyfTable.WriteUint16(offset, (int16_t)glyph.contours.size()); // numberOfContours
			glyfTable.WriteUint16(offset, xMin); // xMin
			glyfTable.WriteUint16(offset, yMin); // yMin
			glyfTable.WriteUint16(offset, xMax); // xMax
			glyfTable.WriteUint16(offset, yMax); // yMax

			// Write the contour end points
			uint16_t totalPointCount = 0;

			for(size_t j = 0; j != glyph.contours.size(); ++j) {
				totalPointCount += (uint16_t)glyph.contours[j].points.size();
				glyfTable.WriteUint16(offset, totalPointCount - 1);
			}

			glyfTable.WriteUint16(offset, 0); // instructionLength

			// Write the point flags
			for(uint16_t j = 0; j != totalPointCount; j += 2) {
				glyfTable.data.push_back(ON_CURVE_POINT); ++offset;
				glyfTable.data.push_back(0); ++offset;
			}

			// Write the X coordinates
			int32_t prevCoord = 0;
			for(size_t j = 0; j != glyph.contours.size(); ++j) {
				for(size_t k = 0; k != glyph.contours[j].points.size(); ++k) {
					// Calculate the current coordinate
					int32_t currCoord = (int32_t)std::roundf(glyph.contours[j].points[k].x * WRITE_UNITS_PER_EM);
					currCoord = std::clamp(currCoord, (int32_t)xMin, (int32_t)xMax);

					// Write the coordinate delta
					glyfTable.WriteUint16(offset, (uint16_t)(currCoord - prevCoord));
					prevCoord = currCoord;
				}
			}

			// Write the Y coordinates
			prevCoord = 0;
			for(size_t j = 0; j != glyph.contours.size(); ++j) {
				for(size_t k = 0; k != glyph.contours[j].points.size(); ++k) {
					// Calculate the current coordinate
					int32_t currCoord = (int32_t)std::roundf(glyph.contours[j].points[k].y * WRITE_UNITS_PER_EM);
					currCoord = std::clamp(currCoord, (int32_t)yMin, (int32_t)yMax);

					// Write the coordinate delta
					glyfTable.WriteUint16(offset, (uint16_t)(currCoord - prevCoord));
					prevCoord = currCoord;
				}
			}
		}

		// Write the end offset
		locaTable.WriteUint32(locaOffset, offset);
	}
	static void WriteHheaTable(Table& hheaTable, const TTFFont& font) {
		// Find the horizontal metric limits
		uint16_t advanceWidthMax = 0;
		int16_t minLeftSideBearing = INT16_T_MAX, minRightSideBearing = INT16_T_MAX;
		int16_t xMaxExtent = INT16_T_MIN;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			// Get the current font's horizontal metrics
			uint16_t advanceWidth = (uint16_t)std::roundf((font.glyphs[i].leftSideBearing + (font.glyphs[i].maxCoords.x - font.glyphs[i].minCoords.x) + font.glyphs[i].rightSideBearing) * WRITE_UNITS_PER_EM);
			int16_t leftSideBearing = (int16_t)std::roundf(font.glyphs[i].leftSideBearing * WRITE_UNITS_PER_EM);
			int16_t rightSideBearing = (int16_t)std::roundf(font.glyphs[i].rightSideBearing * WRITE_UNITS_PER_EM);
			int16_t xExtent = (int16_t)std::roundf((font.glyphs[i].leftSideBearing + (font.glyphs[i].maxCoords.x - font.glyphs[i].minCoords.y)) * WRITE_UNITS_PER_EM);

			// Update the global limits
			advanceWidthMax = std::max(advanceWidthMax, advanceWidth);
			minLeftSideBearing = std::min(minLeftSideBearing, leftSideBearing);
			minRightSideBearing = std::min(minRightSideBearing, rightSideBearing);
			xMaxExtent = std::max(xMaxExtent, xExtent);
		}

		// Write the hhea table data
		uint32_t offset = 0;

		hheaTable.WriteUint32(offset, TTF_VERSION_1_0); // hheaVersion
		hheaTable.WriteUint16(offset, (uint16_t)(int16_t)std::roundf(font.ascent * WRITE_UNITS_PER_EM)); // ascender
		hheaTable.WriteUint16(offset, (uint16_t)(int16_t)std::roundf(font.descent * WRITE_UNITS_PER_EM)); // descender
		hheaTable.WriteUint16(offset, (uint16_t)(int16_t)std::roundf(font.lineGap * WRITE_UNITS_PER_EM)); // lineGap
		hheaTable.WriteUint16(offset, advanceWidthMax); // advanceWidthMax
		hheaTable.WriteUint16(offset, (uint16_t)minLeftSideBearing); // minLeftSideBearing
		hheaTable.WriteUint16(offset, (uint16_t)minRightSideBearing); // minRightSideBearing
		hheaTable.WriteUint16(offset, (uint16_t)xMaxExtent); // xMaxExtent
		hheaTable.WriteUint16(offset, 1); // caretSlopeRise
		hheaTable.WriteUint16(offset, 0); // caretSlopeRun
		hheaTable.WriteUint16(offset, 0); // caretOffset
		hheaTable.WriteUint16(offset, 0); // (reserved)
		hheaTable.WriteUint16(offset, 0); // (reserved)
		hheaTable.WriteUint16(offset, 0); // (reserved)
		hheaTable.WriteUint16(offset, 0); // (reserved)
		hheaTable.WriteUint16(offset, 0); // metricDataFormat
		hheaTable.WriteUint16(offset, (uint16_t)font.glyphs.size()); // numberOfHMetrics
	}
	static void WriteHmtxTable(Table& hmtxTable, const TTFFont& font) {
		// Write a horizontal metric for every glyph
		uint32_t offset = 0;

		for(size_t i = 0; i != font.glyphs.size(); ++i) {
			uint16_t advanceWidth = (uint16_t)std::roundf((font.glyphs[i].leftSideBearing + (font.glyphs[i].maxCoords.x - font.glyphs[i].minCoords.x) + font.glyphs[i].rightSideBearing) * WRITE_UNITS_PER_EM);
			int16_t leftSideBearing = (int16_t)std::roundf(font.glyphs[i].leftSideBearing * WRITE_UNITS_PER_EM);

			hmtxTable.WriteUint16(offset, advanceWidth);
			hmtxTable.WriteUint16(offset, (uint16_t)leftSideBearing);
		}
	}
	static void WriteNameTable(Table& nameTable, const TTFFont& font) {
		// Build the PostScript name
		std::wstring postscriptName;
		for(size_t i = 0; i != font.fontName.size() && postscriptName.size() != MAX_POSTSCRIPT_NAME_SIZE; ++i) {
			if(std::iswalnum(font.fontName[i]))
				postscriptName.push_back(font.fontName[i]);
		}

		// Write the name table header
		uint32_t offset = 0;

		nameTable.WriteUint16(offset, 0); // nameVersion
		nameTable.WriteUint16(offset, 6); // count
		nameTable.WriteUint16(offset, 0); // storageOffset

		// Write all name entries
		const std::wstring* nameStrings[] { &font.fontName, &font.fontStyle, &font.fontName, &font.fontName, &font.fontVersion, &postscriptName };
		uint32_t nameIDs[] { 1, 2, 3, 4, 5, 6 };
		uint32_t nameOffsets[6];

		for(uint32_t i = 0; i != 6; ++i) {
			nameTable.WriteUint16(offset, 0); // platformID
			nameTable.WriteUint16(offset, 3); // encodingID
			nameTable.WriteUint16(offset, 0); // languageID
			nameTable.WriteUint16(offset, nameIDs[i]); // nameID
			nameTable.WriteUint16(offset, (uint16_t)(nameStrings[i]->size() << 1)); // length
			nameTable.WriteUint16(offset, 0); // stringOffset
			nameOffsets[i] = offset - sizeof(uint16_t);
		}

		// Write the storage offset
		uint32_t storageOffsetOffset = sizeof(uint16_t) * 2;
		uint32_t storageOffset = offset;
		nameTable.WriteUint16(storageOffsetOffset, storageOffset);

		// Write all name strings
		for(uint32_t i = 0; i != 6; ++i) {
			nameTable.WriteUint16(nameOffsets[i], offset - storageOffset);
			for(size_t j = 0; j != nameStrings[i]->size(); ++j)
				nameTable.WriteUint16(offset, (uint16_t)nameStrings[i]->at(j));
		}
	}
	static void WriteOS2Table(Table& os2Table, const TTFFont& font) {
		// Calculate the average glyph width
		float avgWidth = 0.0f;
		uint32_t nonZeroCount = 0;

		for(uint32_t i = 0; i != font.glyphs.size(); ++i) {
			if(!font.glyphs[i].contours.empty()) {
				avgWidth += font.glyphs[i].maxCoords.x - font.glyphs[i].minCoords.x;
				++nonZeroCount;
			}
		}

		if(nonZeroCount)
			avgWidth /= nonZeroCount;

		// Write the OS/2 table's data
		uint32_t offset = 0;

		os2Table.WriteUint16(offset, 0x0005); // version
		os2Table.WriteUint16(offset, (int16_t)std::roundf(avgWidth * WRITE_UNITS_PER_EM)); // xAvgCharWidth
		os2Table.WriteUint16(offset, 400); // usWeightClass
		os2Table.WriteUint16(offset, 5); // usWidthClass
		os2Table.WriteUint16(offset, 0); // fsType
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.subscriptSize.x * WRITE_UNITS_PER_EM)); // ySubscriptXSize
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.subscriptSize.y * WRITE_UNITS_PER_EM)); // ySubscriptYSize
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.subscriptOffset.x * WRITE_UNITS_PER_EM)); // ySubscriptXOffset
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.subscriptOffset.y * WRITE_UNITS_PER_EM)); // ySubscriptYOffset
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.superscriptSize.x * WRITE_UNITS_PER_EM)); // ySuperscriptXSize
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.superscriptSize.y * WRITE_UNITS_PER_EM)); // ySuperscriptYSize
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.superscriptOffset.x * WRITE_UNITS_PER_EM)); // ySuperscriptXOffset
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.superscriptOffset.y * WRITE_UNITS_PER_EM)); // ySuperscriptYOffset
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.strikeoutHeight * WRITE_UNITS_PER_EM)); // yStrikeoutSize
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.strikeoutPosition * WRITE_UNITS_PER_EM)); // yStrikeoutPosition
		os2Table.WriteUint16(offset, 0); // sFamilyClass

		for(uint32_t i = 0; i != 10; ++i) {
			os2Table.data.push_back(0); // panose[i]
			++offset;
		}

		os2Table.WriteUint32(offset, 3); // ulUnicodeRange1
		os2Table.WriteUint32(offset, 0); // ulUnicodeRange2
		os2Table.WriteUint32(offset, 0); // ulUnicodeRange3
		os2Table.WriteUint32(offset, 0); // ulUnicodeRange4
		os2Table.WriteUint32(offset, font.vendorID); // achVendID
		os2Table.WriteUint16(offset, 0); // fsSelection
		os2Table.WriteUint16(offset, 0x0000); // usFirstCharIndex
		os2Table.WriteUint16(offset, 0x00ff); // usLastCharIndex
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.ascent * WRITE_UNITS_PER_EM)); // sTypoAscender
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.descent * WRITE_UNITS_PER_EM)); // sTypoDescender
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.lineGap * WRITE_UNITS_PER_EM)); // sTypoLineGap
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.ascent * WRITE_UNITS_PER_EM)); // usWinAscent
		os2Table.WriteUint16(offset, (int16_t)std::roundf(font.descent * WRITE_UNITS_PER_EM)); // usWinDescent
		os2Table.WriteUint32(offset, 1); // ulCodePangeRange1
		os2Table.WriteUint32(offset, 0); // ulCodePageRange2

		size_t glyphX = font.glyphMapping.at(L'x');
		size_t glyphH = font.glyphMapping.at(L'H');

		os2Table.WriteUint16(offset, (int16_t)std::roundf((font.glyphs[glyphX].maxCoords.y - font.glyphs[glyphX].minCoords.x) * WRITE_UNITS_PER_EM)); // sxHeight
		os2Table.WriteUint16(offset, (int16_t)std::roundf((font.glyphs[glyphH].maxCoords.y - font.glyphs[glyphH].minCoords.x) * WRITE_UNITS_PER_EM)); // sCapHeight

		os2Table.WriteUint16(offset, 0); // usDefaultChar
		os2Table.WriteUint16(offset, ' '); // usBreakChar
		os2Table.WriteUint16(offset, 1); // usMaxContext
		os2Table.WriteUint16(offset, 0x0000); // usLowerOpticalPointSize
		os2Table.WriteUint16(offset, 0xffff); // usUpperOpticalPointSize
	}
	static void WritePostTable(Table& postTable, const TTFFont& font) {
		// Write the post table's data
		uint32_t offset = 0;

		postTable.WriteUint32(offset, 0x00030000); // postVersion
		postTable.WriteUint32(offset, 0); // italicAngle
		postTable.WriteUint16(offset, (int16_t)std::roundf(font.underlinePosition * WRITE_UNITS_PER_EM)); // underlinePosition
		postTable.WriteUint16(offset, (int16_t)std::roundf(font.underlineHeight * WRITE_UNITS_PER_EM)); // underlineThickness
		postTable.WriteUint32(offset, 0); // isFixedPitch
		postTable.WriteUint32(offset, 0); // minMemType42
		postTable.WriteUint32(offset, 0); // maxMemType42
		postTable.WriteUint32(offset, 0); // minMemType1
		postTable.WriteUint32(offset, 0); // maxMemType1
	}
	static void SetTableInfo(TableSet& tableSet, uint32_t& totalChecksum) {
		Table* tables[] { &tableSet.cmapTable, &tableSet.glyfTable, &tableSet.headTable, &tableSet.hheaTable, &tableSet.hmtxTable, &tableSet.locaTable, &tableSet.maxpTable, &tableSet.nameTable, &tableSet.os2Table, &tableSet.postTable };
		uint32_t tableTags[] { TABLE_TAG_CMAP, TABLE_TAG_GLYF, TABLE_TAG_HEAD, TABLE_TAG_HHEA, TABLE_TAG_HMTX, TABLE_TAG_LOCA, TABLE_TAG_MAXP, TABLE_TAG_NAME, TABLE_TAG_OS2, TABLE_TAG_POST };

		// Set all table info
		uint32_t offset = TTF_HEADER_SIZE + TABLE_RECORD_SIZE * 10;

		for(size_t i = 0; i != 10; ++i) {
			// Assign the table's checksum
			tables[i]->AssignChecksum();
			totalChecksum += tables[i]->checksum;

			// Set all other table info
			tables[i]->tag = tableTags[i];
			tables[i]->offset = offset;
			tables[i]->length = (uint32_t)tables[i]->data.size();

			// Update the total checksum
			totalChecksum += tables[i]->tag;
			totalChecksum += tables[i]->checksum;
			totalChecksum += tables[i]->offset;
			totalChecksum += tables[i]->length;

			// Update the file offset
			offset += tables[i]->length;
		}
	}
	static void CopyTableSetToStream(std::ostream& stream, TableSet& tableSet) {
		Table* tables[] { &tableSet.cmapTable, &tableSet.glyfTable, &tableSet.headTable, &tableSet.hheaTable, &tableSet.hmtxTable, &tableSet.locaTable, &tableSet.maxpTable, &tableSet.nameTable, &tableSet.os2Table, &tableSet.postTable };

		// Write all table records
		for(size_t i = 0; i != 10; ++i) {
			BinaryWriteUint32BE(stream, tables[i]->tag);
			BinaryWriteUint32BE(stream, tables[i]->checksum);
			BinaryWriteUint32BE(stream, tables[i]->offset);
			BinaryWriteUint32BE(stream, tables[i]->length);
		}

		// Write all table data
		for(size_t i = 0; i != 10; ++i)
			stream.write((const char*)tables[i]->data.data(), tables[i]->length);
	}

	// Public functions
	bool ReadTTFFile(std::istream& stream, TTFFont& font) {
		// Check the given sfnt version
		uint32_t sfntVersion = BinaryReadUint32BE(stream);
		if(sfntVersion != TTF_VERSION_1_0)
			return false;
		
		// Read all other table directory data
		uint16_t tableCount = BinaryReadUint16BE(stream);
		uint16_t searchRange = BinaryReadUint16BE(stream);
		uint16_t entrySelector = BinaryReadUint16BE(stream);
		uint16_t rangeShift = BinaryReadUint16BE(stream);

		// Read all relevant tables
		TableSet tableSet;
		if(!ReadTableSet(stream, tableCount, tableSet))
			return false;
		
		// Read the head table's data
		uint16_t unitsPerEm; int16_t indexToLocFormat;
		if(!ReadHeadTable(tableSet.headTable, unitsPerEm, indexToLocFormat))
			return false;

		// Read the maxp table's data
		if(!ReadMaxpTable(tableSet.maxpTable, font))
			return false;

		// Find the unicode table offset in the cmap table
		uint32_t unicodeOffset = FindCmapUnicodeTableOffset(tableSet.cmapTable);
		if(unicodeOffset == UINT32_T_MAX)
			return false;
		
		// Read the Unicode character mapping
		if(!ReadCmapUnicodeTable(tableSet.cmapTable, font, unicodeOffset))
			return false;
		
		// Read the glyph offsets
		std::vector<uint32_t> glyphOffsets;
		ReadLocaTable(tableSet.locaTable, font.glyphs.size(), indexToLocFormat, glyphOffsets);

		// Read all glyph descriptions
		float invUnitsPerEm = 1.0f / unitsPerEm;
		if(!ReadSimpleGlyphs(tableSet.glyfTable, glyphOffsets, invUnitsPerEm, font))
			return false;
		if(!ReadCompositeGlyphs(tableSet.glyfTable, glyphOffsets, invUnitsPerEm, font))
			return false;

		// Read the glyph horizontal metrics
		uint16_t numberOfHMetrics;
		if(!ReadHheaTable(tableSet.hheaTable, invUnitsPerEm, numberOfHMetrics, font))
			return false;
		if(!ReadHmtxTable(tableSet.hmtxTable, numberOfHMetrics, invUnitsPerEm, font))
			return false;
		
		// Read the font's name
		if(!ReadNameTable(tableSet.nameTable, font))
			return false;
		
		// Read all OS/2 and PostScript info
		if(!ReadOS2Table(tableSet.os2Table, invUnitsPerEm, font))
			return false;
		if(!ReadPostTable(tableSet.postTable, invUnitsPerEm, font))
			return false;

		return true;
	}
	void WriteTTFFile(std::ostream& stream, const TTFFont& font) {
		// Write the table directory data
		uint32_t totalChecksum = 0;

		BinaryWriteUint32BE(stream, TTF_VERSION_1_0); totalChecksum += TTF_VERSION_1_0; // sfntVersion
		BinaryWriteUint16BE(stream, 10); totalChecksum += 10 << 16; // numTables
		BinaryWriteUint16BE(stream, 128); totalChecksum += 128; // searchRange
		BinaryWriteUint16BE(stream, 3); totalChecksum += 3 << 16; // entrySelector
		BinaryWriteUint16BE(stream, 32); totalChecksum += 32; // rangeShift

		// Write all tables
		TableSet tableSet;

		WriteHeadTable(tableSet.headTable, font);
		WriteMaxpTable(tableSet.maxpTable, font);
		WriteCmapTable(tableSet.cmapTable, font);
		WriteGlyphs(tableSet.locaTable, tableSet.glyfTable, font);
		WriteHheaTable(tableSet.hheaTable, font);
		WriteHmtxTable(tableSet.hmtxTable, font);
		WriteNameTable(tableSet.nameTable, font);
		WriteOS2Table(tableSet.os2Table, font);
		WritePostTable(tableSet.postTable, font);

		// Set all table info and update the head table's checksum adjustment
		uint32_t checksumAdjustmentOffset = sizeof(uint32_t) * 2;
		SetTableInfo(tableSet, totalChecksum);
		tableSet.headTable.WriteUint32(checksumAdjustmentOffset, CHECKSUM_ADJUSTMENT_DEC - totalChecksum);

		// Copy all tables and their records to the stream
		CopyTableSetToStream(stream, tableSet);
	}
}
