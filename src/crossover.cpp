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


Crossover::~Crossover()
{};

TwoPointCrossover::TwoPointCrossover(const Randomizer& randomizer)
  : Crossover()
  , _generate(randomizer)
{}


TwoPointCrossover::TwoPointCrossover(const TwoPointCrossover& other)
  : Crossover(other)
  , _generate(other._generate)
{}


TwoPointCrossover::~TwoPointCrossover()
{}


void
validate(const Chromosome father, const Chromosome& mother)
{
  if (father.empty() or mother.empty()) {
    throw invalid_argument("Empty chromosome!");
  }
  
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

  Chromosome son(father);
  Chromosome daughter(father);
  
  Allele left = _generate.unsigned_int(0, father.size());
  Allele right = _generate.unsigned_int(left+1, father.size()+1);
      
  for(Allele index=0 ; index<father.size() ; ++index) {
    if (left <= index and index < right) {
      son[index] = mother[index];
      daughter[index] = father[index];
      
    } else {
      son[index] = father[index];
      daughter[index] = mother[index];
    }

  }

  children.push_back(son);
  children.push_back(daughter);
}
