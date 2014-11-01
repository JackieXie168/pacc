/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2003 by Marc Parizeau
 *  http://manitou.gel.ulaval.ca/~parizeau/PACC
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
* \file PACC/SVG/Types.hpp
 * \brief Class definition for the basic SVG types.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#ifndef PACC_SVG_Types_hpp_
#define PACC_SVG_Types_hpp_

#include <cmath>
#include <vector>
#include <string>

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!
		 * \brief Color name keywords
		 * \ingroup SVGtype
		 * \see <a href="http://www.w3.org/TR/SVG/types.html#ColorKeywords">%SVG color
		 * keywords specification</a>
		 */
		enum ColorName 
		{ 
			eAliceBlue, eAntiqueWhite, eAqua, eAquaMarine, eAzure, eBeige,
			eBisque, eBlack, eBlacheDalmond, eBlue, eBlueViolet, eBrown,
			eBurlyWood, eCadetBlue, eChartreuse, eChocolate, eCoral,
			eCornFlowerBlue, eCornSilk, eCrimson, eCyan, eDarkBlue,
			eDarkCyan, eDarkGoldenRod, eDarkGray, eDarkGreen, eDarkGrey,
			eDarkKhaki,	eDarkMagenta, eDarkOliveGreen, eDarkOrange, 
			eDarkOrchid, eDarkRed, eDarkSalmon, eDarkSeaGreen, 
			eDarkSlateBlue, eDarkSlateGray, eDarkSlateGrey, eDarkTurquoise,
			eDarkViolet, eDeepPink, eDeepSkyBlue, eDimGray, eDimGrey,
			eDodgerBlue, eFireBrick, eFloralWhite, eForestGreen, eFuchsia,
			eGainsboro, eGostWhite, eGold, eGoldenRod, eGray, eGrey, 
			eGreen, eGreenYellow, eHoneyDew, eHotPink, eIndianRed, eIndigo,
			eIvory, eKhaki, eLavender, eLavenderBlush, eLawnGreen,
			eLemonChiffon, eLightBlue, eLightCoral, eLightCyan, 
			eLightGolderRodYellow, eLightGray, eLightGreen, eLightGrey,
			eLigntPink, eLightSalmon, eLightSeaGreen, eLightSkyBlue, 
			eLightSlateGray, eLightSlateGrey, eLightSteelBlue, eLightYellow,
			eLime, eLimeGreen, eLinen, eMagenta, eMaroon, eMediumAquaMarine,
			eMediumBlue, eMediumOrchid, eMediumPurple, eMeidumSeaGreen,
			eMediumSlateBlue, eMediumSpringGreen, eMediumTurquoise,
			eMediumVioletRed, eModNightBlue, eMintCream, eMistyRose,
			eMoccasin, eNavajoWhite, eNavy, eNone, eOldLace, eOlive, 
			eOliveDrab, eOrange, eOrangered, eOrchid, ePaleGoldenRod, 
			ePaleGreen, ePaleTurquoise, ePaleVioletRed, ePapayaWhip, 
			ePeachPuff, ePeru, ePink, ePlum, ePowderBlue, ePurple, eRed, 
			eRosyBrown, eRoyalBlue, eSaddleBrown, eSalmon, eSandyBrown, 
			eSeaGreen, eSeaShell, eSienna, eSilver, eSkyBlue, eSlateBlue, 
			eSlateGray, eSlateGrey, eSnow, eSpringGreen, eSteelBlue, eTan, 
			eTeal, eThistle, eTomato, eTurquoise, eViolet, eWheat, eWhite, 
			eWhiteSmoke, eYellow, eYellowGreen
		};
		
		//! Enumeration of stroke line-cap types.
		//! ingroup SVGtype
		enum CapType {eButt, eRoundCap, eSquare};
		
		//! Enumeration of stroke line-join types.
		//! ingroup SVGtype
		enum JoinType {eMiter, eRoundJoin, eBevel};
		
		//! Enumeration of basic dash types.
		enum DashType {eContinuous, eDashed, eDotted};
		
		//! Enumeration of fill-rule types.
		//! ingroup SVGtype
		enum RuleType {eNonZero, eEvenOdd};
		
		//! Enumeration of text font style types.
		//! ingroup SVGtype
		enum FontType {eBold, eItalic, eOblique, eUnderline, eStrike};
		
		//! Enumeration of text anchor types.
		//! ingroup SVGtype
		enum AnchorType {eStart, eMiddle, eEnd};

		//! Enumeration of text baseline types.
		//! ingroup SVGtype
		enum BaselineType {eAlpha, eBottom, eCentral, eTop};
		
		//! \brief Basic %SVG color string.
		//! \ingroup SVGtype
		class Color : public std::string {
		 public:			
			Color(ColorName inName);
		
		protected:
			Color(const string& inColor);
		};
		
		//! \brief Red, green, and blue color.
		//! \ingroup SVGtype
		class RGBColor : public Color {
		 public:
			RGBColor(float inRed, float inGreen, float inBlue);
		};
		
		//! \brief %Color as a shade of gray.
		//! \ingroup SVGtype
		class Gray : public RGBColor {
		 public:
			Gray(float inLevel);
		};
		
		class PointList;
		
		//! \brief Position (x,y) of a graphic component.
		//! \ingroup SVGtype
		struct Point {
			//! Default constructor.
			Point(void) : x(0), y(0) {}
			
			//! Build with (\c inX,\c inY)..
			Point(double inX, double inY) : x(inX), y(inY) {}
			
			std::string getStringValue() const;
			
			PointList operator+(const Point& inPoint) const;
			PointList operator+(const PointList& inList) const;
			
			double x; //!< Horizontal coordinate.
			double y; //!< Vertical coordinate.
		};
		
		/*!\brief List of points.
		 * \ingroup SVGtype
		 * 
		 * This class is a standard vector of point (std::vector<Point>) with some
		 * utility functions and + and += operators to facilitate the concatenation
		 * of point lists.
		 */
		class PointList : public vector<Point> {
		 public:
			//! Make an empty point list.
			PointList() {}
			
			//! Make a point list out of a single point \c inPoint.
			PointList(const Point inPoint) : std::vector<Point>(1, inPoint) {}
			
			//! Return the result of cancatenating this list with point list \c inList.
			PointList operator+(const PointList& inList) const {return PointList(*this) += inList;}
			
			//! Append point list \c inList to this list.
			PointList& operator+=(const PointList& inList) {
				for ( unsigned i = 0; i < inList.size(); i++ ) push_back(inList[i]);
				return *this;
			}
			
			//! Return the result of cancatenating this list with single point \c inPoint.
			PointList operator+(const Point& inPoint) const {return PointList(*this) += inPoint;}
			
			//! Append single point  \c inPoint to this list.
			PointList& operator+=(const Point& inPoint) {
				push_back(inPoint);
				return *this;
			}
			
			string getStringValue(void) const;
		};
		
		//! \brief Relative position of a point.
		//! \ingroup SVGtype
		struct RelPos : public Point {
			//! Default constructor.
			RelPos(void) : Point(0,0) {}
			
			//! Build with (\c inX,\c inY)..
			RelPos(double inDx, double inDy) : Point(inDx, inDy) {}
		};
		
		//! \brief %Size of graphic component.
		//! \ingroup SVGtype
		class Size {
		 public:
			//! Default constructor.
			Size() : width(0), height(0) {}
			
			//! Construct with width \c inWidth and height \c inHeight.
			Size(double inWidth, double inHeight) : width(inWidth), height(inHeight) {}
			
			string getStringValue(void) const;
			
			double width; //!< Width of graphic component.
			double height; //!< Height of graphic component.
		};
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif //PACC_SVG_Types_hpp_
