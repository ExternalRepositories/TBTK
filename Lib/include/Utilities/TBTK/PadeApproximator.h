/* Copyright 2018 Kristofer Björnson
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
 *  @file Polynomial.h
 *  @brief Class for storing polynomial expressions.
 *
 *  @author Kristofer Björnson
 */

#ifndef COM_DAFER45_TBTK_PADE_APPROXIMATOR
#define COM_DAFER45_TBTK_PADE_APPROXIMATOR

#include "TBTK/Polynomial.h"
#include "TBTK/TBTKMacros.h"

#include <complex>
#include <vector>

namespace TBTK{

class PadeApproximator{
public:
	void setNumeratorDegree(unsigned int numeratorDegree);

	void setDenumeratorDegree(unsigned int numeratorDegree);

	std::vector<
		Polynomial<std::complex<double>, std::complex<double>, int>
	> approximate(
		const std::vector<std::complex<double>> &values,
		const std::vector<std::complex<double>> &arguments
	);
private:
	unsigned int numeratorDegree;

	unsigned int denumeratorDegree;

	void executeLeastSquare(
		std::complex<double> *matrix,
		std::complex<double> *vector,
		unsigned int numRows,
		unsigned int numColumns
	);
};

inline void PadeApproximator::setNumeratorDegree(unsigned int numeratorDegree){
	this->numeratorDegree = numeratorDegree;
}

inline void PadeApproximator::setDenumeratorDegree(
	unsigned int denumeratorDegree
){
	this->denumeratorDegree = denumeratorDegree;
}

}; //End of namespace TBTK

#endif
