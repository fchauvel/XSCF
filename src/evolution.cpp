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


#include "evolution.h"


using namespace xcsf;


Evolution::Evolution()
  :_rules()
{}


Evolution::~Evolution()
{
  for(auto each_rule: _rules) {
    delete each_rule;
  }
}


vector<Rule*>
Evolution::breed(const Rule& father, const Rule& mother, unsigned int cut_point_A, unsigned int cut_point_B)
{
  vector<Rule*> children;
  
  Rule *child_A = new Rule({Interval(0, 100)}, { 4}, 1.0, 1.0, 1.0);
  _rules.push_back(child_A);
  children.push_back(child_A);

  Rule *child_B = new Rule({Interval(50, 50)}, {2}, 1.0, 1.0, 1.0);
  _rules.push_back(child_B);
  children.push_back(child_B);
  
  return children;
}
  
  
