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
 * \file   PACC/Math/Matrix.cpp
 * \brief  Method definitions for class Matrix.
 * \author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et 
 syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.23 $
 * $Date: 2008/04/17 21:08:42 $
 */

#include "PACC/Math/Matrix.hpp"
#include "PACC/Math/Vector.hpp"
#include "PACC/Util/StringFunc.hpp"
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <climits>

using namespace std;
using namespace PACC;

/*! 
This method also returns a reference to the result.
*/
Matrix& Matrix::add(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::add() invalid of empty matrix!");
	outMatrix.setRowsCols(mRows, mCols);	
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] + inScalar;
	return outMatrix;
}

/*! 
This method also returns a reference to the result.
*/
Matrix& Matrix::add(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::add() invalid or empty matrix!");
	PACC_AssertM(mRows == inMatrix.mRows && mCols == inMatrix.mCols, "Matrix::add() matrix mismatch!");	
	outMatrix.setRowsCols(mRows, mCols);	
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] + inMatrix[i];
	return outMatrix;
}

/*!
*/
void Matrix::computeBackSubLU(const vector<unsigned int>& inIndexes, Matrix& ioMatrix) const
{
	unsigned int lII = UINT_MAX;
	for(unsigned int i = 0; i < mRows; ++i) {
		double lSum = ioMatrix(inIndexes[i], 0);
		ioMatrix(inIndexes[i], 0) = ioMatrix(i, 0);
		if(lII != UINT_MAX) {
			for(unsigned int j = lII; j < i; ++j) lSum -= (*this)(i,j) * ioMatrix(j, 0);
		} else if(lSum != 0.0) lII = i;
		ioMatrix(i, 0) = lSum;
	}
	for(unsigned int i = 0; i < mRows; ++i) {
		const unsigned int lR = mRows-i-1;
		double lSum = ioMatrix(lR,0);
		for(unsigned int j = lR+1; j < mCols; ++j) lSum -= (*this)(lR,j) * ioMatrix(j,0);
		ioMatrix(lR,0) = lSum / (*this)(lR,lR);
	}
}

/*!
*/
double Matrix::computeDeterminant(void) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::computeDeterminant() invalid or empty matrix!");
	PACC_AssertM(mRows == mCols, "Matrix::computeDeterminant() matrix not square!");
	Matrix lTmp = *this;
	vector<unsigned int> lIndexes(mRows);
	int lD;
	lTmp.decomposeLU(lIndexes, lD);
	double lResult = lD;
	for(unsigned int i = 0; i < mRows; ++i) lResult *= lTmp(i,i);
	return lResult;
}

/*!
*/
void Matrix::computeEigens(Vector& outValues, Matrix& outVectors) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::computeEigens() invalid or empty matrix!");
	PACC_AssertM(mRows == mCols, "Matrix::computeEigens() matrix not square!");
	outValues.resize(mRows);
	outVectors.resize(mRows, mCols);
	
	// Computer eigenvectors/eigenvalues using Triagonal QL method
	Vector lE(mRows);
	tred2(outValues, lE, outVectors);
	tql2(outValues, lE, outVectors);
	
	// Sort by eigenvalues.
	for(unsigned int j = 0; j < outValues.size(); ++j) {
		double lMax=outValues[j];
		unsigned int lMaxArg=j;
		for(unsigned int l = j+1; l<outValues.size(); ++l) {
			if(outValues[l] > lMax) {
				lMax=outValues[l];
				lMaxArg=l;
			}
		}
		if(lMaxArg != j) {
			for(unsigned int r = 0; r < outVectors.mRows; ++r) {
				double lTmp = outVectors(r,j);
				outVectors(r,j) = outVectors(r,lMaxArg);
				outVectors(r,lMaxArg) = lTmp;
			}
			double lTmp = outValues[j];
			outValues[j] = outValues[lMaxArg];
			outValues[lMaxArg] = lTmp;
		}
	}
}

