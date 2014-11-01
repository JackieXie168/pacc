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
 * \file PACC/SVG/Transforms.cpp
 * \brief Class methods for the SVG transform.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 */

/*!
 * \defgroup SVGtransform Transformations
 * \ingroup SVG
 * \brief Attachable transformations for primitives and containers.
 */

#include "PACC/SVG/Transforms.hpp"

using namespace PACC;

//! Make transform with name \c inName and value \c inValue.
SVG::Transform::Transform(const string& inName, const string& inValue) 
: string(inName+"("+inValue+")") 
{
}

//! Return the transform which results from the concatenation of this transform with \c inTranform.
SVG::Transform SVG::Transform::operator+(const Transform& inTransform) 
{
	return Transform(*this) += inTransform;
}

//! Append transform \c inTranform to this transform.
SVG::Transform& SVG::Transform::operator+=(const Transform& inTransform) 
{
	string::operator+=(string(" ")+inTransform);
	return *this;
}
		
//! Make a rotation transform of angle \c inAngle.
SVG::Rotate::Rotate(double inAngle) 
: Transform("rotate", String::convert(inAngle)) 
{
}
			
//! make a rotation transform of angle \c inAngle from point \c inPoint.
SVG::Rotate::Rotate(double inAngle, const Point& inPoint) 
: Transform("rotate", String::convert(inAngle) + " " + String::convert(inPoint.x) + 
			" " + String::convert(inPoint.y)) 
{
}

//! make a rotation transform of angle \c inAngle from coordinates \c inX and \c inY.
SVG::Rotate::Rotate(double inAngle, double inX, double inY) 
: Transform("rotate", String::convert(inAngle) + " " + String::convert(inX) + 
			" " + String::convert(inY)) 
{
}
		
//! Make a translation transform to point \c inPoint.
SVG::Translate::Translate(const Point& inPoint) 
: Transform("translate", String::convert(inPoint.x) + " " + String::convert(inPoint.y)) 
{
}

//! make a translation transform to relative coordinates \c inX and \c inY.
SVG::Translate::Translate(double inX, double inY) 
: Transform("translate", String::convert(inX) + " " + String::convert(inY)) 
{
}

//! Make a scaling transform of scale \c inScale.
SVG::Scale::Scale(double inScale) 
: Transform("scale", String::convert(inScale)) 
{
}

//! make a scaling transformm of scale \c inX and \c inY.
SVG::Scale::Scale(double inX, double inY) 
: Transform("scale", String::convert(inX) + " " + String::convert(inY)) 
{
}
		

//! Make a horizontal skewing transform of angle \c inAngle. 
SVG::SkewX::SkewX(double inAngle) 
: Transform("skewX", String::convert(inAngle)) 
{
}
		
//! Make a vertical skewing transform of angle \c inAngle.
SVG::SkewY::SkewY(double inAngle) 
: Transform("skewY", String::convert(inAngle)) 
{
}
		
/*!\brief  Construct a matrix transform from the six upper values of 
 *         the matrix.
 * \param  inA  .
 * \param  inB  .
 * \param  inC  .
 * \param  inD  .
 * \param  inE  .
 * \param  inF  .
 *
 * Transformation matrix is of the form:
 *
 * <code>[a b c]<br>
 *       [d e f]<br>
 *       [0 0 1]</code>
 */
SVG::MatrixTransform::MatrixTransform(double inA, double inB, double inC, 
									  double inD, double inE, double inF) 
: Transform("matrix", String::convert(inA) + " " + String::convert(inB) + 
			" " + String::convert(inC) + " " + String::convert(inD) + 
			" " + String::convert(inE) + " " + String::convert(inF)) 
{
}
