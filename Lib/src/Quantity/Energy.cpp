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

/** @file Energy.cpp
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Quantity/Energy.h"

using namespace std;

namespace TBTK{
namespace Quantity{

map<Energy::Unit, string> Energy::unitToString = {
	{Energy::Unit::GeV, "GeV"},
	{Energy::Unit::MeV, "MeV"},
	{Energy::Unit::keV, "keV"},
	{Energy::Unit::eV, "eV"},
	{Energy::Unit::meV, "meV"},
	{Energy::Unit::ueV, "ueV"},
	{Energy::Unit::J, "J"}
};
map<string, Energy::Unit> Energy::stringToUnit = {
	{"GeV", Energy::Unit::GeV},
	{"MeV", Energy::Unit::MeV},
	{"keV", Energy::Unit::keV},
	{"eV", Energy::Unit::eV},
	{"meV", Energy::Unit::meV},
	{"ueV", Energy::Unit::ueV},
	{"J", Energy::Unit::J}
};

string Energy::getUnitString(Unit unit){
	try{
		return unitToString.at(unit);
	}
	catch(std::out_of_range e){
		TBTKExit(
			"Quantity::Energy::getUnitString()",
			"Unkown unit '" << static_cast<int>(unit) << "'.",
			"This should never happen, contact the developer."
		);
	}
}

Energy::Unit Energy::getUnit(const string &str){
	try{
		return stringToUnit.at(str);
	}
	catch(std::out_of_range e){
		TBTKExit(
			"Quantity::Energy::getUnit()",
			"Unkown unit '" << str << "'.",
			""
		);
	}
}

double Energy::getConversionFactor(Unit unit){
	constexpr double J_per_eV = 1.602176634e-19;
	switch(unit){
		case Unit::GeV:	//1e-9 GeV per eV
			return 1e-9;
		case Unit::MeV:
			return 1e-6;
		case Unit::keV:
			return 1e-3;
		case Unit::eV:	//Reference scale
			return 1.;
		case Unit::meV:
			return 1e3;
		case Unit::ueV:
			return 1e6;
		case Unit::J:
			return J_per_eV;
		default:
			TBTKExit(
				"Quantity::Energy::getConversionFactor()",
				"Unknown unit - " << static_cast<int>(unit)
				<< ".",
				"This should never happen, contact the"
				<< " developer."
			);
	}
}

};	//End of namespace Quantity
};	//End of namespace TBTK