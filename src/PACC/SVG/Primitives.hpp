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
 */

#ifndef PACC_SVG_Graphic_hpp_
#define PACC_SVG_Graphic_hpp_

#include "PACC/XML/Node.hpp"
#include "PACC/XML/Streamer.hpp"
#include "PACC/SVG/Styles.hpp"
#include "PACC/SVG/Transforms.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Base class for graphic elements.
		 * \ingroup SVGprimit
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
			void addStyle(const Style& inStyle);
			
			void clearTransform(void);
			void setTransform(const Transform& inTransform);
			
			void setID(const string& inID);
			
			void write(ostream& outStream) const;
			
		 protected:
			Primitive(const string& inName, const XML::AttributeList& inAttrList = XML::AttributeList());
			
			friend class Group; //!< Allowing the use of Element's << operator.
			friend class Document; //!< Allowing access to method Node::serialize
		};
		
		/*!\brief Graphic primitive for circles.
		 * \ingroup SVGprimit
		 * \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * A circle is defined by a central point (called the center) and a radius.
		 * The shape is formed of the closed curve where the distance between the
		 * center and each point of the curve is equal to the radius.
		 */
		class Circle : public Primitive {
		 public:
			Circle(const Point& inCenter, double inRadius, const Style& inStyle = Style());
			
			void setCenter(const Point& inCenter);
			void setRadius(double inRadius);	
		};
		
		/*!\brief Graphic primitive for ellipses.
		 * \ingroup SVGprimit
		 * \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * An ellipse is defined by a center point (the center) and a size (width/height).
		 */
		class Ellipse : public Primitive {
		 public:
			Ellipse(const Point& inCenter, const Size& inSize, const Style& inStyle = Style());
			
			void setCenter(const Point& inCenter);
			void setSize(const Size& inSize);
		};
		
		/*!\brief Graphic primitive for straight lines.
		 * \ingroup SVGprimit
		 * \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * \warning A line links a start point to an end point. Since a line has no fillable
		 * region and by default shapes do not have a stroke, you must explicitly
		 * define a stroke if you want the line to be visible.
		 */
		class Line : public Primitive {
		 public:
			Line(const Point& inStart, const Point& inEnd, const Style& inStyle=Style());
			
			void setStart(const Point& inStart);
			void setEnd(const Point& inEnd);
		};
		
		/*! \brief Graphic primitive for polygons
		 *  \ingroup SVGprimit
		 *  \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * A polygon is a sequence of points linked together by strait lines.
		 * A polygon is a closed path, the last and first points are always 
		 * linked.
		 */
		class Polygon : public Primitive {
		 public:
			Polygon(const Style& inStyle = Style());
			Polygon(const PointList& inLinePath, const Style& inStyle=Style());
			
			const Polygon operator+(const Point& inPoint) const;
			const Polygon operator+(const PointList& inList) const;

			Polygon& operator+=(const Point& inPoint);
			Polygon& operator+=(const PointList& inList);
		};
		
		/*! \brief Graphic primitive for polyline
		 *  \ingroup SVGprimit
		 *  \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * A polyline is a sequence of points linked together by strait lines. 
		 * A polyline is not a closed path, the last and first points are not 
		 * linked.
		 */
		class Polyline : public Primitive {
		 public:
			Polyline(const Style& inStyle = Style());
			Polyline(const PointList& inLinePath, const Style& inStyle=Style());

			const Polyline operator+(const Point& inPoint) const;
			const Polyline operator+(const PointList& inList) const;
			
			Polyline& operator+=(const Point& inPoint);
			Polyline& operator+=(const PointList& inList);
		};
		
		/*! \brief Graphic primitive for rectangles.
		 *  \ingroup SVGprimit
		 *  \see strokeColor, strokeDash, strokeLineCap, strokeLineJoin, strokeMiterLimit, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule
		 *
		 * A rectangle is a four-sided figure with two vertical sides and two
		 * horizontal ones.
		 */
		class Rectangle : public Primitive {
		public:
			Rectangle(const Point& inOrigin, const Size& inSize, const Style& inStyle=Style());
			
			void setCornerRadii(double inRx, double inRy);
			void setOrigin(const Point& inOrigin);
			void setSize(const Size& inSize);
		};
		
		/*!\brief Graphic primitive for text.
		 * \ingroup SVGprimit
		 * \see strokeColor, strokeOpacity, strokeWidth, fillColor, fillOpacity, fillRule, fontFamily, fontStyle, fontSize, textAnchor
		 *
		 * A text object is used to display text in a graphic. Each character of the
		 * enclosed text is represented by a glyph on with standard stroke and fill
		 * attributes apply. For example you can put a stroke on the text to make
		 * an outline, and you can change the color of the character using
		 * style attribute strokeColor.
		 *
		 * There is also some special attributes that can be applyed to text to
		 * control the appearence of the glyphs, like the font family, size, and
		 * style.
		 *
		 * The text is placed on screen around an anchor point. How text is aligned
		 * on the anchor depends of the textAnchor style attribute.
		 */
		class Text : public Primitive {
		 public:
			//! Text substring.
			class Span : protected XML::Node {
			public:
				Span(const string& inString, const Style& inStyle=Style());
				Span(const string& inString, const RelPos& inPos, const Style& inStyle=Style());
				
				friend class Text;
			};
					 
			Text(const string& inString, const Point& inPoint, const Style& inStyle=Style());
			
			void setAnchor(const Point& inAnchor);
			
			Text& operator<<(const Span& inSpan);
			
		private:
			XML::Node* mText; //<! pointer to text node 
		};
		
	} // end of SVG namespace
	
	ostream& operator<<(ostream& outStream, const SVG::Primitive& inPrimitive);
	
} // end of PACC namespace

#endif
