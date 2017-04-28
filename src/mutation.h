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


#ifndef XCSF_MUTATION_H
#define XCSF_MUTATION_H



#include <memory>

#include "chromosome.h"




namespace xcsf
{

  class Mutation
  {
  public:
    virtual ~Mutation();

    virtual void operator () (Chromosome& subject) const = 0;
  };


  
  class MutationFactory
  {
  public:
    virtual ~MutationFactory();

    virtual std::unique_ptr<Mutation> generate(void) const = 0;
  };
  
  

  class ShiftMutation: public Mutation
  {
  public:
    ShiftMutation(const Allele& target, int update);
    virtual ~ShiftMutation();
    
    virtual void operator () (Chromosome& subject) const;
    
  private:
    Allele _target;
    int _update;
    
  };

  
}

#endif
