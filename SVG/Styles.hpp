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
 * \file PACC/SVG/Styles.hpp
 * \brief Class definition for the SVG element styles.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.6 $
 * $Date: 2005/11/29 22:31:51 $
 */

#ifndef PACC_SVG_Styles_hpp_
#define PACC_SVG_Styles_hpp_

#include "SVG/Types.hpp"
#include "XML/Attribute.hpp"
#include "Util/StringFunc.hpp"

namespace PACC {
	
	using namespace String;
	
	namespace SVG {
		
		using namespace std;
		
		class Style;
		
		/*! \brief Container for a single style attribute.
			\ingroup SVG
			
			An attribute a pair of attribute name / attribute value. Its only 
			constructor is protected in order to restrict its usage. It will be
			sub-classed for specific %SVG style attributes.
		*/
		class Attribute : public pair<string, string> {
		 public:
			//! Return a style list of attributes by combining attribute \c inAttribute with this attribute.
			Style operator+(const Attribute& inAttribute) const;
			
			//! Return a style list of attributes by merging list \c inAttrList with this attribute.
			Style operator+(const Style& inAttrList) const;
			
			//! Return name of this attribute.
			string& getName(void) {return first;}
			
			//! Return name of this attribute (const).
			const string& getName(void) const {return first;}
			
			//! Return value of this attribute.
			string& getValue(void) {return second;}

			//! Return value of this attribute (const).
			const string& getValue(void) const {return second;}
			
		protected:
			//! Construct an attribute style from name \c inName and value \c inValue.
			Attribute(const string& inName, const string& inValue) : pair<string, string>(inName, inValue) {}
		};
		
		/*! \brief List of attributes representing the style of an element.
			\ingroup SVG

			A style object holds a list of style attributes.

			\see Attribute
		*/
		class Style : public XML::AttributeList {
		 public:
			
			//! Make an empty style.
			Style() {}
			
			//! Make a style from a single attribute \c inAttribute.
			Style(const Attribute& inAttribute) {insert(inAttribute);}
			
			//! Make a style from an attribute list \c inList.
			Style(const XML::AttributeList& inList) : XML::AttributeList(inList) {}
			
			//! Return concatenation of style list \c inAttrList with this list.
			Style operator+(const Style& inAttrList) const {
				return Style(*this) += inAttrList;
			}
			
			//! Append attribute list \c inAttrList to this list.
			Style& operator+=(const Style& inAttrList) {
				for(Style::const_iterator lPos = inAttrList.begin(); lPos != inAttrList.end(); ++lPos) {
					(*this)[lPos->first] = lPos->second;
				}
				return *this;
			}
		};
		
		/*!\brief %Style attribute for object opacity.
		 * \ingroup SVG
		 *
		 * This will set the opacity of the resulting image of a graphic. 
		 *
		 * This is different than setting FillOpacity and StrokeOpacity. For
		 * example, if you set both StrokeOpacity and FillOpacity to 0.5, you will 
		 * see the fill edge behind the stroke. With opacity the hole shape is
		 * rendered and then made transparent.
		 */
		class Opacity : public Attribute {
		 public:
			//! Set the graphic opacity to value \c inValue.
			Opacity(float inValue) : Attribute("opacity", String::convert(inValue)) {}
		};
		
		/*!\brief %Style attribute for fill color of shapes.
		 * \ingroup SVG
		 *
		 * Currently this class can only take plain color fills. 
		 * It may be extended in the future to support gradient or patterns.
		 */
		class Fill : public Attribute {
		 public:
			//! Set the fill color to value \c inValue.
			Fill(const Color &inValue) : Attribute("fill", inValue) {}
			
			//! This constant represent an empty (or transparent) fill.
			static const Fill cNone;
		};
		
		/*!\brief %Style attribute for the opacity of fills.
		 * \ingroup SVG
		 * \see  Fill
		 */
		class FillOpacity : public Attribute {
		 public:
			//! Set the fill opacity to value \c inValue (0=transparent).
			FillOpacity(float inOpacity) : Attribute("fill-opacity", String::convert(inOpacity)) {}
		};
		
