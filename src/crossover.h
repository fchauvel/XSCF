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


#ifndef XCSF_CROSSOVER_H
#define XCSF_CROSSOVER_H


#include "utils.h"
#include "chromosome.h"


using namespace std;


namespace xcsf
{

  class Crossover
  {
  public:
    virtual ~Crossover();
    
    virtual void operator () (const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const = 0;
  
  };

  
  class TwoPointCrossover : public Crossover
  {
  public:
    TwoPointCrossover(const Randomizer& randomizer);
    TwoPointCrossover(const TwoPointCrossover& other);
    ~TwoPointCrossover();

    virtual void operator () (const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const;

  private:
    const Randomizer& _generate;

  };

  
}



#endif
