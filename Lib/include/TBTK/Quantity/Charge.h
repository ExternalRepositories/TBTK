/* Copyright 2019 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @package TBTKcalc
 *  @file Charge.h
 *  @brief Charge.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_QUANTITY_CHARGE
#define COM_DAFER45_TBTK_QUANTITY_CHARGE

#include "TBTK/Real.h"
#include "TBTK/TBTKMacros.h"

#include "TBTK/json.hpp"

namespace TBTK{
namespace Quantity{

/** @brief Charge.
 *
 *  A Charge is a Real value which implicitly is assumed to have units of
 *  charge. */
class Charge : public Real{
public:
	/** Default constructor. */
	Charge(){};

	/** Constructs a Quantity from a double. */
	Charge(double value) : Real(value){};

	/** Charge units (base unit):
	 * - kC - kilocoulomb
	 * - C - Coulomb
	 * - mC - millicoulomb
	 * - uC - microcoulomb
	 * - nC - nanocoulomb
	 * - pC - picocoulomb
	 * - fC - femtocoulomb
	 * - aC - attocoulomb
	 * - Te - terrae
	 * - Ge - gigae
	 * - Me - megae
	 * - ke - kiloe
	 * - e - e (elementary charge) */
	enum class Unit{kC, C, mC, uC, nC, pC, fC, aC, Te, Ge, Me, ke, e};
};

}; //End of namesapce Quantity
}; //End of namesapce TBTK

#endif
