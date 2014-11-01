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
 * \file PACC/SVG/Primitives.hpp
 * \brief Class definitions for the SVG graphic primitives.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.8 $
 * $Date: 2005/10/05 12:33:52 $
 */

#ifndef PACC_SVG_Graphic_hpp_
#define PACC_SVG_Graphic_hpp_

#include "XML/Node.hpp"
#include "XML/Streamer.hpp"
#include "SVG/Styles.hpp"
#include "SVG/Transforms.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Base class for graphic elements.
		* \ingroup SVG
		*
		* This class provide no more functionality than it's base class. It is
		* intended to be able to distinguish graphics element from others. That way
		* we can restrict at compile time what kind of elements can be inserted in
		* a Group for example.
		*
		* This class must be subclassed to be useful. You may note that the only
		* constructor is protected to enforce that.
		*/
		class Primitive : protected XML::Node {
			public:
			/*!\brief  Add some style attributes to element.
			* \param  inStyle  A style attribute list to add to this element.
			*
			* If the same style attribute with the same name is already present, it
			* will be replaced by the new value.
			*/
			void addStyle(const Style& inStyle) {*this += inStyle;}
			
			//! Remove any coordinate transformation applyed to this element.
			void clearTransform(void) {removeAttribute("transform");}
			
			//! Set id of primitive.
			void setID(const string& inID) {setAttribute("id", inID);}
			
			/*!\brief  Set the coordinate transformation to apply.
			* \param  inTransform  A transformation to apply to this element.
			*
			* The current transformation, if any, will be replaced by the new one.
			*
			* \see transformcomp
			*/
			void setTransform(const Transform& inTransform) {setAttribute("transform", inTransform);}
			
			//! Write serialized primitive into stream \c outStream.
			void write(ostream& outStream) const {
				XML::Streamer lStream(outStream);
				serialize(lStream);
			}
			
			protected:
			/*!\brief  Constructor for a graphic element.
			* \param  inName A tag name for the element.
			* \param  inAttrList A list of attributes to give to this element.
			*/
			Primitive(const string &inName, const XML::AttributeList &inAttrList = XML::AttributeList()) : XML::Node(inName, inAttrList) {}
			
			friend class Group; //!< Allowing the use of Element's << operator.
			
		};
		
		/*!\brief Graphic primitive for rectangles.
			* \ingroup SVG
			*
			* A rectangle is a four-sided figure with two vertical sides and two
			* horizontal ones.
			*/
		class Rectangle : public Primitive {
			public:
			//! Make rectangle with origin \c inOrigin, size \c inSize, and style \c inStyle.
			Rectangle(const Point &inOrigin, const Size &inSize, const Style &inStyle = Style()) : Primitive("rect", inStyle) { 
				setAttribute("x", String::convert(inOrigin.x));
				setAttribute("y", String::convert(inOrigin.y));
				setAttribute("width", String::convert(inSize.width));
				setAttribute("height", String::convert(inSize.height));
			}
			
			//! Return rectangle origin. 
			Point getOrigin(void) const {
				return Point(String::convertToFloat(getAttribute("x")), String::convertToFloat(getAttribute("y")));
			}			
			//! Set rectangle origin to point \c inPoint.
			void setOrigin(const Point &inOrigin) {
				setAttribute("x", String::convert(inOrigin.x));
				setAttribute("y", String::convert(inOrigin.y));
			}			
			//! Set rectangle origin to coordinates \c inX and \c inY.
			void setOrigin(float inX, float inY) {
				setAttribute("x", String::convert(inX));
				setAttribute("y", String::convert(inY));
			}			
			
			//! Return rectangle size.
			Size getSize(void) const {
				return Size(String::convertToFloat(getAttribute("width")), String::convertToFloat(getAttribute("height")));
			}			
			//! Set rectangle size to \c inSize.
			void setSize(const Size &inSize) {
				setAttribute("width", String::convert(inSize.width));
				setAttribute("height", String::convert(inSize.height));
			}			
			//! Set rectangle size to width \c inWidth and height \c inHeight.
			void setSize(float inWidth, float inHeight) {
				setAttribute("width", String::convert(inWidth));
				setAttribute("height", String::convert(inHeight));
			}			
			
		};
		
		/*!\brief Graphic primitive for circles.
			* \ingroup SVG
			*
			* A circle is defined by a central point (called the center) and a radius.
			* The shape is formed of the closed curve where the distance between the
			* center and each point of the curve is equal to the radius.
			*/
		class Circle : public Primitive {
			public:
			//! make circle centered at point \c inPoint, with radius \c inRadius, and style \c inStyle.
			Circle(const Point &inCenter, float inRadius, const Style &inStyle = Style()) : Primitive("circle", inStyle) {
				setAttribute("cx", String::convert(inCenter.x));
				setAttribute("cy", String::convert(inCenter.y));
				setAttribute("r", String::convert(inRadius));
			}
			
			//! Return circle center.
			Point getCenter(void) const{
				return Point(String::convertToFloat(getAttribute("cx")), String::convertToFloat(getAttribute("cy")));
			}
			//! Set circle center to point \c inCenter.
			void setCenter(const Point &inCenter) {
				setAttribute("cx", String::convert(inCenter.x));
				setAttribute("cy", String::convert(inCenter.y));
			}
			//! Set circle center to coordinates \c inCX and \c inCY.
			void setCenter(float inCX, float inCY) {
				setAttribute("cx", String::convert(inCX));
				setAttribute("cy", String::convert(inCY));
			}
			
			//! Retur circle radius.
			float getRadius(void) const {
				return String::convertToFloat(getAttribute("r"));
			}			
			//! Set circle radius to \c inRadius.
			void setRadius(float inRadius) {
				setAttribute("r", String::convert(inRadius));
			}			
		};
		
		/*!\brief Graphic primitive for ellipses.
			* \ingroup SVG
			*
			* The ellipse is defined by a central point (the center) and two radius
			* values: one for the x axis, the other for the y axis.
			*/
		class Ellipse : public Primitive {
			public:
			//! Make ellipse centered at point \c inCenter, with radii \c inXRadius and \c inYRadius, and style \c inStyle.
			Ellipse(const Point &inCenter, float inXRadius, float inYRadius, const Style &inStyle = Style())  : SVG::Primitive("ellipse", inStyle) {
				setAttribute("cx", String::convert(inCenter.x));
				setAttribute("cy", String::convert(inCenter.y));
				setAttribute("rx", String::convert(inXRadius));
				setAttribute("ry", String::convert(inYRadius));
			}
			
			//! return ellipse center.
			Point getCenter() const {
				return Point(String::convertToFloat(getAttribute("cx")), String::convertToFloat(getAttribute("cy")));
			}			
			//! Set ellipse center to point \c inPoint.
			void setCenter(const Point &inCenter) {
				setAttribute("cx", String::convert(inCenter.x));
				setAttribute("cy", String::convert(inCenter.y));
			}
			//! Set ellipse center to coordinates \c inCX and \c inCY.
			void setCenter(float inCX, float inCY) {
				setAttribute("cx", String::convert(inCX));
				setAttribute("cy", String::convert(inCY));
			}
			
			//! Return ellipse first radius.
			float getXRadius() const {
				return String::convertToFloat(getAttribute("rx"));
			}
			//! Set ellipse first radius to \c inXRadius.
			void setXRadius(float inXRadius) {
				setAttribute("rx", String::convert(inXRadius));
			}
			
			//! Return ellipse second radius.
			float getYRadius(void) const {
				return String::convertToFloat(getAttribute("ry"));
			}
			//! Set ellipse second radius to \c inYRadius.
			void setYRadius(float inYRadius) {
				setAttribute("ry", String::convert(inYRadius));
			}
		};
		
		/*!\brief Graphic primitive for straight lines.
			* \ingroup SVG
			*
			* A line link a start point to an end point. Since a line has no fillable
			* region and by default shapes do not have a stroke, you must explicitly
		* define a stroke if you want the line to be visible.
			*
			* \see Stroke
			*/
		class Line : public Primitive {
			public:
			//! Make line segment from start point \c inStart to end point \c inEnd, using style \c inStyle.
			Line(const Point &inStart, const Point &inEnd, const Style &inStyle = Style()) : SVG::Primitive("line", inStyle) {
				setAttribute("x1", String::convert(inStart.x));
				setAttribute("y1", String::convert(inStart.y));
				setAttribute("x2", String::convert(inEnd.x));
				setAttribute("y2", String::convert(inEnd.y));
			}
			
			//! Return start point of line segment.
			Point getStart(void) const {
				return Point(String::convertToFloat(getAttribute("x1")), String::convertToFloat(getAttribute("y1")));
			}			
			//! Set start point of line segment to \c inStart.
			void setStart(const Point &inStart) {
				setAttribute("x1", String::convert(inStart.x));
				setAttribute("y1", String::convert(inStart.y));
			}			
			//! Set start point of line segment to coordinates \c inX and \c inY.
			void setStart(float inX, float inY) {
				setAttribute("x1", String::convert(inX));
				setAttribute("y1", String::convert(inY));
			}			
			
			//! Return end point of line segment.
			Point getEnd(void) const {
				return Point(String::convertToFloat(getAttribute("x2")), String::convertToFloat(getAttribute("y2")));
			}			
			//! Set end point of line segment to \c inEnd.
			void setEnd(const Point &inEnd) {
				setAttribute("x2", String::convert(inEnd.x));
				setAttribute("y2", String::convert(inEnd.y));
			}			
			//! set end point of line segment to coordinates \c inX and \c inY.
			void setEnd(float inX, float inY) {
				setAttribute("x2", String::convert(inX));
				setAttribute("y2", String::convert(inY));
			}			
		};
		
		/*!\brief Graphic primitive for polygons
			* \ingroup SVG
			*
			* A polygon is made of a serie of points linked together with strait lines.
			* A polygon is a closed path, so the last point is always linked with the 
			* first one.
			*
			* \see  Polyline
			*/
		class Polygon : public Primitive {
			public:
			//! Make an empty polygon with style \c inStyle.
			Polygon(const Style &inStyle = Style()) : Primitive("polygon", inStyle) {}
			/*!\brief Make a polygon from point list \c inPointList and style \c inStyle.
			* 
			* This constructor gives a polygon with predefined points. You can
			* always add more points witht the += operator.
			*/
			Polygon(const PointList &inLinePath, const Style &inStyle = Style()) : Primitive("polygon", inStyle) {
				setAttribute("points", inLinePath.getStringValue());
			}
			
			//! Make new polygon from this polygon concatenated with point list \c inLinePath.
			Polygon operator+(const PointList &inLinePath) const {
				return Polygon(*this) += inLinePath;
			}
			//! Append point list \c inLinePath to this polygon.
			Polygon &operator+=(const PointList &inLinePath) {
				(*this)["points"] += inLinePath.getStringValue();
				return *this;
			}
			//! Make new polygon from this polygon concatenated with point \c inPoint.
			Polygon operator+(const Point &inPoint) const {
				return Polygon(*this) += inPoint;
			}
			//! Append point \c inPoint to this polygon.
			Polygon &operator+=(const Point &inPoint) {
				(*this)["points"] += string(" ") + inPoint.getStringValue();
				return *this;
			}			
		};
		
		/*!\brief Graphic primitive for polyline
			* \ingroup SVG
			*
			* A polyline is made of a serie of points linked together with strait 
			* lines. Polyline is not closed: so the last point is not linked with the 
			* first one.
			* 
			* \see Polygon
			*/
		class Polyline : public Primitive {
			public:
			//! Make an empty polyline with style \c inStyle.
			Polyline(const Style &inStyle = Style()) : Primitive("polyline", inStyle) {}
			//! Make a polyline from point list \c inLinePath using style \c inStyle.
			Polyline(const PointList &inLinePath, const Style &inStyle = Style()) : Primitive("polyline", inStyle) {
				setAttribute("points", inLinePath.getStringValue());
			}
			
			
			//! Make new polyline from this polyline concatenated with point list \c inLinePath.
			Polyline operator+(const PointList &inLinePath) const {
				return Polyline(*this) += inLinePath;
			}			
			//! Append point list \c inLinePath to this polyline.
			Polyline &operator+=(const PointList &inLinePath) {
				(*this)["points"] += inLinePath.getStringValue();
				return *this;
			}			
			//! Make new polyline from this polyline concatenated with point \c inPoint.
			Polyline operator+(const Point &inPoint) const {
				return Polyline(*this) += inPoint;
			}			
			//! Append point \c inPoint to this polyline.
			Polyline &operator+=(const Point &inPoint) {
				(*this)["points"] += string(" ") + inPoint.getStringValue();
				return *this;
			}			
		};
		
		/*!\brief Graphic primitive for text.
			* \ingroup SVG
			*
			* A text object is used to display text in a graphic. Each character of the
			* enclosed text is represented by a glyph on with standard stroke and fill
			* attributes apply. For example you can put a stroke on the text to make
			* an outline, and you can change the color of the character using
			* Fill::Color.
			*
			* There is also some special attributes that can be applyed to text to
			* control the appearence of the glyphs, like the font family, size, and
			* style. See Font for details.
			*
			* The text is placed on screen around an anchor point. How text is aligned
			* on the anchor depends of the TextAnchor and TextBaseline attributes.
			*/
		class Text : public Primitive {
			public:
			// Make text from string \c inString, typeset at point \c inAnchor, using style \c inStyle.
			Text(const string &inString, const Point &inAnchor, const Style inStyle = Style()) : Primitive("text", inStyle) {
				setAttribute("x", String::convert(inAnchor.x));
				setAttribute("y", String::convert(-inAnchor.y));
				setTransform(Scale(1, -1));
				insertAsLastChild(new Node(inString, XML::eString));
			}
			
			//! Return text anchor point.
			Point getAnchor(void) const {
				return Point(String::convertToFloat(getAttribute("x")), String::convertToFloat(getAttribute("y")));
			}			
			//! Set text anchor point to \c inAnchor.
			void setAnchor(const Point &inAnchor) {
				setAttribute("x", String::convert(inAnchor.x));
				setAttribute("y", String::convert(inAnchor.y));
			}			
			//! Set text anchor point to coordinates \c inX and \c inY.
			void setAnchor(float inX, float inY) {
				setAttribute("x", String::convert(inX));
				setAttribute("y", String::convert(inY));
			}
			
			//! Return string of text.
			string getText(void) const {
				return getFirstChild()->getValue();
			}
			
			//! Set text to string \c inString.
			void setText(const string &inString) {
				getFirstChild()->setValue(inString);
			}			
		};
		
	} // end of SVG namespace
	
	//! Insert primitive \c inPrimitive into output stream \c outStream.
	inline ostream& operator<<(ostream &outStream, const SVG::Primitive& inPrimitive) {
		inPrimitive.write(outStream);
		return outStream;
	}
	
} // end of PACC namespace

#endif