		/*!\brief %Style attribute for specifying the fill rule.
		 * \ingroup SVG
		 *
		 * Overlapping regions can be filled according to differents rules
		 * defined as constants inside this class.
		 */
		class FillRule : public Attribute {
		 public:
			/*! \brief After counting the crossings, if the result is zero then the 
			*         point is outside the path. Otherwise, it is inside.
			*
			* This rule determines the "insideness" of a point on the canvas by 
			* drawing a ray from that point to infinity in any direction and then 
			* examining the places where a segment of the shape crosses the ray. 
			* Starting with a count of zero, add one each time a path segment 
			* crosses the ray from left to right and subtract one each time a 
			* path segment crosses the ray from right to left. After counting the 
			* crossings, if the result is zero then the point is outside the path. 
			* Otherwise, it is inside. The following drawing illustrates the 
			* non-zero rule:
			* 
			* \image html fillrule-nonzero.png
			*/
			static const FillRule cNonZero;

			/*! \brief After counting the crossings, if the count is odd, the point
			*         is inside; if even, the point is outside.
			*
			* This rule determines the "insideness" of a point on the canvas by 
			* drawing a ray from that point to infinity in any direction and 
			* counting the number of path segments from the given shape that the 
			* ray crosses. If this number is odd, the point is inside; if even, 
			* the point is outside. The following drawing illustrates the even-odd 
			* rule:
			* 
			* \image html fillrule-evenodd.png
			*/
			static const FillRule cEvenOdd;
			
		 private:
			//! Set fill rule to value \c inValue.
			FillRule(const string &inFillRule) : Attribute("fill-rule", inFillRule) {}
		};
		
		/*!\brief %Style attribute for specifying how to fill the stroke of a shape.
		 * \ingroup SVG
		 *
		 * Currently this class can only take plain color strokes. It may be 
		 * extended in the future to support gradient or patterns strokes.
		 */
		class Stroke : public Attribute {
		 public:
			//! Set stroke color to \c inColor.
			Stroke(const Color& inColor) : Attribute("stroke", inColor) {}
			
			//! This constant represent an non-existant (or invisible) stroke.
			static const Stroke cNone;
		};
		
		//! \brief %Style attribute for the opacity of strokes.
		//! \ingroup SVG
		class StrokeOpacity : public Attribute {
		 public:
			//! set stroke opacity to value \c inValue (0=transparent).
			StrokeOpacity(float inValue) : Attribute("stroke-opacity", String::convert(inValue)) {}
		};
		
		/*!\brief %Style attribute specifying width of the stroke.
		 * \ingroup SVG
		 *
		 * Defautl width of as stroke is 1.
		 */
		class StrokeWidth : public Attribute {
		 public:
			//! Set stroke width to value \c inValue.
			StrokeWidth(float inValue) : Attribute("stroke-width", String::convert(inValue)) {}
		};
		
		/*!\brief %Style attribute for specifying how to dash the stroke.
		 * \ingroup SVG
		 *
		 * The dash array is a list of numbers repsesenting the lengths of each
		 * part of the dashed line. The first number represent the first dash 
		 * length, the next is the empty space length, the next is the second dash
		 * lenght, and so on.
		 *
		 * \see  DashArray
		 */
		class StrokeDashArray : public Attribute {
		 public:
			/*!\brief  Make a dash array from a string.
			 * \param  inDashArray  A string with the lenght of dash and spaces
			 *                      separated by spaces characters.
			 *
			 * Sample dash array value: "10 2.5 5"
			 */
			StrokeDashArray(const std::string &inDashArray) : Attribute("stroke-dasharray", inDashArray) {}

			/*!\brief  Construct a dash array with a vector of floats.
			 * \param  inDashArray  A vector of floats as the list of dashes and 
			 *                      spaces.
			 */
			StrokeDashArray(const std::vector<float> &inDashArray);
			
			//! Default dash array for a continous line.
			static const StrokeDashArray cContinuous;

			//! Default dash array for a dotted line (valid only for stroke width of 1).
			static const StrokeDashArray cDotted;

			//! Default dash array for a dashed line.
			static const StrokeDashArray cDashed;
		};
		
		/*!\brief %Style attribute for specifying the type of cap for stroke ends.
		 * \ingroup SVG
		 * \see LineCap
		 */
		class StrokeLineCap : public Attribute {
		 public:
			//! \image html linecap-butt.png
			static const StrokeLineCap cButt;

			//! \image html linecap-round.png
			static const StrokeLineCap cRound;

			//! \image html linecap-square.png
			static const StrokeLineCap cSquare;
			
		 private:
			//! Set line cap style to value \c inValue.
			StrokeLineCap(const std::string &inValue) : Attribute("stroke-linecap", inValue) {}
		};
		
