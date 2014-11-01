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
 * \author Marc Parizeau and Christian Gagn&eacute;, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.11 $
 * $Date: 2005/09/19 06:10:42 $
 */

#include "Math/Matrix.hpp"
#include "Math/Vector.hpp"
#include "Util/StringFunc.hpp"
#include <stdexcept>
#include <cmath>

using namespace std;
using namespace PACC;

/*! 
This method also returns a reference to the result.
*/
Matrix& Matrix::add(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "add() invalid matrix!");
	outMatrix.setRowsCols(mRows, mCols);	
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] + inScalar;
	return outMatrix;
}

/*! 
This method also returns a reference to the result.
*/
Matrix& Matrix::add(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "add() invalid matrix!");
	PACC_AssertM(mRows == inMatrix.mRows && mCols == inMatrix.mCols, "add() matrix mismatch!");	
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
	PACC_AssertM(mRows > 0 && mCols > 0, "computeDeterminant() invalid matrix!");
	PACC_AssertM(mRows == mCols, "computeDeterminant() matrix not square!");
	Matrix lDecompose;
	vector<unsigned int> lIndexes(mRows);
	int lD;
	decomposeLU(lDecompose, lIndexes, lD);
	double lResult = lD;
	for(unsigned int i = 0; i < mRows; ++i) lResult *= lDecompose(i,i);
	return lResult;
}

/*!
*/
void Matrix::computeEigens(Vector& outValues, Matrix& outVectors) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "computeEigens() invalid matrix!");
	PACC_AssertM(mRows == mCols, "computeEigens() matrix not square!");
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
void Matrix::decomposeLU(Matrix& outDecompose, vector<unsigned int>& outIndexes, int& outD) const
{
	outD = 1;
	outDecompose = *this;
	vector<double> lScales;
	scaleLU(lScales);
	for(unsigned int j = 0; j < mCols; ++j) {
		for(unsigned int i = 0; i < j; ++i) {
			double lSum = outDecompose(i, j);
			for(unsigned int k = 0; k < i; ++k) lSum -= outDecompose(i,k) * outDecompose(k,j);
			outDecompose(i, j) = lSum;
		}
		double lMax = 0;
		unsigned int l = j;
		for(unsigned int i = j; i < mRows; ++i) {
			double lSum = outDecompose(i,j);
			for(unsigned int k = 0; k < j; ++k) lSum -= outDecompose(i,k) * outDecompose(k,j);
			outDecompose(i, j) = lSum;
			double lTmp = lScales[i] * fabs(lSum);
			if(lTmp >= lMax) {
				l = i;
				lMax = lTmp;
			}
		}
		if(j != l) {
			for(unsigned int k = 0; k < outDecompose.mCols; ++k) {
				double lTmp = outDecompose(l,k);
				outDecompose(l,k) = outDecompose(j,k);
				outDecompose(j,k) = lTmp;
			}
			outD = -outD;
			lScales[l] = lScales[j];
		}
		outIndexes[j] = l;
		if(outDecompose(j,j) == 0.0) outDecompose(j,j) = 1e-20;
		if(j != (mCols-1)) {
			double lDummy = 1.0 / outDecompose(j,j);
			for(unsigned int i = j+1; i < mRows; ++i) outDecompose(i,j) *= lDummy;
		}
	}
}

