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

/*!\file PACC/SVG.hpp
 * \brief Include all headers for the %SVG drawing API.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.2 $
 * $Date: 2007/01/23 21:27:46 $
 */

/*!\namespace PACC::SVG
 * \brief Scalable vector graphics classes.
 * \ingroup SVG
 */

#include "PACC/SVG/Canvas.hpp"
#include "PACC/SVG/Frame.hpp"
#include "PACC/SVG/Path.hpp"
#include "PACC/SVG/Types.hpp"

/*!\defgroup SVG SVG: Scalable Vector Graphics
 * \brief Framework for basic SVG drawing.
 *
 * For example, the following code will produce a drawing with a 
 * rotated rectangle and output the SVG code at the console:
 * \code
 * #include "PACC/SVG.hpp"
 * #include <iostream>
 *
 * using namespace std;
 * using namespace PACC::SVG;
 * 
 * int main() {
 *    // create empty document in memory; no screen output
 *    Document lDocument("My title", Size(800,600));
 *    // create group; al elements will inherit the attributes
 *    Group lGroup(strokeColor(eBlue)+strokeWidth(11)+fillColor(eRed));
 *    // set transform that will apply to all group elements
 *    lGroup.setTransform(Rotate(45, Point(400,300)));
 *    // insert rectangle
 *    lGroup << Rectangle(Point(200, 200), Point(600, 400));
 *    // insert group into document
 *    lDocument << lGroup;
 *    // ouput valid svg document into stream
 *    cout << lDocument << endl;
 * }
 * \endcode
 *
 * This next example will produce a similar drawing, but with an ellipse,
 * and the output will be displayed on-screen, using user agent
 * SVGview:
 * \code
 * #include "PACC/SVG.hpp"
 * #include <iostream>
 *
 * using namespace std;
 * using namespace PACC::SVG;
 * 
 * int main() {
 *    // create canvas document that will pop a window on the user agent
 *    Canvas lDocument("My title", Size(800,600));
 *    Group lGroup(strokeColor(eBlue)+strokeWidth(11)+fillColor(eRed));
 *    lGroup.setTransform(Rotate(45, Point(400,300)));
 *    lGroup << Ellipse(Point(400, 300), 150, 100);
 *    // insert group and update screen display
 *    lDocument << lGroup;
 * }
 * \endcode
 */
