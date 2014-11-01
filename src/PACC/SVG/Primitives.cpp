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
 * \file PACC/SVG/Primitives.cpp
 * \brief Class methods for the SVG graphic primitives.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

#include "PACC/SVG/Primitives.hpp"

using namespace std;
using namespace PACC;

/*!
 * \defgroup SVGprimit Graphics primitives
 * \ingroup SVG
 * \brief Graphics primitives for drawing shapes.
 */

/*!\brief  Build graphic element.
 * \param  inName A tag name for the element.
 * \param  inAttrList A list of attributes to give to this element.
 */
SVG::Primitive::Primitive(const string& inName, const XML::AttributeList& inAttrList)
: XML::Node(inName, inAttrList) 
{
}

/*!\brief  Add style attributes to element.
 * \param  inStyle  A style attribute list to add to this element.
 *
 * If the same style attribute with the same name is already present, it
 * will be replaced by the new value.
 */
void SVG::Primitive::addStyle(const Style& inStyle) 
{
	*this += inStyle;
}
			
/*! \brief Remove any transformation applied to this element.
 */
void SVG::Primitive::clearTransform(void) 
{
	removeAttribute("transform");
}
			
/*! \brief Set id of primitive.
 *  \param inID Id of primitive.
 */
void SVG::Primitive::setID(const string& inID) 
{
	setAttribute("id", inID);
}
			
/*!\brief Set the coordinate transformation to apply.
 * \param inTransform  Transformation to apply to this element.
 *
 * The specified transformation will replace any previous transformation.
 *
 * \see <a href="http://www.w3.org/TR/SVG/coords.html#TransformAttribute">
 *      %SVG transform specification</a>
 */
void SVG::Primitive::setTransform(const Transform& inTransform) 
{
	setAttribute("transform", inTransform);
}
			
/*! \brief Serialize this primitive into stream.
 *  \param outStream  Output stream.
 */
void SVG::Primitive::write(ostream& outStream) const 
{
	XML::Streamer lStream(outStream);
	serialize(lStream);
}

/*! \brief Make circle primitive.
 *  \param inCenter  Center point of circle.
 *  \param inRadius  Circle radius.
 *  \param inStyle   Specific style attributes.
 *  \see Style
 */
SVG::Circle::Circle(const Point& inCenter, double inRadius, const Style& inStyle) 
: Primitive("circle", inStyle) 
{
	setCenter(inCenter);
	setRadius(inRadius);
}

/*! \brief Set circle center point.
 *  \param inCenter  Center point of circle.
 */
void SVG::Circle::setCenter(const Point& inCenter) 
{
	setAttribute("cx", String::convert(inCenter.x));
	setAttribute("cy", String::convert(inCenter.y));
}

/*! \brief Set circle radius.
 *  \param inRadius Radius of circle.
 */
void SVG::Circle::setRadius(double inRadius) 
{
	setAttribute("r", String::convert(inRadius));
}			

/*! \brief Make ellipse.
 *  \param inCenter Center point of ellipse.
 *  \param inSize   Width and height of ellipse.
 *  \param inStyle  Specific style attributes.
 */
SVG::Ellipse::Ellipse(const Point& inCenter, const Size& inSize, const Style& inStyle)
: SVG::Primitive("ellipse", inStyle) 
{
	setCenter(inCenter);
	setSize(inSize);
}

/*! \brief Set ellipse center.
 *  \param inCenter Center point of ellipse.
 */
void SVG::Ellipse::setCenter(const Point& inCenter) 
{
	setAttribute("cx", String::convert(inCenter.x));
	setAttribute("cy", String::convert(inCenter.y));
}

/*! \brief Set ellipse size.
 *  \param inSize Size of ellipse.
 */
void SVG::Ellipse::setSize(const Size& inSize) 
{
	setAttribute("rx", String::convert(inSize.width/2));
	setAttribute("ry", String::convert(inSize.height/2));
}
		
/*! Make line segment.
 *  \param inStart Starting point of line.
 *  \param inEnd   Ending point off line.
 *  \param inStyle Specific style attributes.
 */
SVG::Line::Line(const Point& inStart, const Point& inEnd, const Style& inStyle)
: SVG::Primitive("line", inStyle) 
{
	setStart(inStart);
	setEnd(inEnd);
}

/*! \brief Set starting point of line segment.
 *  \param inStart Start point of line segment.
 */
void SVG::Line::setStart(const Point& inStart) 
{
	setAttribute("x1", String::convert(inStart.x));
	setAttribute("y1", String::convert(inStart.y));
}

/*! \brief Set ending point of line segment.
 *  \param inEnd End point of line segment.
 */
void SVG::Line::setEnd(const Point& inEnd) 
{
	setAttribute("x2", String::convert(inEnd.x));
	setAttribute("y2", String::convert(inEnd.y));
}			
		
/*! \brief Make an empty polygon.
 *  \param inStyle Specific style attributes.
 */
SVG::Polygon::Polygon(const Style& inStyle) 
: Primitive("polygon", inStyle) 
{
}

/*!\brief Make a polygon using a point list.
 *  \param inPointList Point list of polygon.
 *  \param inStyle     Specific style attributes.
 * 
 * This constructor gives a polygon with predefined points. You can
 * always add more points using the += operator.
 */
SVG::Polygon::Polygon(const PointList& inPointList, const Style& inStyle) 
: Primitive("polygon", inStyle) 
{
	setAttribute("points", inPointList.getStringValue());
}

/*! \brief Make new polygon from this polygon concatenated with point list \c inLinePath.
 */
