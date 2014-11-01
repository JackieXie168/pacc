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
 * \file PACC/SVG/Path.cpp
 * \brief Class methods for the SVG path primitive.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Path.hpp"

using namespace PACC;

/*!
 * \defgroup SVGpath Path elements
 * \ingroup SVGprimit
 * \brief Elements for specifying drawing paths.
 */

//!\brief Make an empty path with style \c inStyle.
SVG::Path::Path(const Style& inStyle) : Primitive("path", inStyle) 
{
}

/*!\brief Make a path command with data \c inPath and style \c inStyle.
 *
 * This constructor gives a already defined path. You can
 * always add more commands witht the += operator.
 */
SVG::Path::Path(const PathData& inPath, const Style& inStyle) 
: Primitive("path", inStyle) 
{
	setAttribute("d", (string&)inPath);
}			
			
//!\brief Return the result of concatenating this path with path data \c inCommands.
SVG::Path SVG::Path::operator+(const PathData& inCommands) const 
{
	return Path(*this) += inCommands;
}

//! Append commands \c inCommands to this path.
SVG::Path& SVG::Path::operator+=(const PathData& inCommands) 
{
	string lDefinition = getAttribute("d");
	lDefinition += string(" ") + (string&)inCommands;
	setAttribute("d", lDefinition);
	return *this;
}			

//! Return the result of concatenating this path data with \c inPath.
SVG::PathData SVG::PathData::operator+(const PathData& inPath) 
{
	return PathData((string&)(*this) + " " + inPath);
}

//! Make data path from string \c inValue (restricted usage).
SVG::PathData::PathData(const string& inValue) : string(inValue) 
{
}

/*! \brief Move current point using absolute coordinates.
 *  \param inX Horizontal coordinate of end point.
 *  \param inY Vertical coordinate of end point.
 */
SVG::MoveTo::MoveTo(double inX, double inY) 
: PathData(string("M") + Point(inX, inY).getStringValue()) 
{
}

/*! \brief Move current point using absolute coordinates.
 *  \param inPoint End point of move.
 */
SVG::MoveTo::MoveTo(const Point& inPoint) 
: PathData(string("M") + inPoint.getStringValue()) 
{
}

/*! \brief Move current point using relative coordinates.
 *  \param inPos End position of move.
 */
SVG::MoveTo::MoveTo(const RelPos& inPos) 
: PathData(string("m") + inPos.getStringValue()) 
{
}

/*! \brief Draw line using absolute coordinates.
 *  \param inX Horizontal coordinate of line end point.
 *  \param inY Vertical coordinate of line end point.
 */
SVG::LineTo::LineTo(double inX, double inY) 
: PathData(string("L") + Point(inX, inY).getStringValue()) 
{
}

/*! \brief Draw line using absolute coordinates.
 *  \param inPoint End point of line.
 */
SVG::LineTo::LineTo(const Point& inPoint) 
: PathData(string("L") + inPoint.getStringValue()) 
{
}

/*! \brief Draw line using relative coordinates.
 *  \param inPos End position of line.
 */
SVG::LineTo::LineTo(const RelPos& inPos) 
: PathData(string("l") + inPos.getStringValue()) 
{
}

/*! \brief Close path.
 */
SVG::ClosePath::ClosePath() : PathData("z") 
{
}


/*! \brief Draw cubic bezier curve with one control point, using absolute coordinates.
 *  \param inPoint      End point of curve.
 *  \param inCtlPoint  Control point.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands">
 *       %SVG quadratic curve specification</a>
 *
 * The beginning control point is assumed to be the reflection of the 
 * end control point on the previous command relative to the current 
 * point. If previous command is not a CubicCurveTo, assume the end 
 * control point is coincident with the current point.
 */
SVG::CubicCurveTo::CubicCurveTo(const Point& inPoint, const Point& inCtlPoint) 
: PathData("S" + inCtlPoint.getStringValue() + " " +  inPoint.getStringValue()) 
{
}

/*! \brief Draw cubic bezier curve with one control point, using relative coordinates.
 *  \param inPos      End position of curve.
 *  \param inCtlPos   Control position.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands">
 *       %SVG quadratic curve specification</a>
 *
 * The beginning control point is assumed to be the reflection of the 
 * end control point on the previous command relative to the current 
 * point. If previous command is not a CubicCurveTo, assume the end 
 * control point is coincident with the current point.
 */
SVG::CubicCurveTo::CubicCurveTo(const RelPos& inPos, const RelPos& inCtlPos) 
: PathData("s" + inCtlPos.getStringValue() + " " +  inPos.getStringValue()) 
{
}

/*! \brief Draw cubic bezier curve with two control points, using absolute coordinates.
 *  \param inPoint      End point of curve.
 *  \param inCtlPoint1  First control point.
 *  \param inCtlPoint2  Second control point.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands">
 *       %SVG quadratic curve specification</a>
 */
