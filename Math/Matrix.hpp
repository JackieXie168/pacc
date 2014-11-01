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
 *  \file   PACC/Math/Matrix.hpp
 *  \brief  Definition of class Matrix.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.4 $
 *  $Date: 2005/04/19 13:57:09 $
 */
 
#ifndef PACC_Matrix_hpp
#define PACC_Matrix_hpp

#include "Util/Assert.hpp"
#include "XML/Document.hpp"
#include "XML/Streamer.hpp"
#include <vector>

namespace PACC {
	
	using namespace std;

	// Forward declarations
	class Vector;

	/*! \brief %Matrix of floating point numbers.
	    \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
	    \ingroup Math
		 \ingroup MLP
	 
	 This class encapsulates a vector of floating point numbers (double) as a matrix. It contains operators and methods for sum, difference, and product of matrices, as well as product with a scalar. It also includes matrix transposition and inversion methods, as well as computation of eigenvalues and eigenvectors for symetric matrices. Matrices can read and write themselves in %XML.
    */
	class Matrix : protected vector<double> {
    public:		
		//! Construct default empty matrix with name \c inName.
		Matrix(const string& inName="") : mRows(0), mCols(0), mName(inName) {}
		//! Construct matrix of \c inRows rows by \c inColumns columns, with name \c inName.
		explicit Matrix(unsigned int inRows, unsigned int inCols, const string& inName="") : vector<double>(inRows*inCols, 0), mRows(inRows), mCols(inCols), mName(inName) {}
		//! Matrix destructor.
		virtual ~Matrix() {mRows = mCols = 0;}
		
		//! Assign matrix \c inMatrix to this matrix but do not overwrite name unless it is undefined.
		Matrix& operator=(const Matrix& inMatrix) {
			if(&inMatrix != this) {
				// don't self assign!
				vector<double>::operator=(inMatrix);
				mRows = inMatrix.mRows; mCols = inMatrix.mCols;
				if(mName == "") mName = inMatrix.mName;
			}
			return *this;
		}
		
		//! Return const reference to element \c (inRow,inColumn).
		inline const double& operator()(unsigned int inRow, unsigned int inCol) const {
			PACC_AssertM(inRow < mRows && inCol < mCols, "invalid matrix indices!");
			return (*this)[(inRow*mCols)+inCol];
		}
		
		//! Return reference to element \c (inRow,inColumn).
		inline double& operator()(unsigned int inRow, unsigned int inCol) {
			PACC_AssertM(inRow < mRows && inCol < mCols, "invalid matrix indices!");
			return (*this)[(inRow*mCols)+inCol];
		}
		
		//! Add scalar \c inScalar to this matrix, and return new matrix.
		inline Matrix operator+(double inScalar) const {Matrix lMatrix; return add(lMatrix, inScalar);}
		//! Add scalar \c inScalar to this matrix, and assign result to this matrix.
		inline Matrix& operator+=(double inScalar) {return add(*this, inScalar);}
		//! Add matrix \c inMatrix to this matrix, and return new matrix.
		inline Matrix operator+(const Matrix& inMatrix) const {Matrix lMatrix; return add(lMatrix, inMatrix);}
		//! Add matrix \c inMatrix to this matrix, and assign result to this matrix.
		inline Matrix& operator+=(const Matrix& inMatrix) {return add(*this, inMatrix);}
		
		//! Substract scalar \c inScalar from this matrix, and return new matrix.
		inline Matrix operator-(double inScalar) const {Matrix lMatrix; return substract(lMatrix, inScalar);}
		//! Substract scalar \c inScalar from this matrix, and assign result to this matrix.
		inline Matrix& operator-=(double inScalar) {return substract(*this, inScalar);}
		//! Substract matrix \c inMatrix from this matrix, and return new matrix.
		inline Matrix operator-(const Matrix& inMatrix) const {Matrix lMatrix; return substract(lMatrix, inMatrix);}
		//! Substract matrix \c inMatrix from this matrix, and assign result to this matrix.
		inline Matrix& operator-=(const Matrix& inMatrix) {return substract(*this, inMatrix);}
		
		//! Multiply scalar \c inScalar with this matrix, and return new matrix.
		inline Matrix operator*(double inScalar) const {Matrix lMatrix; return multiply(lMatrix, inScalar);}
		//! Multiply scalar \c inScalar with this matrix, and assign result to this matrix.
		inline Matrix& operator*=(double inScalar) {return multiply(*this, inScalar);}
		//! Multiply this matrix with matrix \c inMatrix, and return new matrix.
		inline Matrix operator*(const Matrix& inMatrix) const {Matrix lMatrix; return multiply(lMatrix, inMatrix);}
		//! Multiply this matrix with matrix \c inMatrix, and assign result to this matrix.
		inline Matrix operator*=(const Matrix& inMatrix) {return multiply(*this, inMatrix);}
		
		//! Return number of columns.
		inline unsigned int getCols(void) const {return mCols;}
		//! Return number of row.
		inline unsigned int getRows(void) const {return mRows;}
				
		Matrix& add(Matrix& outMatrix, double inScalar) const;
		Matrix& add(Matrix& outMatrix, const Matrix& inMatrix) const;
		double computeDeterminant(void) const;
		void computeEigens(Vector& outValues, Matrix& outVectors) const;
		Matrix& extract(Matrix& outMatrix, unsigned int inRow1, unsigned int inRow2, unsigned int inCol1, unsigned int inCol2) const;
		Matrix& extractColumn(Matrix& outVector, unsigned int inCol) const;
		Matrix& extractRow(Matrix& outVector, unsigned int inRow) const;
		Matrix invert(void) const;
		Matrix& invert(Matrix& outMatrix) const;
		Matrix& substract(Matrix& outMatrix, double inScalar) const;
		Matrix& substract(Matrix& outMatrix, const Matrix& inMatrix) const;
		Matrix& multiply(Matrix& outMatrix, double inScalar) const;
		Matrix& multiply(Matrix& outMatrix, const Matrix& inMatrix) const;
		Matrix transpose(void) const;
		Matrix& transpose(Matrix& outMatrix) const;

		void resize(unsigned int inRows, unsigned int inCols);
		void setIdentity(unsigned int inSize);
		
		//! Return matrix name.
		const string& getName(void) const {return mName;}
		//! Set matrix name.
		void setName(string& inName) {mName = inName;};

		string read(const XML::Iterator& inNode);
		void write(XML::Streamer& outStream, const string& inTag="Matrix") const;
		
protected:
		unsigned int mRows; //!< Number of rows.
		unsigned int mCols; //!< Number of columns.
		string mName; //!< Matrix name.
		
		void computeBackSubLU(const vector<unsigned int>& inIndexes, Matrix& ioMatrixB) const;
		void decomposeLU(Matrix& outDecompose, vector<unsigned int>& outIndexes, int& outD) const;
		double hypot(double a, double b) const;
		void scaleLU(vector<double>& outScales) const;
		void tql2(Vector& d, Vector& e, Matrix& V) const;
		void tred2(Vector& d, Vector& e, Matrix& V) const;
		
		//! Set matrix size to \c inRows rows and \c inCols columns; matrix content is lost.
		inline void setRowsCols(unsigned int inRows, unsigned int inCols) {
			mRows = inRows; mCols = inCols;
			vector<double>::resize(mRows*mCols);
		}
		
		void throwError(const string& inMessage, const XML::Iterator& inNode) const;

	};
	
   ostream& operator<<(ostream& outStream, const Matrix& inMatrix);	
   XML::Document& operator>>(XML::Document& inDocument, Matrix& outMatrix);	
	
}

#endif // PACC_Matrix_hpp

