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


#include "selection.h"


using namespace xcsf;


Selection::~Selection()
{}


DummySelection::~DummySelection()
{}


vector<Rule*>
DummySelection::operator () (const RuleSet& rules) const
{
  vector<Rule*> selection;
  selection.push_back(&rules[0]);
  selection.push_back(&rules[1]);
  return selection;  
}