const SVG::Polygon SVG::Polygon::operator+(const PointList& inLinePath) const 
{
	return Polygon(*this) += inLinePath;
}

/*! \brief Append point list \c inLinePath to this polygon.
 */
SVG::Polygon& SVG::Polygon::operator+=(const PointList& inLinePath) 
{
	(*this)["points"] += inLinePath.getStringValue();
	return *this;
}

/*! \brief Make new polygon from this polygon concatenated with point \c inPoint.
 */
const SVG::Polygon SVG::Polygon::operator+(const Point& inPoint) const 
{
	return Polygon(*this) += inPoint;
}

/*! \brief Append point \c inPoint to this polygon.
 */
SVG::Polygon& SVG::Polygon::operator+=(const Point& inPoint) 
{
	(*this)["points"] += string(" ") + inPoint.getStringValue();
	return *this;
}			

/*! \brief Make an empty polyline with style \c inStyle.
 */
SVG::Polyline::Polyline(const Style& inStyle)
: Primitive("polyline", inStyle) {}

/*! \brief Make a polyline from point list \c inLinePath using style \c inStyle.
 */
SVG::Polyline::Polyline(const PointList& inLinePath, const Style& inStyle)
: Primitive("polyline", inStyle) {
	setAttribute("points", inLinePath.getStringValue());
}

/*! \brief Make new polyline by concatenating this polyline with point list \c inLinePath.
 */
const SVG::Polyline SVG::Polyline::operator+(const PointList& inLinePath) const
{
	return Polyline(*this) += inLinePath;
}

/*! \brief Append point list \c inLinePath to this polyline.
 */
SVG::Polyline& SVG::Polyline::operator+=(const PointList& inLinePath) 
{
	(*this)["points"] += inLinePath.getStringValue();
	return *this;
}

/*! \brief Make new polyline by concatenating this polyline with point \c inPoint.
 */
const SVG::Polyline SVG::Polyline::operator+(const Point& inPoint) const 
{
	return Polyline(*this) += inPoint;
}

/*! \brief Append point to Polyline.
 *  \param inPoint %Point to append.
 */
SVG::Polyline& SVG::Polyline::operator+=(const Point& inPoint) 
{
	(*this)["points"] += string(" ") + inPoint.getStringValue();
	return *this;
}			

/*! \brief Make rectangle.
 *  \param inOrigin Origin point of rectangle.
 *  \param inSize   Width and height of rectangle.
 *  \param inStyle  Specific style attributes.
 */
SVG::Rectangle::Rectangle(const Point& inOrigin, const Size& inSize, const Style& inStyle) 
: Primitive("rect", inStyle) 
{ 
	setOrigin(inOrigin);
	setSize(inSize);
}

/*! \brief Set corner radii of rectangle.
 *  \param inRx Horizontal corner radius.
 *  \param inRy Vertical corner radius.
 *
 * Default values are nul (sharp corners) and maximal values are half of the 
 * rectangle size (width and height).
 */
void SVG::Rectangle::setCornerRadii(double inRx, double inRy) 
{
	setAttribute("rx", String::convert(inRx));
	setAttribute("ry", String::convert(inRy));
}

/*! \brief Set rectangle origin.
 *  \param inOrigin Lower left point of rectangle.
 */
void SVG::Rectangle::setOrigin(const Point& inOrigin) 
{
	setAttribute("x", String::convert(inOrigin.x));
	setAttribute("y", String::convert(inOrigin.y));
}

/*! \brief Set rectangle size.
 *  \param inSize Size (width and height) of rectangle.
 */
void SVG::Rectangle::setSize(const Size& inSize) {
	setAttribute("width", String::convert(inSize.width));
	setAttribute("height", String::convert(inSize.height));
}

/*! \brief Make text.
 *  \param inString Character string of text.
 *  \param inPoint  Anchor point of text.
 *  \param inStyle  Specific style attributes.
 */
SVG::Text::Text(const string& inString, const Point& inPoint, const Style&  inStyle)
: Primitive("g", inStyle), mText(insertAsLastChild(new Node("text", XML::eData)))
{
	setAnchor(inPoint);
	mText->setAttribute("transform", Scale(1,-1)+Translate(0,-2*inPoint.y));
	mText->insertAsLastChild(new Node(inString, XML::eString));
}

SVG::Text::Span::Span(const string& inString, const Style& inStyle)
: XML::Node("tspan", inStyle) 
{
	insertAsLastChild(new Node(inString, XML::eString));
}

SVG::Text::Span::Span(const string& inString, const RelPos& inPos, const Style& inStyle)
: XML::Node("tspan", inStyle) 
{
	setAttribute("dx", String::convert(inPos.x));
	setAttribute("dy", String::convert(inPos.y));
	insertAsLastChild(new Node(inString, XML::eString));
}

/*! \brief Set text anchor point.
 *  \param inPoint Anchor point of text.
 */
void SVG::Text::setAnchor(const Point& inPoint) 
{
	mText->setAttribute("x", String::convert(inPoint.x));
	mText->setAttribute("y", String::convert(inPoint.y));
}

SVG::Text& SVG::Text::operator<<(const Span& inSpan)
{
	mText->insertAsLastChild(new Node(inSpan));
	return *this;
}

/*! \brief Insert primitive into output stream.
 *  \param outStream   Output stream.
 *  \param inPrimitive %Primitive to insert.
 *  \ingroup SVGprimit
 */
inline ostream& operator<<(ostream& outStream, const SVG::Primitive& inPrimitive) 
{
	inPrimitive.write(outStream);
	return outStream;
}