		/*!\brief %Style attribute for specifying the type of join between stroke 
		 *         segments.
		 * \ingroup SVG
		 * \see LineJoin
		 */
		class StrokeLineJoin : public Attribute {
		 public:
			//! \image html linejoin-miter.png
			static const StrokeLineJoin cMiter;

			//! \image html linejoin-round.png
			static const StrokeLineJoin cRound;

			//! \image html linejoin-bevel.png
			static const StrokeLineJoin cBevel;
			
		 private:
			//! Set line join style to value \c inValue.
			StrokeLineJoin(const std::string &inValue) : Attribute("stroke-linejoin", inValue) {}
		};
		
		/*!\brief %Style attribute for specifying the length to width ratio of strokes.
			\ingroup SVG

			This style attribute has no effect on strokes with line join that is not
			set to miter. The default miter limit ratio is 4.

			\see  StrokeLineJoin, MiterLimit
		*/
		class StrokeMiterLimit : public Attribute {
		 public:
			//! Set the meter limit to ratio \c inRatio.
			StrokeMiterLimit(float inRatio) : Attribute("stroke-miterlimit", String::convert(inRatio)) {}
		};
		
		/*!\brief %Style attribute for specifying the font family.
		 * \ingroup SVG
		 */
		class FontFamily : public Attribute {
		 public:
			/*!\brief  Constructor with family name.
			 * \param  inName  The name of the family.
			 * 
			 * You can also optionally pass a list of families to choose from. In
			 * this case the first matching family will be used. The list must be
			 * of the form: "Times, Times New Roman, serif".
			 */
			FontFamily(const std::string &inName) : Attribute("font-family", inName) {}
			
			//! Default serif font from the viewer.
			static const FontFamily cSerif;

			//! \brief  Default sans serif font from the viewer.
			static const FontFamily cSansSerif;
		};
		
		//! \brief %Style attribute for specifying text style.
		//! \ingroup SVG
		class FontStyle : public Attribute {
		 public:
			//! Use normal font style.
			static const FontStyle cNormal;

			//! Use italic font style (similar to cOblique).
			static const FontStyle cItalic;

			//! Use oblique font style (similar to cItalic).
			static const FontStyle cOblique;
			
		 private:
			//! Constructor with font style name.
			//! \param inName A valid CSS style name string.
			FontStyle(const std::string &inName) : Attribute("font-style", inName) {}
		};
		
		//! \brief %Style attribute for defining font weight.
		//! \ingroup SVG
		class FontWeight : public Attribute {
		 public:
			//! Normal font weight.
			static const FontWeight cNormal;

			//! Bold font weight.
			static const FontWeight cBold;
			
		 private:
			//! Constructor with weight name.
			FontWeight(const std::string &inName) : Attribute("font-weight", inName) {}
		};
		
		//! \brief %Style attribute for specifying font size.
		//! \ingroup SVG
		class FontSize : public Attribute {
		 public:
			//! Set font size to value \c inValue.
			FontSize(float inValue) : Attribute("font-size", String::convert(inValue)) {}
		};
		
		/*!\brief %Style attribute for specifying text positioning.
		 *
		 * This attribute allow you to specify if the text anchor should be at the
		 * start, the middle or the end of the text. Static member constants are
		 * defined for each allowed value.
		 */
		class TextAnchor : public Attribute {
		 public:
			//! Start the text at the anchor point.
			static const TextAnchor cStart;

			//! Put the middle of the text at the anchor point.
			static const TextAnchor cMiddle;

			//! Put the end of the text at the anchor point.
			static const TextAnchor cEnd;
			
		 private:
			/*!\brief  Anchor the text using the value given in the string.
			 * \param  inValue A valid SVG value for the <code>text-anchor</code>
			 *                 attribute.
			 */
			TextAnchor(const std::string &inValue) : Attribute("text-anchor", inValue) {}
		};
		
		/*!\brief %Style attribute for specifying the text decoration.

			Supported decorations are underline and line-through.
		 */
		class TextDecoration : public Attribute {
		 public:
			//! Clears all text decorations.
			static const TextDecoration cNone;
			
			//! Set an underline decoration for the text.
			static const TextDecoration cUnderline;
			
			//! Set a line-through decoration for the text.
			static const TextDecoration cLineThrough;
			
		 private:
			TextDecoration(const std::string &inDecoration) : Attribute("text-decoration", inDecoration) {}
		};
		
		typedef StrokeDashArray DashArray;
		typedef StrokeLineCap LineCap;
		typedef StrokeLineJoin LineJoin;
		typedef StrokeMiterLimit MiterLimit;
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif // PACC_SVG_Styles_hpp_
