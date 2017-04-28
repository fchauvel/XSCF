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

#include "utils.h"
#include "chromosome.h"


namespace xcsf
{

  
  class AlleleMutation
  {
  public:
    virtual ~AlleleMutation();

    virtual void operator () (Chromosome& subject, const Allele& target) const = 0;
  };


  
  class RandomAlleleMutation: public AlleleMutation
  {
  public:
    RandomAlleleMutation(const Randomizer& randomizer);
    virtual ~RandomAlleleMutation();

    virtual void operator () (Chromosome& subject, const Allele& target) const;

  private:
    const Randomizer& _generate;
    
  };
 
  
}

#endif
