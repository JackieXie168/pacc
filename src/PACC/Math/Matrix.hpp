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
 *  $Revision: 1.23 $
 *  $Date: 2007/02/24 19:33:15 $
 */

#ifndef PACC_Matrix_hpp
#define PACC_Matrix_hpp

#include "PACC/Util/Assert.hpp"
#include "PACC/XML/Document.hpp"
#include "PACC/XML/Streamer.hpp"
#include <vector>

namespace PACC {
	
	using namespace std;
	
	// Forward declarations
	class Vector;
	
	/*! \brief %Matrix of floating point numbers.
		\author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et 
		syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
		\ingroup Math
		\ingroup MLP
		
		This class encapsulates a vector of floating point numbers (double) as a matrix. 
		It contains operators and methods for sum, difference, and product of matrices, 
		as well as product with a scalar. It also includes matrix transposition and 
		inversion methods, as well as computation of eigenvalues and eigenvectors 
		for symetric matrices. Matrices can read and write themselves in %XML.
		
		\attention Row and column indices start at 0.
    */
	class Matrix : protected vector<double> {
	 public:		
		//! Construct an empty matrix with name \c inName.
		Matrix(const string& inName="") : mRows(0), mCols(0), mPrec(15), mName(inName) {}
		
		//! Construct a matrix of size \c inRows rows by \c inColumns columns, initialized with 0, and with name \c inName.
		explicit Matrix(unsigned int inRows, unsigned int inCols, const string& inName="") 
		: vector<double>(inRows*inCols, 0), mRows(inRows), mCols(inCols), mPrec(15), mName(inName) {}
		
		//! Construct a matrix of size \c inRows rows by \c inColumns columns, initialized with value \c inValue, and with name \c inName.
		explicit Matrix(unsigned int inRows, unsigned int inCols, double inValue, const string& inName="") 
		: vector<double>(inRows*inCols, inValue), mRows(inRows), mCols(inCols), mPrec(15), mName(inName) {}
		
		//! Delete this matrix.
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
			PACC_AssertM(inRow < mRows && inCol < mCols, "Matrix::operator() invalid matrix indices!");
			return (*this)[(inRow*mCols)+inCol];
		}
		
