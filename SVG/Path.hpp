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

/*!\file PACC/SVG/Path.hpp
 * \brief Class definition for the SVG path primitive.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.1 $
 * $Date: 2005/06/08 18:46:50 $
 */

#ifndef PACC_SVG_Path_hpp_
#define PACC_SVG_Path_hpp_

#include "SVG/Primitives.hpp"
#include "SVG/Styles.hpp"
#include "SVG/Types.hpp"

namespace PACC {
   
	namespace SVG {
		
		using namespace std;

		// forward declaration
		class PathData;

		/*!\brief Graphic primitive for paths.
		 * \ingroup SVG
		 *
		 * \see  pathcommands
		 */
		class Path : public Primitive {
       public:
        //! Make an empty path with style \c inStyle.
        explicit Path(const Style& inStyle = Style()) : Primitive("path", inStyle) {}
        /*!\brief Make a path command with data \c inPath and style \c inStyle.
         *
         * This constructor gives a already defined path. You can
         * always add more commands witht the += operator.
         */
			Path(const PathData& inPath, const Style& inStyle = Style()) : Primitive("path", inStyle) {
				setAttribute("d", (string&)inPath);
			}			

			//! Return the result of concatenating this path with path data \c inCommands.
			Path operator+(const PathData& inCommands) const {
				return Path(*this) += inCommands;
			}
			//! Append commands \c inCommands to this path.
			Path &operator+=(const PathData &inCommands) {
				string lDefinition = getAttribute("d");
				lDefinition += string(" ") + (string&)inCommands;
				setAttribute("d", lDefinition);
				return *this;
			}			
		};
		
		/*!\brief %Data defining a path.
		 * \ingroup SVG
		 *
		 * This is the base class for path commands. It is not possible to create
		 * an instance of this class directly. Instead you should use one of the
		 * subclasses that provide real functionalities.
		 */
 		class PathData : protected string {
       public:
			//! Return the result of concatenating this path data with \c inData.
			PathData operator+(const PathData &inData) {
				return PathData((string &)(*this) + " " + inData);
			}

       protected:
			//! Make data path from string \c inValue.
			PathData(const string &inValue) : string(inValue) {}
		};
		
		//! \brief %Path command for starting a new subpath at given coordinate.
		//! \ingroup SVG
		class MoveTo : public PathData {
       public:
			//! make moveto from point \c inPoint.
			MoveTo(const Point &inPoint) : PathData(string("M") + inPoint.getStringValue()) {}
			//! Make moveto from coordinates \c inX and \c inY.
			MoveTo(float inX, float inY) : PathData(string("M") + Point(inX, inY).getStringValue()) {}
		};
		
		//! \brief %Path command for drawing a line.
		//! \ingroup SVG
		class LineTo : public PathData {
       public:
			//! Make lineto with point \c inPoint. 
			LineTo(const Point &inPoint)  : PathData(string("L") + inPoint.getStringValue()) {}
			//! Make lineto with coordinates \c inX and \c inY,
			LineTo(float inX, float inY) : PathData(string("L") + Point(inX, inY).getStringValue()) {}
		};
		
		/*!\brief %Path command to close the current subpath.
		 * \ingroup SVG
		 *
		 * This command close the subpath by drawing a straight line from the 
		 * current point to current subpath's initial point.
		 */
		class ClosePath : public PathData {
       public:
			//! Default constructor.
			ClosePath() : PathData("z") {}
		};
		
		//! \brief %Path command to draw a cubic Bezier curve.
		//! \ingroup SVG
		class CubicCurveTo : public PathData {
       public:
			//! Make cubic curve to point \c inPoint, using control points \c inCtlPoint/ and \c inCtlPoint2;
			CubicCurveTo(const Point &inPoint, const Point &inCtlPoint1, const Point &inCtlPoint2) : PathData("C" + inCtlPoint1.getStringValue() + " " + inCtlPoint2.getStringValue() + " " + inPoint.getStringValue()) {}
			
			/*!\brief Make cubic curve to point \c inPoint, using a single control point \c inCtlPoint.
			 *
			 * The beginning control point is assumed to be the reflection of the 
			 * end control point on the previous command relative to the current 
			 * point. If previous command is not a CubicCurveTo, assume the end 
			 * control point is coincident with the current point.
			 */
			CubicCurveTo(const Point &inPoint, const Point &inCtlPoint) : PathData("S" + inCtlPoint.getStringValue() + " " +  inPoint.getStringValue()) {}
		};
		
