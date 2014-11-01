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
 * \file PACC/SVG/Transforms.hpp
 * \brief Class definition for the SVG transform.
 * \author Marc Parizeau and Michel Fortin, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.8 $
 * $Date: 2007/01/23 21:27:47 $
 */

#ifndef PACC_SVG_Transforms_hpp_
#define PACC_SVG_Transforms_hpp_

#include "PACC/SVG/Types.hpp"
#include "PACC/Util/StringFunc.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Coordinate tranform for graphic elements.
		 * \ingroup SVG
		 */
		class Transform : public string {
		 public:
			//! Return the transform which results from the concatenation of this transform with \c inTranform.
			Transform operator+(const Transform& inTransform) {
				return Transform(*this) += inTransform;
			}
			
			//! Append transform \c inTranform to this transform.
			Transform& operator+=(const Transform& inTransform) {
				string::operator+=(string(" ")+inTransform);
				return *this;
			}
			
		 protected:
			//! Make transform with name \c inName and value \c inValue.
			Transform(const string &inName, const string &inValue) : string(inName+"("+inValue+")") {}
		};
		
		//! \brief Rotation tranform.
		//! \ingroup SVG
		class Rotate : public Transform {
		 public:
			//! Make a rotation transform of angle \c inAngle.
			Rotate(float inAngle) : Transform("rotate", String::convert(inAngle)) {}
			
			//! make a rotation transform of angle \c inAngle from point \c inPoint.
			Rotate(float inAngle, const Point &inPoint) : Transform("rotate", String::convert(inAngle) + " " + String::convert(inPoint.x) + " " + String::convert(inPoint.y)) {}
			
			//! make a rotation transform of angle \c inAngle from coordinates \c inX and \c inY.
			Rotate(float inAngle, float inX, float inY) : Transform("rotate", String::convert(inAngle) + " " + String::convert(inX) + " " + String::convert(inY)) {}
		};
		
		/*!\brief Translation tranform.
		 * \ingroup SVG
		 *
		 * If you want to set a new origin, you can use the translation. As an 
		 * example, to put the origin at (1,1), translate by (-1,-1).
		 */
		class Translate : public Transform {
		 public:
			//! Make a translation transform to point \c inPoint.
			Translate(const Point &inPoint) : Transform("translate", String::convert(inPoint.x) + " " + String::convert(inPoint.y)) {}
			
			//! make a translation transform to relative coordinates \c inX and \c inY.
			Translate(float inX, float inY) : Transform("translate", String::convert(inX) + " " + String::convert(inY)) {}
		};
		
		//! \brief Scaling tranform.
		//! \ingroup SVG
		class Scale : public Transform {
		 public:
			//! Make a scaling transform of scale \c inScale.
			Scale(float inScale) : Transform("scale", String::convert(inScale)) {}
			
			//! make a scaling transformm of scale \c inX and \c inY.
			Scale(float inX, float inY) : Transform("scale", String::convert(inX) + " " + String::convert(inY)) {}
		};
		
		//! \brief Horizontal skewing tranform.
		//! \ingroup SVG
		class SkewX : public Transform {
		 public:
			//! Make a horizontal skewing transform of angle \c inAngle. 
			SkewX(float inAngle) : Transform("skewX", String::convert(inAngle)) {}
		};
		
		//! \brief Vertical skewing tranform.
		//! \ingroup SVG
		class SkewY : public Transform {
		 public:
			//! Make a vertical skewing transform of angle \c inAngle.
			SkewY(float inAngle) : Transform("skewY", String::convert(inAngle)) {}
		};
		
		//! \brief General matrix transformation.
		//! \ingroup SVG
		class MatrixTransform : public Transform {
		 public:
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
			MatrixTransform(float inA, float inB, float inC, float inD, float inE, float inF) 
			: Transform("matrix", String::convert(inA) + " " + String::convert(inB) + " " + String::convert(inC) + " " + String::convert(inD) + " " + String::convert(inE) + " " + String::convert(inF)) {}
		};
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif
