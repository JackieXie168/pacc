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
 *  $Revision: 1.19 $
 *  $Date: 2007/09/13 12:58:36 $
 */

#ifndef PACC_Vector_hpp
#define PACC_Vector_hpp

#include "PACC/Util/Assert.hpp"
#include "PACC/Math/Matrix.hpp"
#include "PACC/XML/Document.hpp"
#include "PACC/XML/Streamer.hpp"
#include <cmath>
#include <algorithm>

namespace PACC {
	
	using namespace std;
	
	/*! 
	\brief Single column matrix.
	\author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	\ingroup Math
	\ingroup MLP
		
	This class encapsulates a vector as a single column matrix.
	
	\attention Vector indices start at 0.
	*/
	class Vector : public Matrix {
		public:
		//! Construct an empty vector with name \c inName.
		Vector(const string& inName="") : Matrix(0, 1, inName) {}
		
		//! Constructs a vector of size \c inSize, initialized with 0, and withname \c inName.
		explicit Vector(unsigned int inSize, const string& inName="") : Matrix(inSize, 1, inName) {}
		
		//! Constructs a vector of size \c inSize, initialized with value \c inValue, and with name \c inName.
		explicit Vector(unsigned int inSize, double inValue, const string& inName="") : Matrix(inSize, 1, inValue, inName) {}
		
		//! Convert matrix to vector.
		Vector(const Matrix& inMatrix) : Matrix(inMatrix) {
			PACC_AssertM(mCols == 1, "Vector::Vector() cannot convert matrix to vector!");
		}
		
		//! Return reference to element \c inRow.
		inline double& operator[](unsigned int inRow) {
			PACC_AssertM(mCols == 1, "Vector::operator[] invalid vector!");
			PACC_AssertM(inRow < mRows, "Vector::operator[] invalid vector index!");
			return vector<double>::operator[](inRow);
		}
		
		//! Return const reference to element \c inRow.
		inline const double& operator[](unsigned int inRow) const {
			PACC_AssertM(mCols == 1, "Vector::operator[] invalid vector!");
			PACC_AssertM(inRow < mRows, "Vector::operator[] invalid vector index!");
			return vector<double>::operator[](inRow);
		}
		
		//! Add scalar \c inScalar to this vector, and return new vector.
		inline Vector operator+(double inScalar) const {
			Matrix lMatrix; 
			return add(lMatrix, inScalar);
		}
		
		//! Add scalar \c inScalar to this vector, and assign result to this vector.
		inline Vector& operator+=(double inScalar) {
			return (Vector&) add((Matrix&)*this, inScalar);
		}
		
		//! Add vector \c inVector to this vector, and return new vector.
		inline Vector operator+(const Vector& inVector) const {
			Matrix lMatrix; 
			return add(lMatrix, inVector);
		}
		
		//! Add vector \c inVector to this vector, and assign result to this vector.
		inline Vector& operator+=(const Vector& inVector) {
			return (Vector&) add((Matrix&)*this, inVector);
		}
		
		//! Add matrix \c inMatrix to this vector, and return new vector.
		inline Vector operator+(const Matrix& inMatrix) const {
			Matrix lMatrix; 
			return add(lMatrix, inMatrix);
		}
		
		//! Add matrix \c inMatrix to this vector, and assign result to this vector.
		inline Vector& operator+=(const Matrix& inMatrix) {
			return (Vector&) add((Matrix&)*this, inMatrix);
		}
		
		//! Subtract scalar \c inScalar from this vector, and return new vector.
		inline Vector operator-(double inScalar) const {
			Matrix lMatrix; 
			return subtract(lMatrix, inScalar);
		}
		
		//! Subtract scalar \c inScalar from this vector, and assign result to this vector.
		inline Vector& operator-=(double inScalar) {
			return (Vector&) subtract((Matrix&)*this,inScalar);
		}
		
		//! Subtract vector \c inVector from this vector, and return new vector.
		inline Vector operator-(const Vector& inVector) const {
			Matrix lMatrix; 
			return subtract(lMatrix, inVector);
		}
		
		//! Subtract vector \c inVector from this vector, and assign result to this vector.
		inline Vector& operator-=(const Vector& inVector) {
			return (Vector&) subtract((Matrix&)*this, inVector);
		}
		
