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


#ifndef XCSF_CONTEXT_H
#define XCSF_CONTEXT_H


#include <vector>
#include <iostream>
#include <stdexcept>


namespace xcsf
{

  class Value
  {
  public:

    Value(int value);
    Value(unsigned int value);
    Value(const Value& prototype);
    ~Value();

    Value& operator = (const Value& other);
 
    explicit operator unsigned int (void) const;
  
    friend bool operator == (const Value& left, const Value& right);
    friend bool operator != (const Value& left, const Value& right);
    friend bool operator > (const Value& left, const Value& right);
    friend bool operator >= (const Value& left, const Value& right);
    friend bool operator < (const Value& left, const Value& right);
    friend bool operator <= (const Value& left, const Value& right);
  
    friend const Value operator + (const Value& left, const Value& right);
    friend const Value operator - (const Value& left, const Value& right);

    friend std::ostream& operator << (std::ostream& out, const Value& value);

    static const unsigned int MAXIMUM = 100;
  
  private:
  
    unsigned int _value;
  
  };
   

  class Vector
  {
  public:
    Vector(unsigned int dimension_count);
    Vector(std::initializer_list<int> values);
    Vector(const std::vector<int>& values);
    Vector(const std::vector<unsigned int>& values);
    Vector(const Vector& other);
    ~Vector();

    void operator = (const Vector& other);
    bool operator == (const Vector& other) const;
    bool operator < (const Vector& other) const;
  
    unsigned int size(void) const;
    const Value& operator [] (unsigned int index) const;


    static Vector parse(const std::string& text);
  
  private:
    friend std::ostream& operator << (std::ostream& out, const Vector& vector);
  
    std::vector<Value> _values;
  
  };


}

#endif
