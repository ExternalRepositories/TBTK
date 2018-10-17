/* Copyright 2017 Kristofer Björnson
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

/** @file ElectronFluctuationVertex.cpp
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Functions.h"
#include "TBTK/InteractionAmplitude.h"
#include "TBTK/Solver/ElectronFluctuationVertex.h"
#include "TBTK/UnitHandler.h"

#include <complex>
#include <iomanip>

using namespace std;

namespace TBTK{
namespace Solver{

ElectronFluctuationVertex::ElectronFluctuationVertex(
	const MomentumSpaceContext &momentumSpaceContext,
	const Property::Susceptibility &susceptibility
) :
	Communicator(true),
	momentumSpaceContext(momentumSpaceContext),
	susceptibility(susceptibility)
{
}

vector<complex<double>> ElectronFluctuationVertex::calculateSelfEnergyVertex(
	const Index &index
){
	unsigned int numEnergies;
	switch(susceptibility.getEnergyType()){
	case Property::EnergyResolvedProperty<complex<double>>::EnergyType::Real:
		numEnergies = susceptibility.getResolution();

		break;
	case Property::EnergyResolvedProperty<complex<double>>::EnergyType::BosonicMatsubara:
		numEnergies = susceptibility.getNumMatsubaraEnergies();

		break;
	default:
		TBTKExit(
			"Solver::ElectronFluctuationVertex::calculateSelfEnergyVertex()",
			"Unknow EnergyType.",
			""
		);
	}

	vector<complex<double>> selfEnergyVertex;
	selfEnergyVertex.reserve(numEnergies);
	for(
		unsigned int n = 0;
		n < numEnergies;
		n++
	){
		selfEnergyVertex.push_back(0.);
	}

	calculateSelfEnergyVertexMainAlgorithm(
		selfEnergyVertex,
		index,
		susceptibility,
		leftInteraction,
		rightInteraction,
		multiplier
	);

	return selfEnergyVertex;
}

void ElectronFluctuationVertex::calculateSelfEnergyVertexMainAlgorithm(
	vector<complex<double>> &selfEnergyVertex,
	const Index &index,
	const Property::Susceptibility &susceptibility,
	const vector<InteractionAmplitude> &uLeft,
	const vector<InteractionAmplitude> &uRight,
	double multiplier
){
	vector<Index> components = index.split();
	TBTKAssert(
		components.size() == 5,
		"Solver::ElectronFluctuationVertex::calculateSelfEnergyVertexMainAlgorithm()",
		"The Index must be a compound Index with 5 component Indices,"
		<< " but '" << components.size() << "' components supplied.",
		""
	);
	const Index kIndex = components[0];
	const Index intraBlockIndices[4] = {
		components[1],
		components[2],
		components[3],
		components[4]
	};
	for(unsigned int n = 0; n < 4; n++){
		TBTKAssert(
			intraBlockIndices[n].getSize() == 1,
			"Solver::ElectronFluctuationVertex::calculateSelfEnergyVertexMainAlgorithm()",
			"The four last components of the compound Index"
			<< " currently is restricted to have a single"
			<< " subindex, but component '" << n+1 << "' has '"
			<< intraBlockIndices[n].getSize() << "' subindices.",
			"Contact the developer if support for more general"
			<< " Indices is required."
		);
		//TODO
		//Rewrite code below to not depend on the intraBlockIndices
		//having a single subindex each. The remove this assert
		//statement.
	}

	for(unsigned int in = 0; in < uLeft.size(); in++){
		const InteractionAmplitude &incommingAmplitude = uLeft.at(in);
		complex<double> amplitude_i = incommingAmplitude.getAmplitude();
		int c0_i = incommingAmplitude.getCreationOperatorIndex(0).at(0);
		int c1_i = incommingAmplitude.getCreationOperatorIndex(1).at(0);
		int a0_i = incommingAmplitude.getAnnihilationOperatorIndex(0).at(0);
		int a1_i = incommingAmplitude.getAnnihilationOperatorIndex(1).at(0);

		if(
			a1_i != intraBlockIndices[3][0]
			|| c0_i != intraBlockIndices[2][0]
			|| abs(amplitude_i) < 1e-10
		){
			continue;
		}

		for(unsigned int out = 0; out < uRight.size(); out++){
			const InteractionAmplitude &outgoingAmplitude = uRight.at(out);
			complex<double> amplitude_o = outgoingAmplitude.getAmplitude();
			int c0_o = outgoingAmplitude.getCreationOperatorIndex(0).at(0);
			int c1_o = outgoingAmplitude.getCreationOperatorIndex(1).at(0);
			int a0_o = outgoingAmplitude.getAnnihilationOperatorIndex(0).at(0);
			int a1_o = outgoingAmplitude.getAnnihilationOperatorIndex(1).at(0);

			if(
				a0_o != intraBlockIndices[0][0]
				|| c1_o != intraBlockIndices[1][0]
				|| abs(amplitude_o) < 1e-10
			){
				continue;
			}

			const vector<complex<double>> &susceptibilityData
				= susceptibility.getData();
			unsigned int offsetChargeSusceptibility
				= susceptibility.getOffset({
					kIndex,
					{c0_o},
					{a1_o},
					{c1_i},
					{a0_i}
				});

			for(
				unsigned int n = 0;
				n < selfEnergyVertex.size();
				n++
			){
				selfEnergyVertex.at(n) += amplitude_i*amplitude_o*(
					susceptibilityData[
						offsetChargeSusceptibility + n
					]
				)*multiplier;
			}
		}
	}
}

}	//End of namespace Solver
}	//End of namesapce TBTK
