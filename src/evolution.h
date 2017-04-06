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

  class Evolution
  {
  public:
    Evolution(void);
    ~Evolution(void);
    
    vector<Rule*> breed(const Rule& father, const Rule& mother, unsigned int cut_point_A, unsigned int cut_point_B);

  private:
    vector<Rule*> _rules;
  };

  
}

#endif
