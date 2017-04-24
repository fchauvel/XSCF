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


#ifndef XCSF_DECODER_H
#define XCSF_DECODER_H


#include <iostream>


using namespace std;


namespace xcsf {

  class Controllable
  {
  public:
    virtual ~Controllable() = 0;
       
    virtual void reward(double value) = 0;
  };
  

  class Decoder
  {
  public:
    Decoder(istream& source, Controllable& target);
    
    void decode(void);
    
    
  private:
    istream& _source;
    Controllable& _target;
    
  };

};

#endif
