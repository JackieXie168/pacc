/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2001-2004 by Marc Parizeau
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
 *  \file   PACC/Math/Vector.hpp
 *  \brief  Definition of class Vector.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2005/06/02 07:00:42 $
 */
 
#ifndef PACC_Vector_hpp
#define PACC_Vector_hpp

#include "Util/Assert.hpp"
#include "Math/Matrix.hpp"
#include "XML/Document.hpp"
#include "XML/Streamer.hpp"
#include <vector>

namespace PACC {
	
	using namespace std;
	
   /*! \brief Single column matrix.
		\author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Math
		\ingroup MLP
		
		This class encapsulates a vector as a single column matrix.
	*/
   class Vector : protected Matrix
   {
    public:
		//! Construct empty vector with name \c inName.
		Vector(const string& inName="") : Matrix(inName) {}
      //! Constructs a vector of size \c inSize with name \c inName.
      explicit Vector(unsigned inSize, const string& inName="") : Matrix(inSize, 1, inName) {}
		//! Convert matrix to vector.
		Vector(const Matrix& inMatrix) : Matrix(inMatrix) {PACC_AssertM(mCols == 1, "cannot convert matrix to vector!");}
		
		//! Return reference to element \c inRow.
		inline double& operator[](unsigned int inRow) {
			PACC_AssertM(inRow < mRows, "invalid vector index!");
			return vector<double>::operator[](inRow);
		}
		
		//! Return const reference to element \c inRow.
		inline const double& operator[](unsigned int inRow) const {
			PACC_AssertM(inRow < mRows, "invalid vector index!");
			return vector<double>::operator[](inRow);
		}
		
		//! Add scalar \c inScalar to this vector, and return new vector.
		inline Vector operator+(double inScalar) const {Vector lVector; return add(lVector, inScalar);}
		//! Add scalar \c inScalar to this vector, and assign result to this vector.
		inline Vector& operator+=(double inScalar) {return (Vector&) add(*this, inScalar);}
		//! Add vector \c inVector to this vector, and return new vector.
		inline Vector operator+(const Vector& inVector) const {Vector lVector; return add(lVector, inVector);}
		//! Add vector \c inVector to this vector, and assign result to this vector.
		inline Vector& operator+=(const Vector& inVector) {return (Vector&) add(*this, inVector);}
		
		//! Substract scalar \c inScalar from this vector, and return new vector.
		inline Vector operator-(double inScalar) const {Vector lVector; return substract(lVector, inScalar);}
		//! Substract scalar \c inScalar from this vector, and assign result to this vector.
		inline Vector& operator-=(double inScalar) {return (Vector&) substract(*this,inScalar);}
		//! Substract vector \c inVector from this vector, and return new vector.
		inline Vector operator-(const Vector& inVector) const {Vector lVector; return substract(lVector, inVector);}
		//! Substract vector \c inVector from this vector, and assign result to this vector.
		inline Vector& operator-=(const Vector& inVector) {return (Vector&) substract(*this, inVector);}
		
		//! Multiply scalar \c inScalar to this vector, and return new vector.
		inline Vector operator*(double inScalar) const {Vector lVector; return multiply(lVector, inScalar);}
		//! Multiply scalar \c inScalar to this vector, and assign result to this vector.
		inline Vector& operator*=(double inScalar) {return (Vector&) multiply(*this, inScalar);}
		
		double operator*(const Vector& inVector) const;
		
		double computeL1Norm(void) const;
		double computeL2Norm(void) const;
		double computeL2Norm2(void) const;
		double computeLinfNorm(void) const;
		
		//! Return index of max element.
		int getArgMax(void) const {return distance(begin(), max_element(begin(), end()));}
		//! Return max element.
		double getMax(void) const {return *max_element(begin(), end());}
		
		//! Return size of this vector.
		unsigned int size(void) const {return Matrix::size();}
		//! Resize this vector to \c inSize dimensions.
		inline void resize(unsigned int inSize) {Matrix::resize(inSize,1);}
		
		string read(const XML::Iterator& inNode);
		void write(XML::Streamer& outStream, const string& inTag="Vector") const;
	};
   
   ostream& operator<<(ostream& outStream, const Vector& inVector);
   XML::Document& operator>>(XML::Document& inDocument, Vector& outMatrix);	
	
}

#endif // PACC_Vector_hpp

