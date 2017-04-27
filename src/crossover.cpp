/*
 * This file is part of XCSF.
 *
 * XCSF is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XCSF is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XCSF.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "crossover.h"

#include <iostream>
#include <sstream>
#include <stdexcept>


using namespace std;
using namespace xcsf;


Crossover::Crossover()
{}


Crossover::~Crossover()
{}


TwoPointCrossover::TwoPointCrossover(unsigned int cut_point_A, unsigned int cut_point_B)
  : Crossover()
  , _cut_point_A(cut_point_A)
  , _cut_point_B(cut_point_B)
{
  if (_cut_point_A >= _cut_point_B) {
    ostringstream error;
    error << "Invalid cut-points! Left cut-point must be smaller than the right one";
    throw invalid_argument(error.str());
  }
}


TwoPointCrossover::TwoPointCrossover(const TwoPointCrossover& other)
  : Crossover(other)
  , _cut_point_A(other._cut_point_A)
  , _cut_point_B(other._cut_point_B)
{}


TwoPointCrossover::~TwoPointCrossover()
{}


TwoPointCrossover& 
TwoPointCrossover::operator = (const TwoPointCrossover& other)
{
  _cut_point_A = other._cut_point_A;
  _cut_point_B = other._cut_point_B;
  return *this;
}


bool
TwoPointCrossover::operator == (const TwoPointCrossover& other) const
{
  return _cut_point_A == other._cut_point_A
    and _cut_point_B == other._cut_point_B;
}


void
validate(const Chromosome father, const Chromosome& mother)
{ 
  if (father.size() != mother.size()) {
    ostringstream error;
    error << "Parents chromosomes have different length!"
	  << "(father=" << father.size() << ", but mother="
	  << mother.size() << ")";
    throw invalid_argument(error.str());
  }  
}


void
TwoPointCrossover::operator()(const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const
{
  validate(father, mother);

  if (_cut_point_B >= father.size()) {
    throw invalid_argument("The chromosomes are too short for the selected cut_points");
  }
  
  if (children.size() < 2) {
    throw invalid_argument("TwoPointCrossovers generates at two children!");
  }
  
  for(unsigned int index=0 ; index<father.size() ; ++index) {
    if (_cut_point_A <= index and index < _cut_point_B) {
      children[0].push_back(mother[index]);
      children[1].push_back(father[index]);
    } else {
      children[0].push_back(father[index]);
      children[1].push_back(mother[index]);
    }
  }
}