		//! Return reference to element \c (inRow,inColumn).
		inline double& operator()(unsigned int inRow, unsigned int inCol) {
			PACC_AssertM(inRow < mRows && inCol < mCols, "Matrix::operator() invalid matrix indices!");
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
		
		//! Subtract scalar \c inScalar from this matrix, and return new matrix.
		inline Matrix operator-(double inScalar) const {Matrix lMatrix; return subtract(lMatrix, inScalar);}
		
		//! Subtract scalar \c inScalar from this matrix, and assign result to this matrix.
		inline Matrix& operator-=(double inScalar) {return subtract(*this, inScalar);}
		
		//! Subtract matrix \c inMatrix from this matrix, and return new matrix.
		inline Matrix operator-(const Matrix& inMatrix) const {Matrix lMatrix; return subtract(lMatrix, inMatrix);}
		
		//! Subtract matrix \c inMatrix from this matrix, and assign result to this matrix.
		inline Matrix& operator-=(const Matrix& inMatrix) {return subtract(*this, inMatrix);}
		
		//! Multiply scalar \c inScalar with this matrix, and return new matrix.
		inline Matrix operator*(double inScalar) const {Matrix lMatrix; return multiply(lMatrix, inScalar);}
		
		//! Multiply scalar \c inScalar with this matrix, and assign result to this matrix.
		inline Matrix& operator*=(double inScalar) {return multiply(*this, inScalar);}
		
		//! Multiply this matrix with matrix \c inMatrix, and return new matrix.
		inline Matrix operator*(const Matrix& inMatrix) const {Matrix lMatrix; return multiply(lMatrix, inMatrix);}
		
		//! Multiply this matrix with matrix \c inMatrix, and assign result to this matrix.
		inline Matrix operator*=(const Matrix& inMatrix) {return multiply(*this, inMatrix);}
		
		//! Return number of columns (this function is deprecated; use Matrix::cols).
		inline unsigned int getCols(void) const {return mCols;}
		
		//! Return number of columns.
		inline unsigned int cols(void) const {return mCols;}
		
		//! Return number of rows (this function is deprecated; use Matrix::rows).
		inline unsigned int getRows(void) const {return mRows;}
		
		//! Return number of rows.
		inline unsigned int rows(void) const {return mRows;}
		
		//! Add this matrix with scalar \c inScalar and return result through matrix \c outMatrix.		
		Matrix& add(Matrix& outMatrix, double inScalar) const;
		
		//! Add this matrix with matrix \c inMatrix and return result through matrix \c outMatrix.
		Matrix& add(Matrix& outMatrix, const Matrix& inMatrix) const;
		
		//! Return determinant of this matrix.
		double computeDeterminant(void) const;
		
		//! Compute eigenvalues and eigenvectors of a symetric matrix using the Triagonal QL method (matrix must be symetric).
		void computeEigens(Vector& outValues, Matrix& outVectors) const;
		
		//! Concatenate the columns of this matrix with those of matrix \c inMatrix, and return result through matrix \c outMatrix.
		Matrix& concatenateColumns(Matrix& outMatrix, const Matrix& inMatrix) const;
		
		//! Concatenate the rows of this matrix with those of matrix \c inMatrix, and return result through matrix \c outMatrix.
		Matrix& concatenateRows(Matrix& outMatrix, const Matrix& inMatrix) const;
		
		//! Extract from this matrix a sub-matrix defined by row range \c [inRow1,inRow2] and column range \c [inCol1,inCol2], return result through matrix \c outMatrix. 
		Matrix& extract(Matrix& outMatrix, unsigned int inRow1, unsigned int inRow2, unsigned int inCol1, unsigned int inCol2) const;
		
		//! Extract column \c inCol from this matrix and return it through matrix \c outMatrix.
		Matrix& extractColumn(Matrix& outVector, unsigned int inCol) const;
		
		//! Extract columns \c inCol1 through \c inCol2 from this matrix and return them through matrix \c outMatrix.
		Matrix& extractColumns(Matrix& outVector, unsigned int inCol1, unsigned int inCol2) const;
		
		//! Extract row \c inRow from this matrix and return it through matrix \c outMatrix. 
		Matrix& extractRow(Matrix& outVector, unsigned int inRow) const;
		
		//! Extract rows \c inRow1 through \c inRow2 from this matrix and return them through matrix \c outMatrix.
		Matrix& extractRows(Matrix& outVector, unsigned int inRow1, unsigned int inRow2) const;
		
		//! Return the inverse of this matrix.
		Matrix invert(void) const;
		
		//! Invert this matrix and return result through matrix \c outMatrix.
		Matrix& invert(Matrix& outMatrix) const;
		
		//! Find maximum of each column; return matrix with single row.
		Matrix& maxColumns(Matrix& outMatrix) const;
		
		//! Find maximum of each row; return matrix with single column.
		Matrix& maxRows(Matrix& outMatrix) const;
		
		//! Find minimum of each column; return matrix with single row.
		Matrix& minColumns(Matrix& outMatrix) const;
		
		//! Find minimum of each row; return matrix with single column.
		Matrix& minRows(Matrix& outMatrix) const;
		
		//! Multiply this matrix with scalar \c inScalar and return result through matrix \c outMatrix.
		Matrix& multiply(Matrix& outMatrix, double inScalar) const;
		
		//! Multiply this matrix with matrix \c inMatrix and return result through matrix \c outMatrix.
		Matrix& multiply(Matrix& outMatrix, const Matrix& inMatrix) const;
		
		//! Subtract this matrix with scalar \c inScalar and return result through matrix \c outMatrix.
		Matrix& subtract(Matrix& outMatrix, double inScalar) const;
		
		//! Subtract this matrix with matrix \c inMatrix and return result through matrix \c outMatrix.
		Matrix& subtract(Matrix& outMatrix, const Matrix& inMatrix) const;
		
		//! Compute sum of absolute values for each column; return matrix with single row.
		Matrix& sumAbsColumns(Matrix& outMatrix) const;
		
		//! Compute sum of absolute values for each row; return matrix with single column.
		Matrix& sumAbsRows(Matrix& outMatrix) const;
		
		//! Compute sum of each column; return matrix with single row.
		Matrix& sumColumns(Matrix& outMatrix) const;
		
		//! Compute sum of squares of each column; return matrix with single row.
		Matrix& sum2Columns(Matrix& outMatrix) const;
		
		//! Compute sum of each row; return matrix with single column.
		Matrix& sumRows(Matrix& outMatrix) const;
		
		//! Compute sum of squares each row; return matrix with single column.
		Matrix& sum2Rows(Matrix& outMatrix) const;
		
		//! Return the transpose of this matrix.
		Matrix transpose(void) const;
		
		//! Transpose this matrix and return result through matrix \c outMatrix.
		Matrix& transpose(Matrix& outMatrix) const;
		
		//! Resize matrix to \c inRows rows and \c inCols columns, while filing blanks with null values.
		void resize(unsigned int inRows, unsigned int inCols);
		
		//! Set this matrix to an identity matrix of size \c inSize.
		void setIdentity(unsigned int inSize);
		
		//! Return matrix name.
		inline const string& getName(void) const {return mName;}
		
		//! Set matrix name.
		inline void setName(const string& inName) {mName = inName;};
		
		//! Read this matrix from parse tree node \c inNode.
		string read(const XML::ConstIterator& inNode);
		
		//! Write this matrix into streamer \c outStream using tag name \c inTag.
		void write(XML::Streamer& outStream, const string& inTag="Matrix") const;
		
		//! Set output write precision to \c inPrecision number of digits (this method is deprecated in favor of Matrix::setOutputPrecision).
		void setPrecision(unsigned int inPrecision) {setOutputPrecision(inPrecision);}
		
		//! Set output write precision to \c inPrecision number of digits.
		void setOutputPrecision(unsigned int inPrecision) {mPrec = inPrecision;}
		
	 protected:
		unsigned int mRows; //!< Number of rows.
		unsigned int mCols; //!< Number of columns.
		unsigned int mPrec; //!< Output precision.
		string mName; //!< Name of matrix.
		
		//! Compute back substitution for the L-U decomposition.
		void computeBackSubLU(const vector<unsigned int>& inIndexes, Matrix& ioMatrixB) const;
		
		//! Compute L-U decomposition.
		void decomposeLU(vector<unsigned int>& outIndexes, int& outD);
		
		//! Return sqrt(a^2 + b^2) without under/overflow (used internally by method tql2).
		double hypot(double a, double b) const;
		
		//! Return in vector \c outScales the scaling values for the L-U decomposition.
		void scaleLU(vector<double>& outScales) const;
		
		//! Triagonalize matrix for computing eigensystem using QL method.
		void tql2(Vector& d, Vector& e, Matrix& V) const;

		//! Diagonalize matrix for computing eigensystem using QL method.
		void tred2(Vector& d, Vector& e, Matrix& V) const;
		
		//! Set matrix size to \c inRows rows and \c inCols columns; matrix content is lost.
		inline void setRowsCols(unsigned int inRows, unsigned int inCols) {
			mRows = inRows; mCols = inCols;
			vector<double>::resize(mRows*mCols);
		}
		
		//! Parse matrix elements in string \c inString.
		void parse(const string& inString);
		
		//! Serialize matrix elements.
		string serialize(void) const;
		
		//! Throw runtime error with message \c inMessage using parse tree node \c inNode.
		void throwError(const string& inMessage, const XML::ConstIterator& inNode) const;
		
	 private:
		// disabled methods
		Matrix& add(Vector&, double) const;
		Matrix& add(Vector&, const Matrix&) const;
		void computeEigens(Vector&, Vector&) const;
		Matrix& extract(Vector&, unsigned int, unsigned int, unsigned int, unsigned int) const;
		Matrix& extractColumns(Vector&, unsigned int, unsigned int) const;
		Matrix& extractRow(Vector&, unsigned int) const;
		Matrix& extractRows(Vector&, unsigned int, unsigned int) const;
		Matrix& invert(Vector&) const;
		Matrix& maxColumns(Vector& outMatrix) const;
		Matrix& minColumns(Vector& outMatrix) const;
		Matrix& subtract(Vector&, double) const;
		Matrix& subtract(Vector&, const Matrix&) const;
		Matrix& multiply(Vector&, double) const;
		Matrix& multiply(Vector&, const Matrix&) const;
		Matrix& sumAbsColumns(Vector& outMatrix);
		Matrix& sumColumns(Vector& outMatrix);
		Matrix& sum2Columns(Vector& outMatrix);
		Matrix& transpose(Vector&) const;
		
	};
	
	//! Add scalar by matrix.
	inline Matrix operator+(double inScalar, const Matrix& inMatrix) {return inMatrix+inScalar;}
	
	//! Subtract scalar by matrix.
	inline Matrix operator-(double inScalar, const Matrix& inMatrix) {return inMatrix-inScalar;}
	
	//! Multiply scalar by matrix.
	inline Matrix operator*(double inScalar, const Matrix& inMatrix) {return inMatrix*inScalar;}
	
	//! Insert matrix \c inMatrix into output stream \c outStream.
	ostream& operator<<(ostream& outStream, const Matrix& inMatrix);
	
	//! Extract matrix \c outMatrix from %XML document \c inDocument.	
	XML::Document& operator>>(XML::Document& inDocument, Matrix& outMatrix);	
	
}

#endif // PACC_Matrix_hpp

