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
 * \file PACC/Threading.hpp
 * \brief Include all headers for the multithreading module.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.17 $
 * $Date: 2007/02/24 19:29:55 $
 */

/*!
 * \namespace PACC::Threading
 * \brief Portable thread management classes.
 */

/*!
 * \defgroup Threading Threading: portable thread management
 * \brief Framework for multithreaded programming. 
 */

#include "PACC/Threading/Condition.hpp"
#include "PACC/Threading/Mutex.hpp"
#include "PACC/Threading/Semaphore.hpp"
#include "PACC/Threading/Thread.hpp"
#include "PACC/Threading/ThreadPool.hpp"
#include "PACC/Threading/TLS.hpp"
