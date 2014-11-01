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
 * \file PACC/SVG/Path.hpp
 * \brief Class definition for the SVG path primitive.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#ifndef PACC_SVG_Path_hpp_
#define PACC_SVG_Path_hpp_

#include "PACC/SVG/Primitives.hpp"
#include "PACC/SVG/Styles.hpp"
#include "PACC/SVG/Types.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		// forward declaration
		class PathData;
		
		/*! \brief Graphic primitive for paths.
		 *  \ingroup SVGprimit
		 *  \see PathData
		 */
		class Path : public Primitive {
		 public:
			explicit Path(const Style& inStyle = Style());
			Path(const PathData& inPath, const Style& inStyle = Style());
			
			Path operator+(const PathData& inCommands) const;
			Path& operator+=(const PathData& inCommands);
		};
		
		/*! \brief %Data defining a path.
		 *  \ingroup SVGpath
		 *
		 * This is the base class for path commands. It is not possible to create
		 * an instance of this class directly. Instead you should use one of the
		 * subclasses that provide real functionalities.
		 */
		class PathData : protected string {
		 public:
			PathData operator+(const PathData& inPath);
			
			protected:
			PathData(const string& inValue);
		};
		
		/*! \brief %Path command for starting a new subpath at given coordinate.
		 *  \ingroup SVGpath
		 */
		class MoveTo : public PathData {
		 public:
			MoveTo(double inX, double inY);
			MoveTo(const Point& inPoint);
			MoveTo(const RelPos& inPos);
		};
		
		/*! \brief %Path command for drawing a line.
		 *  \ingroup SVGpath
		 */
		class LineTo : public PathData {
		 public:
			LineTo(double inX, double inY);
			LineTo(const Point& inPoint);
			LineTo(const RelPos& inPos);
		};
		
		/*! \brief %Path command to close the current subpath.
		 *  \ingroup SVGpath
		 *
		 * This command close the subpath by drawing a straight line from the 
		 * current point to current subpath's initial point.
		 */
		class ClosePath : public PathData {
		 public:
			ClosePath();
		};
		
		/*! \brief %Path command to draw a cubic Bezier curve.
		 *  \ingroup SVGpath
		 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands">
		 *       %SVG elliptical arc specification</a>
		 */
		class CubicCurveTo : public PathData {
		 public:
			CubicCurveTo(const Point& inPoint, const Point& inCtlPoint);
			CubicCurveTo(const RelPos& inPos, const RelPos& inCtlPos);
			CubicCurveTo(const Point& inPoint, const Point& inCtlPoint1, 
						 const Point& inCtlPoint2);
			CubicCurveTo(const RelPos& inPos, const RelPos& inCtlPos1, 
						 const RelPos& inCtlPos2);
		};
		
		/*! \brief %Path command to draw a quadratic Bezier curve.
		 *  \ingroup SVGpath
		 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands">
		 *       %SVG quadratic curve specification</a>
		 */
		class QuadraticCurveTo : public PathData {
		 public:
			QuadraticCurveTo(const Point& inPoint);
			QuadraticCurveTo(const RelPos& inPos);
			QuadraticCurveTo(const Point& inPoint, const Point& inCtlPoint);
			QuadraticCurveTo(const RelPos& inPos, const RelPos& inCtlPos);
		};
		
		/*! \brief %Path command to draw an elliptical arc.
		 *  \ingroup SVGpath
		 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataEllipticalArcCommands">
		 *       %SVG elliptical arc specification</a>
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
			EllipticalArcTo(const Point& inPoint, const Size& inRadii, 
							double inRotation, bool inLargeArc, bool inSweep);
			EllipticalArcTo(const RelPos& inPos, const Size& inRadii, 
							double inRotation, bool inLargeArc, bool inSweep);
		};
		
		/*! \brief Short name for a cubic Bezier curve path command.
		 *  \ingroup SVGpath
		 */
		typedef CubicCurveTo CurveTo;
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif // PACC_SVG_Path_hpp_
