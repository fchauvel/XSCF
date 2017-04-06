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


Rule*
create_rule_from(vector<unsigned int> values, unsigned int input_count, unsigned int output_count) {
  using namespace std;

  if (values.size() != 2 * input_count + output_count) {
    throw invalid_argument("Input and output size do not match the given vector size!");
  }

  vector<Interval> constraints;
  for (unsigned int index=0 ; index < input_count ; ++index) {
    unsigned int lower_bound = values[index * 2];
    unsigned int upper_bound = values[index * 2 + 1];
    constraints.push_back(Interval(lower_bound, upper_bound));
  }

  vector<unsigned int> prediction;
  for (unsigned int index=input_count*2 ; index<values.size() ; ++index) {
    prediction.push_back(values[index]);
  }

  Vector p = Vector(prediction);
  Rule *result = new Rule(constraints, p, 1.0, 1.0, 1.0);
  return result;
}  


vector<Rule*>
Evolution::breed(const Rule& father, const Rule& mother, unsigned int cut_point_A, unsigned int cut_point_B)
{
  vector<Rule*> children;
  
  vector<unsigned int> v_father(father.as_vector());
  vector<unsigned int> v_mother(mother.as_vector());

  if (v_father.size() != v_mother.size()) {
    throw invalid_argument("Parents are of different length!");
  }
  
  vector<unsigned int> v_child_A;
  vector<unsigned int> v_child_B;
  
  for(unsigned int index=0 ; index<v_father.size() ; ++index) {
    if (cut_point_A <= index and index < cut_point_B) {
      v_child_A.push_back(v_mother[index]);
      v_child_B.push_back(v_father[index]);
    } else {
      v_child_A.push_back(v_father[index]);
      v_child_B.push_back(v_mother[index]);
    }
  }

  Rule *child_A = create_rule_from(v_child_A, 1, 1);
  _rules.push_back(child_A);
  children.push_back(child_A);

  Rule *child_B = create_rule_from(v_child_B, 1, 1);
  _rules.push_back(child_B);
  children.push_back(child_B);

  return children;
}