/*!
*/
void Matrix::decomposeLU(vector<unsigned int>& outIndexes, int& outD)
{
	outD = 1;
	vector<double> lScales;
	scaleLU(lScales);
	for(unsigned int j = 0; j < mCols; ++j) {
		for(unsigned int i = 0; i < j; ++i) {
			double lSum = (*this)(i, j);
			for(unsigned int k = 0; k < i; ++k) lSum -= (*this)(i,k) * (*this)(k,j);
			(*this)(i, j) = lSum;
		}
		double lMax = 0;
		unsigned int l = j;
		for(unsigned int i = j; i < mRows; ++i) {
			double lSum = (*this)(i,j);
			for(unsigned int k = 0; k < j; ++k) lSum -= (*this)(i,k) * (*this)(k,j);
			(*this)(i, j) = lSum;
			double lTmp = lScales[i] * fabs(lSum);
			if(lTmp >= lMax) {
				l = i;
				lMax = lTmp;
			}
		}
		if(j != l) {
			for(unsigned int k = 0; k < (*this).mCols; ++k) {
				double lTmp = (*this)(l,k);
				(*this)(l,k) = (*this)(j,k);
				(*this)(j,k) = lTmp;
			}
			outD = -outD;
			lScales[l] = lScales[j];
		}
		outIndexes[j] = l;
		if((*this)(j,j) == 0.0) (*this)(j,j) = 1e-20;
		if(j != (mCols-1)) {
			double lDummy = 1.0 / (*this)(j,j);
			for(unsigned int i = j+1; i < mRows; ++i) (*this)(i,j) *= lDummy;
		}
	}
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::concatenateColumns(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows == inMatrix.mRows, "Matrix::concatenateColumns() matrix mismatch!");	
	if(&outMatrix != this) {
		// output matrix is not self assigning
		outMatrix.setRowsCols(mRows, mCols+inMatrix.mCols);
		// copy this matrix to output matrix
		for(unsigned int i = 0; i < mRows; ++i) {
			for(unsigned int j = 0; j <= mCols; ++j) {
				outMatrix(i,j) = (*this)(i,j);
			}
		}
	} else {
		// resize the output (this) matrix
		outMatrix.resize(mRows, mCols+inMatrix.mCols);
	}
	// concatenate input matrix to output matrix
	for(unsigned int i = 0; i < inMatrix.mRows; ++i) {
		for(unsigned int j = 0; j <= inMatrix.mCols; ++j) {
			outMatrix(i,j+mCols) = inMatrix(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::concatenateRows(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows == inMatrix.mRows, "Matrix::concatenateRows() matrix mismatch!");	
	if(&outMatrix != this) {
		// output matrix is not self assigning
		outMatrix.setRowsCols(mRows+inMatrix.mRows, mCols);
		// copy this matrix to output matrix
		for(unsigned int i = 0; i < mRows; ++i) {
			for(unsigned int j = 0; j <= mCols; ++j) {
				outMatrix(i,j) = (*this)(i,j);
			}
		}
	} else {
		// resize the output (this) matrix
		outMatrix.resize(mRows+inMatrix.mRows, mCols);
	}
	// concatenate input matrix to output matrix
	for(unsigned int i = 0; i < inMatrix.mRows; ++i) {
		for(unsigned int j = 0; j <= inMatrix.mCols; ++j) {
			outMatrix(i+mRows,j) = inMatrix(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
*/
Matrix& Matrix::extract(Matrix& outMatrix, unsigned int inRow1, unsigned int inRow2, unsigned int inCol1, unsigned int inCol2) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::extract() invalid or empty matrix!");
	PACC_AssertM(inRow1 <= inRow2 && inCol1 <= inCol2 && inRow2 < mRows && inCol2 < mCols, "Matrix::extract() invalid indexes!");
	if(&outMatrix != this) {
		// output matrix is not self assigning
		outMatrix.setRowsCols(inRow2-inRow1+1, inCol2-inCol1+1);
		for(unsigned int i = inRow1; i <= inRow2; ++i) {
			for(unsigned int j = inCol1; j <= inCol2; ++j) {
				outMatrix(i-inRow1,j-inCol1) = (*this)(i,j);
			}
		}
	} else {
		// use temporary matrix to self assign
		Matrix lMatrix(*this);
		outMatrix.setRowsCols(inRow2-inRow1+1, inCol2-inCol1+1);
		for(unsigned int i = inRow1; i <= inRow2; ++i) {
			for(unsigned int j = inCol1; j <= inCol2; ++j) {
				outMatrix(i-inRow1,j-inCol1) = lMatrix(i,j);
			}
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::extractColumn(Matrix& outMatrix, unsigned int inCol) const
{
	return extract(outMatrix, 0, mRows-1, inCol, inCol);
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::extractColumns(Matrix& outMatrix, unsigned int inCol1, unsigned int inCol2) const
{
	return extract(outMatrix, 0, mRows-1, inCol1, inCol2);
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::extractRow(Matrix& outMatrix, unsigned int inRow) const
{
	return extract(outMatrix, inRow, inRow, 0, mCols-1);
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::extractRows(Matrix& outMatrix, unsigned int inRow1, unsigned int inRow2) const
{
	return extract(outMatrix, inRow1, inRow2, 0, mCols-1);
}

/*!
 */
double Matrix::hypot(double a, double b) const
{
	double r;
	if(abs(a) > abs(b)) {
		r = b/a;
		r = abs(a)*sqrt(1+r*r);
	}
	else if(b != 0) {
		r = a/b;
		r = abs(b)*sqrt(1+r*r);
	}
	else {
		r = 0.0;
	}
	return r;
}

/*!
*/
Matrix Matrix::invert(void) const 
{
	Matrix lMatrix; 
	return invert(lMatrix);
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::invert(Matrix& outMatrix) const
{
	PACC_AssertM(mRows == mCols, "Matrix::invert() matrix not square!");
	Matrix lTmp = *this;
	vector<unsigned int> lIndexes(mRows);
	int lD;
	lTmp.decomposeLU(lIndexes, lD);
	outMatrix.setIdentity(mRows);
	Matrix lB(mRows, 1);
	for(unsigned int j = 0; j < mCols; ++j) {
		for(unsigned int i = 0; i < mRows; ++i) lB(i,0) = outMatrix(i,j);
		lTmp.computeBackSubLU(lIndexes, lB);
		for(unsigned int i = 0; i < mRows; ++i) outMatrix(i,j) = lB(i,0);
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::maxColumns(Matrix& outMatrix) const
{
	extractRow(outMatrix, 0);
	for(unsigned int i = 1; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			if(outMatrix(0, j) < (*this)(i,j)) outMatrix(0,j) = (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::maxRows(Matrix& outMatrix) const
{
	extractColumn(outMatrix, 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 1; j < getCols(); ++j) {
			if(outMatrix(i, 0) < (*this)(i,j)) outMatrix(i, 0) = (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::minColumns(Matrix& outMatrix) const
{
	extractRow(outMatrix, 0);
	for(unsigned int i = 1; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			if(outMatrix(0, j) > (*this)(i,j)) outMatrix(0, j) = (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::minRows(Matrix& outMatrix) const
{
	extractColumn(outMatrix, 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 1; j < getCols(); ++j) {
			if(outMatrix(i, 0) > (*this)(i,j)) outMatrix(i, 0) = (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::multiply(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::multiply() invalid or empty matrix!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] * inScalar;
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::multiply(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mCols == inMatrix.mRows, "Matrix::multiply() matrix mismatch!");
	if(&outMatrix != this && &outMatrix != &inMatrix) {
		// output matrix is neither left or right matrix (no self assigment)
		outMatrix.setRowsCols(mRows, inMatrix.mCols);
		for(unsigned int i = 0; i < outMatrix.mRows; ++i) {
			for(unsigned int j = 0; j < outMatrix.mCols; ++j) {
				outMatrix(i,j) = 0;
				for(unsigned int k = 0; k < mCols; ++k) {
					outMatrix(i,j) += (*this)(i,k) * inMatrix(k,j);
				}
			}
		}
	} else if(&outMatrix == this && &outMatrix != &inMatrix) {
		// use temporary matrix to self assign with left matrix
		Matrix lMatrix(*this);
		outMatrix.setRowsCols(mRows, inMatrix.mCols);
		for(unsigned int i = 0; i < outMatrix.mRows; ++i) {
			for(unsigned int j = 0; j < outMatrix.mCols; ++j) {
				outMatrix(i,j) = 0;
				for(unsigned int k = 0; k < mCols; ++k) {
					outMatrix(i,j) += lMatrix(i,k) * inMatrix(k,j);
				}
			}
		}
	} else if(&outMatrix != this && &outMatrix == &inMatrix) {
		// use temporary matrix to self assign with right matrix
		Matrix lMatrix(inMatrix);
		outMatrix.setRowsCols(mRows, inMatrix.mCols);
		for(unsigned int i = 0; i < outMatrix.mRows; ++i) {
			for(unsigned int j = 0; j < outMatrix.mCols; ++j) {
				outMatrix(i,j) = 0;
				for(unsigned int k = 0; k < mCols; ++k) {
					outMatrix(i,j) += (*this)(i,k) * lMatrix(k,j);
				}
			}
		}
	} else {
		// use temporary matrix to self assign with both left and right matrices
		Matrix lMatrix(*this);
		outMatrix.setRowsCols(mRows, inMatrix.mCols);
		for(unsigned int i = 0; i < outMatrix.mRows; ++i) {
			for(unsigned int j = 0; j < outMatrix.mCols; ++j) {
				outMatrix(i,j) = 0;
				for(unsigned int k = 0; k < mCols; ++k) {
					outMatrix(i,j) += lMatrix(i,k) * lMatrix(k,j);
				}
			}
		}
	}
	return outMatrix;
}

/*!
 This method parses the input string for matrix elements. Elements should be in 
 row order, separated by either comas (',') or white space. Rows should be separated
 by semi-columns (';'). See method Matrix::read for more details.
 
 */
void Matrix::parse(const string& inString)
{
	clear();
	mRows = mCols = 0;
	istringstream lStream(inString);
	Tokenizer lTokenizer(lStream);
	lTokenizer.setDelimiters(" \n\r\t,", ";");
	string lToken;
	unsigned int lColumn = 0;
	bool lFinished = false;
	while(!lFinished) {
		if(!lTokenizer.getNextToken(lToken)) {
			lFinished = true;
			lToken = ";";
		}
		if(lToken == ";") {
			// either end of string or end of row
			if(lColumn > 0 && mCols == 0) {
				// this token marks the end of the first row
				mCols = lColumn;
				++mRows;
			} else if(lColumn > 0 && lColumn == mCols) {
				// the current row is valid
				++mRows;
			} else if(lColumn != mCols) {
				// rows don't have the same number of columns
				XML::Node lNode(inString, XML::eString);
				throwError("Matrix::parse() invalid format, variable number of columns!", &lNode);
			}
			lColumn = 0;
		} else {
			push_back(String::convertToFloat(lToken));
			++lColumn;
		}
	}
}

/*!
 Two types of input nodes are supported: either XML::eData or XML::eString.

 In the first case, the input node is any valid data tag that embeds a string 
 which enumerates the matrix elements. These must be enumerated in row order with 
 row elements delimited by commas (',') or white space, and rows separated by 
 semi-columns (';'). For example, the following defines a 3x4 matrix:
 \verbatim
 <Matrix name="My Matrix" rows="3" cols="4">1,2,3,4;5,6,7,8;9,10,11,12</Matrix>
 \endverbatim
 Note that the tag name (here "Matrix") is irrelevant; any name can be used. The 
 method either returns the value of the "name" attribute, if present, or an empty 
 string, otherwise. Attributes "rows" and "cols" are optional, but must be 
 coherent with the parsed matrix, if present. Otherwise, an std::runtime_error 
 exception will be raised.
 
 In the second case, matrix elements are directly enumerated in a string, in row 
 order, with row elements separated by comas or white space, and rows separated 
 by semi-columns. For example, the following also defines a valid 3x4 matrix:
 \verbatim
 1,2,3,4;5,6,7,8;9,10,11,12
 \endverbatim
 In this case, however, the method always returns an empty string.
 
 Any parse error raises an std::runtime_error exception.
 */
string Matrix::read(const XML::ConstIterator& inNode)
{
	if(!inNode) throw runtime_error("Matrix::read() nothing to read!");
	if(inNode->getType() == XML::eData) {
		// read first format
		bool lStringParsed = false;
		for(XML::ConstIterator lChild = inNode->getFirstChild(); lChild; ++lChild) {
			if(lChild->getType() == XML::eString) {
				if(lStringParsed) 
					throwError("Matrix::read() invalid format, matrix contains multiple strings!", inNode);
				parse(lChild->getValue());
				lStringParsed = true;
			}
		}
		if(inNode->isDefined("rows")) {
			// validate number of rows
			int lRows = String::convertToInteger(inNode->getAttribute("rows"));
			if((int)mRows != lRows) throwError("Matrix::read() invalid 'rows' attribute!", inNode);
		}
		if(inNode->isDefined("cols")) {
			// validate number of cols
			int lCols = String::convertToInteger(inNode->getAttribute("cols"));
			if((int)mCols != lCols) throwError("Matrix::read() invalid 'cols' attribute!", inNode);
		}
	} else if(inNode->getType() == XML::eString) {
		// read second format
		parse(inNode->getValue());
	} else {
		// unsupported node type
		throwError("Matrix::read() node type must be XML::eData or XML::eString!", inNode);
	}
	if(inNode->isDefined("name")) mName = inNode->getAttribute("name");
	return mName;
}

/*!
*/
void Matrix::resize(unsigned int inRows, unsigned int inCols)
{
	Matrix lMat(*this);
	setRowsCols(inRows, inCols);
	for(unsigned int i = 0; i < mRows; ++i) {
		for(unsigned int j = 0; j < mCols; ++j) {
			(*this)(i,j) = (i < lMat.mRows && j < lMat.mCols ? lMat(i,j) : 0.);
		}
	}
}

/*!
*/
void Matrix::scaleLU(vector<double>& outScales) const
{
	outScales.resize(mCols);
	for(unsigned int i = 0; i < mRows; ++i) {
		double lMax = 0.;
		for(unsigned int j = 0; j < mCols; ++j) {
			const double lTmp = fabs((*this)(i,j));     
			if(lTmp > lMax) lMax=lTmp;
		}
		if(lMax == 0.) throw runtime_error("<Matrix::scaleLU> matrix is singular!");
		outScales[i] = 1./lMax;
	}
}

/*!
This method enumerates matrix elements matrix elements in row order, with row 
 elements separated by comas or white space, and rows separated by semi-columns. 
 For example, the following defines a 3x4 matrix:
 \verbatim
 1,2,3,4;5,6,7,8;9,10,11,12
 \endverbatim
 By default, the output precision of the matrix elements can be set with method 
 Matrix::setOutputPrecision.
 */
string Matrix::serialize(void) const
{
	ostringstream lContent;
	lContent.precision(mPrec);
	for(unsigned int i = 0; i < size(); ++i) {
		if(i != 0 && i % mCols == 0) lContent << ";";
		else if(i != 0) lContent << ",";
		lContent << (*this)[i];
	}
	return lContent.str();
}

/*!
*/
void Matrix::setIdentity(unsigned int inSize)
{
	setRowsCols(inSize, inSize);
	for(unsigned int j = 0; j < mCols; ++j) {
		for(unsigned int i = 0; i < mRows; ++i) {
			(*this)(i,j) = (i == j ? 1 : 0);
		}
	}
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::subtract(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::subtract() invalid or empty matrix!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] - inScalar;
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::subtract(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::subtract() invalid or empty matrix!");
	PACC_AssertM(mRows == inMatrix.mRows && mCols == inMatrix.mCols, "Matrix::subtract() matrix mismatch!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] - inMatrix[i];
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sumAbsColumns(Matrix& outMatrix) const
{
	outMatrix = Matrix(1, getCols(), 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(0, j) += fabs((*this)(i,j));
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sumAbsRows(Matrix& outMatrix) const
{
	outMatrix = Matrix(getRows(), 1, 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(i, 0) += fabs((*this)(i,j));
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sumColumns(Matrix& outMatrix) const
{
	outMatrix = Matrix(1, getCols(), 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(0, j) += (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sum2Columns(Matrix& outMatrix) const
{
	outMatrix = Matrix(1, getCols(), 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(0, j) += (*this)(i,j) * (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sumRows(Matrix& outMatrix) const
{
	outMatrix = Matrix(getRows(), 1, 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(i, 0) += (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::sum2Rows(Matrix& outMatrix) const
{
	outMatrix = Matrix(getRows(), 1, 0);
	for(unsigned int i = 0; i < getRows(); ++i) {
		for(unsigned int j = 0; j < getCols(); ++j) {
			outMatrix(i, 0) += (*this)(i,j) * (*this)(i,j);
		}
	}
	return outMatrix;
}

/*!
 *  \param d Real part of eigenvalues computed from the matrix.
 *  \param e Imaginary part of eigenvalues computed from the matrix.
 *  \param V Eigenvectors computed from the matrix.
 *
 *  This method is derived from procedure tql2 of the Java package JAMA,
 *  which is itself derived from the Algol procedures tql2, by
 *  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
 *  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
 *  Fortran subroutine in EISPACK.
 */
void Matrix::tql2(Vector& d, Vector& e, Matrix& V) const
{
	const unsigned int n=mRows;
	for(unsigned int i = 1; i < n; i++) e[i-1] = e[i];
	e[n-1] = 0.0;
	
	double f = 0.0;
	double tst1 = 0.0;
	double eps = std::pow(2.0,-52.0);
	for(unsigned int l = 0; l < n; l++) {
		// Find small subdiagonal element
		tst1 = max(tst1, abs(d[l]) + abs(e[l]));
		unsigned int m=l;
		while((m+1) < n) {
			if(std::abs(e[m]) <= eps*tst1) break;
			m++;
		}
		
		// If m == l, d[l] is an eigenvalue,
		// otherwise, iterate.
		if(m > l) {
			unsigned int iter = 0;
			do {
				iter = iter + 1;  // (Could check iteration count here.)
													// Compute implicit shift
				double g = d[l];
				double p = (d[l+1] - g) / (2.0 * e[l]);
				double r = hypot(p,1.0);
				if(p < 0) r = -r;
				d[l] = e[l] / (p + r);
				d[l+1] = e[l] * (p + r);
				double dl1 = d[l+1];
				double h = g - d[l];
				for(unsigned int i = l+2; i < n; i++) d[i] -= h;
				f = f + h;
				
				// Implicit QL transformation.
				p = d[m];
				double c = 1.0;
				double c2 = c;
				double c3 = c;
				double el1 = e[l+1];
				double s = 0.0;
				double s2 = 0.0;
				for(unsigned int i = m-1; i >= l; i--) {
					c3 = c2;
					c2 = c;
					s2 = s;
					g = c * e[i];
					h = c * p;
					r = hypot(p,e[i]);
					e[i+1] = s * r;
					s = e[i] / r;
					c = p / r;
					p = c * d[i] - s * g;
					d[i+1] = h + s * (c * g + s * d[i]);
					
					// Accumulate transformation.
					for(unsigned int k = 0; k < n; k++) {
						h = V(k,i+1);
						V(k,i+1) = s * V(k,i) + c * h;
						V(k,i) = c * V(k,i) - s * h;
					}
					if(i == 0) break;
				}
				p = -s * s2 * c3 * el1 * e[l] / dl1;
				e[l] = s * p;
				d[l] = c * p;
				
				// Check for convergence.
			} while (std::abs(e[l]) > eps*tst1);
		}
		d[l] = d[l] + f;
		e[l] = 0.0;
	}
}

/*!
 *  \param d Real part of eigenvalues computed from the matrix.
 *  \param e Imaginary part of eigenvalues computed from the matrix.
 *  \param V Eigenvectors computed from the matrix.
 *
 *  This method is derived from procedure tred2 of the Java package JAMA,
 *  which is itself derived from the Algol procedures tred2, by
 *  Bowdler, Martin, Reinsch, and Wilkinson, Handbook for
 *  Auto. Comp., Vol.ii-Linear Algebra, and the corresponding
 *  Fortran subroutine in EISPACK.
 */
void Matrix::tred2(Vector& d, Vector& e, Matrix& V) const
{
	const unsigned int n=mRows;
	V = *this;
	for(unsigned int j = 0; j < n; ++j) d[j] = V(n-1,j);
	
	// Householder reduction to tridiagonal form.
	for(unsigned int i = n-1; i > 0; --i) {
		
		// Scale to avoid under/overflow.
		double scale = 0.0;
		double h = 0.0;
		for(unsigned int k = 0; k < i; ++k) scale += abs(d[k]);
		if(scale == 0.0) {
			e[i] = d[i-1];
			for(unsigned int j = 0; j < i; ++j) {
				d[j] = V(i-1,j);
				V(i,j) = 0.0;
				V(j,i) = 0.0;
			}
		} else {
			// Generate Householder vector.
			for(unsigned int k=0; k<i; ++k) {
				d[k] /= scale;
				h += d[k] * d[k];
			}
			double f = d[i-1];
			double g = sqrt(h);
			if(f > 0.0) g = -g;
			e[i] = scale * g;
			h = h - f * g;
			d[i-1] = f - g;
			for(unsigned int j = 0; j < i; j++) e[j] = 0.0;
			
			// Apply similarity transformation to remaining columns.
			for(unsigned int j = 0; j < i; j++) {
				f = d[j];
				V(j,i) = f;
				g = e[j] + V(j,j) * f;
				for(unsigned int k = j+1; k <= i-1; k++) {
					g += V(k,j) * d[k];
					e[k] += V(k,j) * f;
				}
				e[j] = g;
			}
			f = 0.0;
			for(unsigned int j = 0; j < i; j++) {
				e[j] /= h;
				f += e[j] * d[j];
			}
			double hh = f / (h + h);
			for(unsigned int j=0; j<i; j++) e[j] -= hh * d[j];
			for(unsigned int j=0; j<i; j++) {
				f = d[j];
				g = e[j];
				for(unsigned int k = j; k <= i-1; k++) V(k,j) -= (f * e[k] + g * d[k]);
				d[j] = V(i-1,j);
				V(i,j) = 0.0;
			}
		}
		d[i] = h;
	}
	
	// Accumulate transformations.
	for(unsigned int i = 0; i < n-1; i++) {
		V(n-1,i) = V(i,i);
		V(i,i) = 1.0;
		double h = d[i+1];
		if(h!=0.0) {
			for(unsigned int k=0; k<=i; k++) d[k] = V(k,i+1) / h;
			for(unsigned int j=0; j<=i; j++) {
				double g = 0.0;
				for(unsigned int k=0; k<=i; k++) g += V(k,i+1) * V(k,j);
				for(unsigned int k=0; k<=i; k++) V(k,j) -= g * d[k];
			}
		}
		for(unsigned int k=0; k<=i; k++) V(k,i+1) = 0.0;
	}
	for(unsigned int j=0; j<n; j++) {
		d[j] = V(n-1,j);
		V(n-1,j) = 0.0;
	}
	V(n-1,n-1) = 1.0;
	e[0] = 0.0;
}

/*!
*/
Matrix Matrix::transpose(void) const 
{
	Matrix lMatrix; 
	return transpose(lMatrix);
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::transpose(Matrix& outMatrix) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "Matrix::transpose() invalid or empty matrix!");
	if(&outMatrix != this) {
		// output matrix is not self assigning
		outMatrix.setRowsCols(mCols,mRows);
		// transpose elements
		for(unsigned int i = 0; i< mRows; ++i) {
			for(unsigned int j = 0; j < mCols; ++j) {
				outMatrix(j,i) = (*this)(i,j);
			}
		}
	} else {
		// use temporary matrix to self assign
		Matrix lMatrix(*this);
		outMatrix.setRowsCols(mCols,mRows);
		// transpose elements
		for(unsigned int i = 0; i< mRows; ++i) {
			for(unsigned int j = 0; j < mCols; ++j) {
				outMatrix(j,i) = lMatrix(i,j);
			}
		}
	}		
	return outMatrix;
}

/*!
*/
void Matrix::throwError(const string& inMessage, const XML::ConstIterator& inNode) const
{
	ostringstream lStream;
	lStream << inMessage << " for markup:\n";
	XML::Streamer lStreamer(lStream);
	if(inNode) inNode->serialize(lStreamer);
	throw runtime_error(lStream.str());
}

/*!
The following markup illustrates the write format for a 3x4 matrix:
 \verbatim
 <Matrix name="My Matrix" rows="3" cols="4">1,2,3,4;5,6,7,8;9,10,11,12</Matrix>
 \endverbatim
 when using the default tag name \c inTag="Matrix". The output precision of the 
 matrix elements can be set using method Matrix::setOutputPrecision.
*/
void Matrix::write(XML::Streamer& outStream, const string& inTag) const
{
	outStream.openTag(inTag, false);
	if(mName != "") outStream.insertAttribute("name", mName);
	outStream.insertAttribute("rows", mRows);
	outStream.insertAttribute("cols", mCols);
	outStream.insertStringContent(serialize());
	outStream.closeTag();
}

/*!
*/
ostream& PACC::operator<<(ostream &outStream, const Matrix& inMatrix)
{
	XML::Streamer lStream(outStream);
	inMatrix.write(lStream);
	return outStream;
}

/*!
This method uses the first data tag of the parse tree to read the matrix. The 
corresponding subtree is then erased. Any read error throws a std::runtime_error.
*/
XML::Document& PACC::operator>>(XML::Document& inDocument, Matrix& outMatrix)
{
	XML::Iterator lNode = inDocument.getFirstDataTag();
	outMatrix.read(lNode);
	inDocument.erase(lNode);
	return inDocument;
}
