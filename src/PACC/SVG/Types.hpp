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
 * $Revision: 1.5 $
 * $Date: 2005/11/29 22:31:51 $
 */

#ifndef PACC_SVG_Types_hpp_
#define PACC_SVG_Types_hpp_

#include <cmath>
#include <vector>
#include <string>

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		//! \brief Basic %SVG color string.
		//! \ingroup SVG
		class Color : public string {
		 public:
			/*!
			 * \brief  Constructor with color string.
			 * \param  inColorString  A string representing an" SVG color.
			 * 
			 * Here are the valid" SVG colors formats:
			 *
			 * <table>
			 * <tr><th> Format          </th><th> Description             </th></tr>
			 * <tr><td> #rgb            </td><td> Three-digit RGB notation</td></tr>
			 * <tr><td> #rrggbb         </td><td> Six-digit RGB notation  </td></tr>
			 * <tr><td> rgb(R, G, B)    </td>
			 *           <td> Functional notation (integer from 0 to 255) </td></tr>
			 * <tr><td> rgb(R%, G%, B%) </td>
			 *                         <td> Functional notation (percent) </td></tr>
			 * <tr><td> &lt;colorname>  </td><td> Keyword                 </td></tr>
			 * </table>
			 *
			 * If you want to use a color name keyword, you should use constants 
			 * defined in this class instead. If you need to use direct numerical 
			 * values, you should use the RGBColor or Gray subclass to construct
			 * the appropriate string.
			 *
			 * \note   No string format validation is performed to ensure that it
			 *         conform to SVG specifications. Make sure of what you write.
			 */
			explicit Color(const string &inColorString) : string(inColorString) {}
			
			/*!
			 * \name  Color Constants Keywords
			 * 
			 * Borowed from 
			 * <a href="http://www.w3.org/TR/SVG/types.html#ColorKeywords">SVG color
			 * keywords specifications</a>. Theses predefined colors should make 
			 * your life easier.
			 */
			//@{
			static const Color cAliceBlue;      //!< "aliceblue" SVG color
			static const Color cAntiqueWhite;   //!< "antiquewhite" SVG color
			static const Color cAqua;           //!< "aqua" SVG color
			static const Color cAquaMarine;     //!< "aquamarine" SVG color
			static const Color cAzure;          //!< "azure" SVG color
			static const Color cBeige;          //!< "beige" SVG color
			static const Color cBisque;         //!< "bisque" SVG color
			static const Color cBlack;          //!< "black" SVG color
			static const Color cBlacheDalmond;  //!< "blanchedalmond" SVG color
			static const Color cBlue;           //!< "blue" SVG color
			static const Color cBlueViolet;     //!< "blueviolet" SVG color
			static const Color cBrown;          //!< "brown" SVG color
			static const Color cBurlyWood;      //!< "burlywood" SVG color
			static const Color cCadetBlue;      //!< "cadetblue" SVG color
			static const Color cChartreuse;     //!< "chartreuse" SVG color
			static const Color cChocolate;      //!< "chocolate" SVG color
			static const Color cCoral;          //!< "coral" SVG color
			static const Color cCornFlowerBlue; //!< "cornflowerblue" SVG color
			static const Color cCornSilk;       //!< "cornsilk" SVG color
			static const Color cCrimson;        //!< "crimson" SVG color
			static const Color cCyan;           //!< "cyan" SVG color
			static const Color cDarkBlue;       //!< "darkblue" SVG color
			static const Color cDarkCyan;       //!< "darkcyan" SVG color
			static const Color cDarkGoldenRod;  //!< "darkgoldenrod" SVG color
			static const Color cDarkGray;       //!< "darkgray" SVG color
			static const Color cDarkGreen;      //!< "darkgreen" SVG color
			static const Color cDarkGrey;       //!< "darkgrey" SVG color
			static const Color cDarkKhaki;      //!< "darkkhaki" SVG color
			static const Color cDarkMagenta;    //!< "darkmagenta" SVG color
			static const Color cDarkOliveGreen; //!< "darkolivegreen" SVG color
			static const Color cDarkOrange;     //!< "darkorange" SVG color
			static const Color cDarkOrchid;     //!< "darkorchid" SVG color
			static const Color cDarkRed;        //!< "darkred" SVG color
			static const Color cDarkSalmon;     //!< "darksalmon" SVG color
			static const Color cDarkSeaGreen;   //!< "darkseagreen" SVG color
			static const Color cDarkSlateBlue;  //!< "darkslateblue" SVG color
			static const Color cDarkSlateGray;  //!< "darkslategray" SVG color
			static const Color cDarkSlateGrey;  //!< "darkslategrey" SVG color
			static const Color cDarkTurquoise;  //!< "darkturquoise" SVG color
			static const Color cDarkViolet;     //!< "darkviolet" SVG color
			static const Color cDeepPink;       //!< "deeppink" SVG color
			static const Color cDeepSkyBlue;    //!< "deepskyblue" SVG color
			static const Color cDimGray;        //!< "dimgray" SVG color
			static const Color cDimGrey;        //!< "dimgrey" SVG color
			static const Color cDodgerBlue;     //!< "dodgerblue" SVG color
			static const Color cFireBrick;      //!< "firebrick" SVG color
			static const Color cFloralWhite;    //!< "floralwhite" SVG color
			static const Color cForestGreen;    //!< "forestgreen" SVG color
			static const Color cFuchsia;        //!< "fuchsia" SVG color
			static const Color cGainsboro;      //!< "gainsboro" SVG color
			static const Color cGostWhite;      //!< "ghostwhite" SVG color
			static const Color cGold;           //!< "gold" SVG color
			static const Color cGoldenRod;      //!< "goldenrod" SVG color
			static const Color cGray;           //!< "gray" SVG color
			static const Color cGrey;           //!< "grey" SVG color
			static const Color cGreen;          //!< "green" SVG color
			static const Color cGreenYellow;    //!< "greenyellow" SVG color
			static const Color cHoneyDew;       //!< "honeydew" SVG color
			static const Color cHotPink;        //!< "hotpink" SVG color
			static const Color cIndianRed;      //!< "indianred" SVG color
			static const Color cIndigo;         //!< "indigo" SVG color
			static const Color cIvory;          //!< "ivory" SVG color
			static const Color cKhaki;          //!< "khaki" SVG color
			static const Color cLavender;       //!< "lavender" SVG color
			static const Color cLavenderBlush;  //!< "lavenderblush" SVG color
			static const Color cLawnGreen;      //!< "lawngreen" SVG color
			static const Color cLemonChiffon;   //!< "lemonchiffon" SVG color
			static const Color cLightBlue;      //!< "lightblue" SVG color
			static const Color cLightCoral;     //!< "lightcoral" SVG color
			static const Color cLightCyan;      //!< "lightcyan" SVG color
			static const Color cLightGolderRodYellow; //!< "lightgoldenrodyellow" SVG color
			static const Color cLightGray;      //!< "lightgray" SVG color
			static const Color cLightGreen;     //!< "lightgreen" SVG color
			static const Color cLightGrey;      //!< "lightgrey" SVG color
			static const Color cLigntPink;      //!< "lightpink" SVG color
			static const Color cLightSalmon;    //!< "lightsalmon" SVG color
			static const Color cLightSeaGreen;  //!< "lightseagreen" SVG color
			static const Color cLightSkyBlue;   //!< "lightskyblue" SVG color
			static const Color cLightSlateGray; //!< "lightslategray" SVG color
			static const Color cLightSlateGrey; //!< "lightslategrey" SVG color
			static const Color cLightSteelBlue; //!< "lightsteelblue" SVG color
			static const Color cLightYellow;    //!< "lightyellow" SVG color
			static const Color cLime;           //!< "lime" SVG color
			static const Color cLimeGreen;      //!< "limegreen" SVG color
			static const Color cLinen;          //!< "linen" SVG color
			static const Color cMagenta;        //!< "magenta" SVG color
			static const Color cMaroon;         //!< "maroon" SVG color
			static const Color cMediumAquaMarine; //!< "mediumaquamarine" SVG color
			static const Color cMediumBlue;       //!< "mediumblue" SVG color
			static const Color cMediumOrchid;     //!< "mediumorchid" SVG color
			static const Color cMediumPurple;     //!< "mediumpurple" SVG color
			static const Color cMeidumSeaGreen;   //!< "mediumseagreen" SVG color
			static const Color cMediumSlateBlue;  //!< "mediumslateblue" SVG color
			static const Color cMediumSpringGreen;//!< "mediumspringgreen" SVG color
			static const Color cMediumTurquoise;  //!< "mediumturquoise" SVG color
			static const Color cMediumVioletRed;  //!< "mediumvioletred" SVG color
			static const Color cModNightBlue;   //!< "midnightblue" SVG color
			static const Color cMintCream;      //!< "mintcream" SVG color
			static const Color cMistyRose;      //!< "mistyrose" SVG color
			static const Color cMoccasin;       //!< "moccasin" SVG color
			static const Color cNavajoWhite;    //!< "navajowhite" SVG color
			static const Color cNavy;           //!< "navy" SVG color
			static const Color cOldLace;        //!< "oldlace" SVG color
			static const Color cOlive;          //!< "olive" SVG color
			static const Color cOliveDrab;      //!< "olivedrab" SVG color
			static const Color cOrange;         //!< "orange" SVG color
			static const Color cOrangered;      //!< "orangered" SVG color
			static const Color cOrchid;         //!< "orchid" SVG color
			static const Color cPaleGoldenRod;  //!< "palegoldenrod" SVG color
			static const Color cPaleGreen;      //!< "palegreen" SVG color
			static const Color cPaleTurquoise;  //!< "paleturquoise" SVG color
			static const Color cPaleVioletRed;  //!< "palevioletred" SVG color
			static const Color cPapayaWhip;     //!< "papayawhip" SVG color
			static const Color cPeachPuff;      //!< "peachpuff" SVG color
			static const Color cPeru;           //!< "peru" SVG color
			static const Color cPink;           //!< "pink" SVG color
			static const Color cPlum;           //!< "plum" SVG color
			static const Color cPowderBlue;     //!< "powderblue" SVG color
			static const Color cPurple;         //!< "purple" SVG color
			static const Color cRed;            //!< "red" SVG color
			static const Color cRosyBrown;      //!< "rosybrown" SVG color
			static const Color cRoyalBlue;      //!< "royalblue" SVG color
			static const Color cSaddleBrown;    //!< "saddlebrown" SVG color
			static const Color cSalmon;         //!< "salmon" SVG color
			static const Color cSandyBrown;     //!< "sandybrown" SVG color
			static const Color cSeaGreen;       //!< "seagreen" SVG color
			static const Color cSeaShell;       //!< "seashell" SVG color
			static const Color cSienna;         //!< "sienna" SVG color
			static const Color cSilver;         //!< "silver" SVG color
			static const Color cSkyBlue;        //!< "skyblue" SVG color
			static const Color cSlateBlue;      //!< "slateblue" SVG color
			static const Color cSlateGray;      //!< "slategray" SVG color
			static const Color cSlateGrey;      //!< "slategrey" SVG color
			static const Color cSnow;           //!< "snow" SVG color
			static const Color cSpringGreen;    //!< "springgreen" SVG color
			static const Color cSteelBlue;      //!< "steelblue" SVG color
			static const Color cTan;            //!< "tan" SVG color
			static const Color cTeal;           //!< "teal" SVG color
			static const Color cThistle;        //!< "thistle" SVG color
			static const Color cTomato;         //!< "tomato" SVG color
			static const Color cTurquoise;      //!< "turquoise" SVG color
			static const Color cViolet;         //!< "violet" SVG color
			static const Color cWheat;          //!< "wheat" SVG color
			static const Color cWhite;          //!< "white" SVG color
			static const Color cWhiteSmoke;     //!< "whitesmoke" SVG color
			static const Color cYellow;         //!< "yellow" SVG color
			static const Color cYellowGreen;    //!< "yellowgreen" SVG color
																					//@}
		};
		
		//! \brief Red, green, and blue color.
		//! \ingroup SVG
		class RGBColor : public Color {
		 public:
			//! Construct with components \c inRed, \c inGreen, and \c inBlue.
			RGBColor(float inRed, float inGreen, float inBlue);
		};
		
		//! \brief %Color as a shade of gray.
		//! \ingroup SVG
		class Gray : public RGBColor {
		 public:
			//! Construct gray color of intensity \c inLevel.
			Gray(float inLevel) : RGBColor(inLevel, inLevel, inLevel) {}
		};
		
		class PointList;
		
		//! \brief Position of graphic component.
		//! \ingroup SVG
		class Point {
		 public:
			//! Default constructor.
			Point(void) : x(0), y(0) {}
			
			//! Construct with (\c inX,\c inY)..
			Point(double inX, double inY) : x(inX), y(inY) {}
			
			//! return the angle with the origin.
			double getAngle(void) const {return atan2(y,x);}
			
			//! Return the distance from the origin.
			double getDistance(void) const {return sqrt(x*x+y*y);}
			
			//! Rotate by \c inAngle degrees.
			void rotate(double inAngle) {*this = Point(x*cos(inAngle)-y*sin(inAngle), x*sin(inAngle)+y*cos(inAngle));}
			
			//! Translate by offset (\c inDX,\c inDY).
			void translateBy(double inDX, double inDY) {x += inDX; y += inDY;}
			
			//! Scale by factor \c inFactor
			void scaleBy(double inFactor) {x *= inFactor; y *= inFactor;}
			
			std::string getStringValue() const;
			
			//! Concatenate with point \c inPoint, and return new point list.
			PointList operator+(const Point &inPoint) const;
			
			//! Concatenate with point list \c inList and return new point list.
			PointList operator+(const PointList &inList) const;
			
			double x; //!< Horizontal coordinate.
			double y; //!< Vertical coordinate.
		};
		
		/*!\brief List of points.
		 * \ingroup SVG
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
			PointList operator+(const PointList &inList) const {return PointList(*this) += inList;}
			
			//! Append point list \c inList to this list.
			PointList &operator+=(const PointList &inList) {
				for ( unsigned i = 0; i < inList.size(); i++ ) push_back(inList[i]);
				return *this;
			}
			
			//! Return the result of cancatenating this list with single point \c inPoint.
			PointList operator+(const Point &inPoint) const {return PointList(*this) += inPoint;}
			
			//! Append single point  \c inPoint to this list.
			PointList &operator+=(const Point &inPoint) {
				push_back(inPoint);
				return *this;
			}
			
			//! Return a string representation of this list.
			string getStringValue(void) const;
		};
		
		//! \brief %Size of graphic component.
		//! \ingroup SVG
		class Size {
		 public:
			//! Default constructor.
			Size() : width(0), height(0) {}
			
			//! Construct with width \c inWidth and height \c inHeight.
			Size(double inWidth, double inHeight) : width(inWidth), height(inHeight) {}
			
			//! Return length of diagonal.
			double getDiagonal(void) const {return sqrt(width*width + height*height);}
			
			//! Scale by factor \c inFactor.
			void scaleBy(float inFactor) {width *= inFactor; height *= inFactor;}
			
			string getStringValue(void) const;
			
			double width; //!< Width of graphic component.
			double height; //!< Height of graphic component.
		};
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif //PACC_SVG_Types_hpp_