		//! Subtract matrix \c inMatrix from this vector, and return new vector.
		inline Vector operator-(const Matrix& inMatrix) const {
			Matrix lMatrix; 
			return subtract(lMatrix, inMatrix);
		}
		
		//! Subtract matrix \c inMatrix from this vector, and assign result to this vector.
		inline Vector& operator-=(const Matrix& inMatrix) {
			return (Vector&) subtract((Matrix&)*this, inMatrix);
		}
		
		//! Multiply scalar \c inScalar to this vector, and return new vector.
		inline Vector operator*(double inScalar) const {
			Matrix lMatrix; 
			return multiply(lMatrix, inScalar);
		}
		
		//! Multiply scalar \c inScalar to this vector, and assign result to this vector.
		inline Vector& operator*=(double inScalar) {
			return (Vector&) multiply((Matrix&)*this, inScalar);
		}
		
		//! Multiply matrix \c inMatrix to this vector, and return new matrix.
		inline Matrix operator*(const Matrix& inMatrix) const {
			Matrix lMatrix; 
			return multiply(lMatrix, inMatrix);
		}
		
		//! Compute inner product of this vector with vector \c inVector.
		inline double operator*(const Vector& inVector) const {
			PACC_AssertM(mRows == inVector.mRows, "Vector::operator*() vector mismatch for inner product!");
			double lResult = 0;
			for(unsigned int i = 0; i < mRows; ++i) lResult += (*this)[i] * inVector[i];
			return lResult;
		}
		
		//! Compute L1 norm for this vector.
		inline double computeL1Norm(void) const {
			PACC_AssertM(mCols == 1, "Vector::computeL1Norm() invalid vector!");
			Matrix lResult;
			sumAbsColumns(lResult);
			return lResult(0,0);
		}
		
		//! Compute L2 norm for this vector.
		inline double computeL2Norm(void) const {
			PACC_AssertM(mCols == 1, "Vector::computeL2Norm() invalid vector!");
			Matrix lResult;
			sum2Columns(lResult);
			return sqrt(lResult(0,0));
		}
		
		//! Compute square of L2 norm for this vector.
		inline double computeL2Norm2(void) const {
			PACC_AssertM(mCols == 1, "Vector::computeL2Norm2() invalid vector!");
			Matrix lResult;
			sum2Columns(lResult);
			return lResult(0,0);
		}
		
		//! Compute Linf norm for this vector.
		inline double computeLinfNorm(void) const {
			PACC_AssertM(mCols == 1, "Vector::computeLinfNorm() invalid vector!");
			Matrix lResult;
			maxColumns(lResult);
			return lResult(0,0);
		}
		
		//! Return index of max element.
		inline int getArgMax(void) const {
			PACC_AssertM(mCols == 1, "Vector::getArgMax() invalid number of columns!");
			return distance(begin(), max_element(begin(), end()));
		}
		
		//! Return max element.
		inline double getMax(void) const {
			PACC_AssertM(mCols == 1, "Vector::getMax() invalid number of columns!");
			return *max_element(begin(), end());
		}
		
		//! Return index of min element.
		inline int getArgMin(void) const {
			PACC_AssertM(mCols == 1, "Vector::getArgMin() invalid number of columns!");
			return distance(begin(), min_element(begin(), end()));
		}
		
		//! Return min element.
		inline double getMin(void) const {
			PACC_AssertM(mCols == 1, "Vector::getMin() invalid number of columns!");
			return *min_element(begin(), end());
		}
		
		//! Return size of this vector.
		inline unsigned int size(void) const {
			PACC_AssertM(mCols == 1, "Vector::size() invalid vector!");
			return mRows;
		}
		
		//! Resize this vector to \c inSize dimensions.
		inline void resize(unsigned int inSize) {
			PACC_AssertM(mCols == 1, "Vector::resize() invalid vector!");
			Matrix::resize(inSize,1);
		}
		
		//! Read from parse tree node \c inNode.
		string read(const XML::ConstIterator& inNode);
		
		//! Write this vector into streamer \c outStream using tag name \c inTag.
		void write(XML::Streamer& outStream, const string& inTag="Vector") const;
		
	};
	
	//! Insert vector \c inVector into output stream \c outStream.
	ostream& operator<<(ostream& outStream, const Vector& inVector);
	
	//! Extract vector \c outVector from %XML document \c inDocument.
	XML::Document& operator>>(XML::Document& inDocument, Vector& outVector);	
	
}

#endif // PACC_Vector_hpp

