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
 */

#ifndef PACC_SVG_Transforms_hpp_
#define PACC_SVG_Transforms_hpp_

#include "PACC/SVG/Types.hpp"
#include "PACC/Util/StringFunc.hpp"

namespace PACC {
	
	namespace SVG {
		
		using namespace std;
		
		/*!\brief Coordinate tranform for graphic elements.
		 * \ingroup SVGtransform
		 */
		class Transform : public string {
		 public:
			Transform operator+(const Transform& inTransform);
			Transform& operator+=(const Transform& inTransform);
			
		 protected:
			// restricted usage
			Transform(const string& inName, const string& inValue);
		};
		
		//! \brief Rotation tranform.
		//! \ingroup SVGtransform
		class Rotate : public Transform {
		 public:
			Rotate(double inAngle);
			Rotate(double inAngle, const Point& inPoint);
			Rotate(double inAngle, double inX, double inY);
		};
		
		/*!\brief Translation tranform.
		 * \ingroup SVGtransform
		 *
		 * If you want to set a new origin, you can use the translation. As an 
		 * example, to put the origin at (1,1), translate by (-1,-1).
		 */
		class Translate : public Transform {
		 public:
			Translate(const Point& inPoint);
			Translate(double inX, double inY);
		};
		
		//! \brief Scaling tranform.
		//! \ingroup SVGtransform
		class Scale : public Transform {
		 public:
			Scale(double inScale);
			Scale(double inX, double inY);
		};
		
		//! \brief Horizontal skewing tranform.
		//! \ingroup SVGtransform
		class SkewX : public Transform {
		 public:
			SkewX(double inAngle);
		};
		
		//! \brief Vertical skewing tranform.
		//! \ingroup SVGtransform
		class SkewY : public Transform {
		 public:
			SkewY(double inAngle);
		};
		
		//! \brief General matrix transformation.
		//! \ingroup SVGtransform
		class MatrixTransform : public Transform {
		 public:
			MatrixTransform(double inA, double inB, double inC, double inD, 
							double inE, double inF);
		};
		
	} // end of SVG namespace
	
} // end of PACC namespace

#endif