SVG::CubicCurveTo::CubicCurveTo(const Point& inPoint, const Point& inCtlPoint1, 
								const Point& inCtlPoint2) 
: PathData("C" + inCtlPoint1.getStringValue() + " " + inCtlPoint2.getStringValue() + 
		   " " + inPoint.getStringValue()) 
{
}

/*! \brief Draw cubic bezier curve with two control points, using relative coordinates.
 *  \param inPos      End position of curve.
 *  \param inCtlPos1  First control position.
 *  \param inCtlPos2  Second control position.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataCubicBezierCommands">
 *       %SVG quadratic curve specification</a>
 */
SVG::CubicCurveTo::CubicCurveTo(const RelPos& inPos, const RelPos& inCtlPos1, 
								const RelPos& inCtlPos2) 
: PathData("c" + inCtlPos1.getStringValue() + " " + inCtlPos2.getStringValue() + 
		   " " + inPos.getStringValue()) 
{
}

/*! \brief Draw quadratic curve using absolute coordinates.
 *  \param  inPoint End point of curve.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands">
 *       %SVG quadratic curve specification</a>
 *
 * The control point is assumed to be the reflection of the control 
 * point on the previous command relative to the current point. If
 * previous command is not a QuadraticCurveTo, assume the control point 
 * is coincident with the current point.
 */
SVG::QuadraticCurveTo::QuadraticCurveTo(const Point& inPoint) 
: PathData("S" +  inPoint.getStringValue()) 
{
}

/*! \brief Draw quadratic curve using relative coordinates.
 *  \param inPos End position of curve.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands">
 *       %SVG quadratic curve specification</a>
 *
 * The control point is assumed to be the reflection of the control 
 * point on the previous command relative to the current point. If
 * previous command is not a QuadraticCurveTo, assume the control point 
 * is coincident with the current point.
 */
SVG::QuadraticCurveTo::QuadraticCurveTo(const RelPos& inPos) 
: PathData("s" +  inPos.getStringValue()) 
{
}

/*! \brief Draw quadratic curve with control point, using absolute coordinates.
 *  \param inPoint    End point of curve.
 *  \param inCtlPoint Control point.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands">
 *       %SVG quadratic curve specification</a>
 */
SVG::QuadraticCurveTo::QuadraticCurveTo(const Point& inPoint, const Point& inCtlPoint)
: PathData("Q" + inCtlPoint.getStringValue() + " " + " " +  inPoint.getStringValue())
{
}

/*! \brief Draw quadratic curve with control point, using relative coordinates. 
 *  \param inPos    End position of curve.
 *  \param inCtlPos Control position.
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataQuadraticBezierCommands">
 *       %SVG quadratic curve specification</a>
 */
SVG::QuadraticCurveTo::QuadraticCurveTo(const RelPos& inPos, const RelPos& inCtlPos)
: PathData("q" + inCtlPos.getStringValue() + " " + " " +  inPos.getStringValue())
{
}

/*! \brief  Build elliptical arc using absolute coordinates.
 *  \param  inPoint          End point of arc.
 *  \param  inRadii          X and Y radii of the underlying ellipse.
 *  \param  inRotation       Rotation angle of the underlying ellipse.
 *  \param  inLargeArc       Whether the path follows the large arc (true), 
 *                           or the smaller one (false).
 *  \param  inSweep          Whether the path turns counter-clockwise (true), 
 *                          or clockwise (false).
 *  \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataEllipticalArcCommands">
 *       %SVG elliptical arc specification</a>
 */
SVG::EllipticalArcTo::EllipticalArcTo(const Point& inPoint, const Size& inRadii, 
									  double inRotation, bool inLargeArc, bool inSweep) 
: PathData(string("A") + inRadii.getStringValue() + " " + String::convert(inRotation) + 
		   " " + ( inLargeArc ? "1" : "0" ) + "," + ( inSweep ? "1" : "0" ) + " " + 
		   inPoint.getStringValue()) 
{
}

/*!\brief  Build elliptical arc using relative coordinates.
 * \param  inPos            End position of arc.
 * \param  inRadii          X and Y radii of the underlying ellipse.
 * \param  inRotation       Rotation angle of the underlying ellipse.
 * \param  inLargeArc       Whether the path follows the large arc (true), 
 *                          or the smaller one (false).
 * \param  inSweep          Whether the path turns counter-clockwise (true), 
 *                          or clockwise (false).
 * \see <a href="http://www.w3.org/TR/SVG/paths.html#PathDataEllipticalArcCommands">
 * %SVG elliptical arc specification</a>
 */
SVG::EllipticalArcTo::EllipticalArcTo(const RelPos& inPos, const Size& inRadii, 
									  double inRotation, bool inLargeArc, bool inSweep) 
: PathData(string("a") + inRadii.getStringValue() + " " + String::convert(inRotation) + 
		   " " + ( inLargeArc ? "1" : "0" ) + "," + ( inSweep ? "1" : "0" ) + " " + 
		   inPos.getStringValue()) 
{
}
