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


#ifndef XCSF_UTILS_H
#define XCSF_UTILS_H


#include "context.h"


namespace xcsf
{

  
  class Interval
  {
  public:
    Interval(const Value& lower=0, const Value& upper=Value::MAXIMUM);

    
    bool operator == (const Interval& other) const;
    bool operator != (const Interval& other) const;

    const Value& lower() const;
    const Value& upper() const;
    
    bool includes(const Interval& other) const;
    bool contains(const Value value) const;
    
  private:
    friend std::ostream& operator << (std::ostream& out, const Interval& interval);
    
    Value _lower_bound;
    Value _upper_bound;
    
  };
  
  
  class Randomizer
  {
  public:
    Randomizer();
    virtual ~Randomizer();

    unsigned int unsigned_int(unsigned int lower=0, unsigned int upper=100) const;

    virtual double uniform(void) const;    
  };
  
}


#endif