		//! \brief %Path command to draw a quadratic Bezier curve.
		//! \ingroup SVG
		class QuadraticCurveTo : public PathData {
       public:
			//! Make quadratic curve to point \c inPoint, using control point \c inCtlPoint.
			QuadraticCurveTo(const Point &inPoint, const Point &inCtlPoint) : PathData("Q" + inCtlPoint.getStringValue() + " " + " " +  inPoint.getStringValue()) {}
			/*!\brief Make quadratic curve to point \c inPoint, without using any control point.
			 *
			 * The control point is assumed to be the reflection of the control 
			 * point on the previous command relative to the current point. If
			 * previous command is not a QuadraticCurveTo, assume the control point 
			 * is coincident with the current point.
			 */
			QuadraticCurveTo(const Point &inPoint) : PathData("S" +  inPoint.getStringValue()) {}
		};
		
		/*!\brief %Path command to draw an elliptical arc.
		 * \ingroup SVG
		 *
		 * Draws an elliptical arc from the current point to given coordinates. The 
		 * size and orientation of the ellipse are defined by x-radius and y-radius
		 * and an x-axis-rotation, which indicates how the ellipse as a whole is 
		 * rotated relative to the current coordinate system. The center of the 
		 * ellipse is calculated automatically to satisfy the constraints imposed 
		 * by the other parameters. Large-arc and sweep contribute to the automatic 
		 * calculations and help determine how the arc is drawn.
		 *
		 * For most situations, there are actually four different arcs (two 
		 * different ellipses, each with two different arc sweeps) that satisfy 
		 * these constraints. large-arc-flag and sweep-flag indicate which one of 
		 * the four arcs are drawn, as follows:
		 *
		 * <ul>
		 * <li> Of the four candidate arc sweeps, two will represent an arc sweep of 
		 *      greater than or equal to 180 degrees (the "large-arc"), and two will 
		 *      represent an arc sweep of less than or equal to 180 degrees (the 
       *      "small-arc"). If large-arc is true, then one of the two larger
		 *      arc sweeps will be chosen; otherwise, if large-arc is false, one 
		 *      of the smaller arc sweeps will be chosen,
		 *
		 * <li> If sweep is true, then the arc will be drawn in a 
		 *      "positive-angle" direction (i.e., the ellipse formula 
		 *      x=cx+rx*cos(theta) and y=cy+ry*sin(theta) is evaluated such that 
       *      theta starts at an angle corresponding to the current point and 
		 *      increases positively until the arc reaches (x,y)). A value of false 
		 *      causes the arc to be drawn in a "negative-angle" direction (i.e., 
       *      theta starts at an angle value corresponding to the current point 
       *      and decreases until the arc reaches (x,y)).
		 * </ul>
		 */
		class EllipticalArcTo : public PathData {
       public:
			/*!\brief  Constructor.
			 * \param  inPoint          A destination point for the curve.
			 * \param  inXRadius        An x radius of the ellipse.
			 * \param  inYRadius        An y radius of the ellipse.
			 * \param  inXAxisRotation  A angle of rotation for the ellipse.
			 * \param  inLargeArc       True for the path to follow the large arc, 
			 *                          false to follow the smaller one.
			 * \param  inSweep          True to turn by a positive angle, false
			 *                          for a negative one.
			 */
			EllipticalArcTo(const Point &inPoint, float inXRadius, float inYRadius, float inXAxisRotation, bool inLargeArc, bool inSweep) : PathData(string("A") + String::convert(inXRadius) + "," +  String::convert(inYRadius) + " " + String::convert(inXAxisRotation) + " " + ( inLargeArc ? "1" : "0" ) + " " + ( inSweep ? "1" : "0" ) + " " + inPoint.getStringValue()) {}
		};
		
		//! Short name for a cubic Bezier curve path command.
		typedef CubicCurveTo CurveTo;
		
	} // end of SVG namespace
	
} // end of PACC namespace
	
#endif // PACC_SVG_Path_hpp_
	