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

#ifndef XCSF_EVOLUTION_H
#define XCSF_EVOLUTION_H


#include <vector>
#include "rule.h"


using namespace std;


namespace xcsf {

  typedef unsigned int Allele;
  typedef vector<unsigned int> Chromosome;

  class Mutation
  {
  public:
    Mutation(const Allele& target, int update);

    void apply_to(Chromosome& subject) const;
    
  private:
    Allele _target;
    int _update;
  };

  
  class Crossover
  {
  public:
    Crossover(unsigned int cut_point_A, unsigned int cut_point_B);
    Crossover(const Crossover& other);
    ~Crossover();

    Crossover& operator = (const Crossover& other);
    bool operator == (const Crossover& other) const;

    void breed(const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const;

  private: 
    unsigned int _cut_point_A;
    unsigned int _cut_point_B;

  };

  
  class Evolution
  {
  public:
    Evolution(const Crossover& crossover, unsigned int input=1, unsigned int output=1);
    ~Evolution(void);
    
    vector<Rule*> breed(const Rule& father, const Rule& mother);
    Rule* decode(const Chromosome&) const;
    Chromosome encode(const Rule& rule) const;
    
  private:
    const Crossover& _crossover;
    unsigned int _input_count;
    unsigned int _output_count;
    vector<Rule*> _rules;
  };

  
}

#endif