/*!
This method also returns a reference to the result.
*/
Matrix& Matrix::extract(Matrix& outMatrix, unsigned int inRow1, unsigned int inRow2, unsigned int inCol1, unsigned int inCol2) const
{
	PACC_AssertM(inRow1 <= inRow2 && inCol1 <= inCol2 && inRow2 < mRows && inCol2 < mCols, "extract() invalid indexes!");
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
Matrix& Matrix::extractRow(Matrix& outMatrix, unsigned int inRow) const
{
	return extract(outMatrix, inRow, inRow, 0, mCols-1);
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
	PACC_AssertM(mRows == mCols, "invert() matrix not square!");
	outMatrix = *this;
	Matrix lDecompose;
	vector<unsigned int> lIndexes(mRows);
	int lD;
	outMatrix.decomposeLU(lDecompose, lIndexes, lD);
	outMatrix.setIdentity(mRows);
	Matrix lB(mRows, 1);
	for(unsigned int j = 0; j < mCols; ++j) {
		for(unsigned int i = 0; i < mRows; ++i) lB(i,0) = outMatrix(i,j);
		lDecompose.computeBackSubLU(lIndexes, lB);
		for(unsigned int i = 0; i < mRows; ++i) outMatrix(i,j) = lB(i,0);
	}
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::multiply(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "multiply() invalid matrix!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] * inScalar;
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::multiply(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mCols == inMatrix.mRows, "multiply() matrix mismatch!");
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
Matrix elements must be enumerated in row order and delimited by either commas 
(','), semi-columns (';'), or white space. The recommended style is to seperate 
elements with comas, and rows with semi-columns. For example:
\verbatim
<Matrix name="My Matrix" rows="3" cols="4">1,2,3,4;5,6,7,8;9,10,11,12</Matrix>
\endverbatim
The number of elements must match the product of the "rows" and "cols" attributes.
*/
string Matrix::read(const XML::Iterator& inNode)
{
	if(!inNode) throw runtime_error("Matrix::read() nothing to read!");
	clear();
	for(XML::Iterator lChild = inNode->getFirstChild(); lChild; ++lChild) {
		if(lChild->getType() == XML::eString) {
			istringstream lStream(lChild->getValue());
			Tokenizer lTokenizer(lStream);
			lTokenizer.setDelimiters(" \n\r\t,;", "");
			string lToken;
			while(lTokenizer.getNextToken(lToken)) push_back(String::convertToFloat(lToken));
		}
	}
	mRows = String::convertToInteger(inNode->getAttribute("rows"));
	mCols = String::convertToInteger(inNode->getAttribute("cols"));
	if(vector<double>::size() != mRows*mCols) {
		throwError("Matrix::read() number of elements does not match the rows x cols attributes", inNode);
	}
	string lName = inNode->getAttribute("name");
	if(lName != "") mName = lName;
	return lName;
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
Matrix& Matrix::substract(Matrix& outMatrix, double inScalar) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "substract() invalid matrix!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] - inScalar;
	return outMatrix;
}

/*!
This method also returns a reference to the result.
 */
Matrix& Matrix::substract(Matrix& outMatrix, const Matrix& inMatrix) const
{
	PACC_AssertM(mRows > 0 && mCols > 0, "substract() invalid matrix!");
	PACC_AssertM(mRows == inMatrix.mRows && mCols == inMatrix.mCols, "substract() matrix mismatch!");
	outMatrix.setRowsCols(mRows, mCols);
	for(unsigned int i = 0; i < size(); ++i) outMatrix[i] = (*this)[i] - inMatrix[i];
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
		tst1 = std::max(tst1, std::abs(d[l]) + std::abs(e[l]));
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
	PACC_AssertM(mRows > 0 && mCols > 0, "transpose() invalid matrix!");
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
void Matrix::throwError(const string& inMessage, const XML::Iterator& inNode) const
{
	ostringstream lStream;
	lStream << inMessage << " for markup:\n";
	XML::Streamer lStreamer(lStream);
	inNode->serialize(lStreamer);
	throw runtime_error(lStream.str());
}

/*!
See Matrix::read for the write format.
*/
void Matrix::write(XML::Streamer& outStream, const string& inTag) const
{
	outStream.openTag(inTag, false);
	if(mName != "") outStream.insertAttribute("name", mName);
	outStream.insertAttribute("rows", mRows);
	outStream.insertAttribute("cols", mCols);
	ostringstream lContent;
	for(unsigned int i = 0; i < size(); ++i) {
		if(i != 0 && i % mCols == 0) lContent << ";";
		else if(i != 0) lContent << ",";
		lContent << (*this)[i];
	}
	outStream.insertStringContent(lContent.str());
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
corresponding tree root is then erased. Any read error throws a std::runtime_error.
*/
XML::Document& PACC::operator>>(XML::Document& inDocument, Matrix& outMatrix)
{
	XML::Iterator lNode = inDocument.getFirstDataTag();
	outMatrix.read(lNode);
	inDocument.erase(lNode);
	return inDocument;
}
