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


#include "chromosome.h"


using namespace std;


namespace xcsf
{

  class Crossover
  {
  public:
    Crossover();
    virtual ~Crossover();

    virtual void operator () (const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const = 0;
  
  };

  
  class TwoPointCrossover : public Crossover
  {
  public:
    TwoPointCrossover(unsigned int cut_point_A, unsigned int cut_point_B);
    TwoPointCrossover(const TwoPointCrossover& other);
    ~TwoPointCrossover();

    TwoPointCrossover& operator = (const TwoPointCrossover& other);
    bool operator == (const TwoPointCrossover& other) const;

    virtual void operator () (const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const;

  private: 
    unsigned int _cut_point_A;
    unsigned int _cut_point_B;

  };

  
}



#endif
