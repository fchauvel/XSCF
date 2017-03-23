
#include <sstream>

#include "classifier.h"


Interval::Interval(Value lower, Value upper):
  _lower_bound(lower),
  _upper_bound(upper)
{}


Interval::~Interval()
{}

bool
Interval::contains(const Value value) const
{
  return _lower_bound <= value and _upper_bound >= value;
}



Classifier::Classifier(unsigned int input_size, unsigned int output_size):
  _intervals(input_size, Interval(0, 1)),
  _outputs(output_size, 0)
{}


Classifier::~Classifier()
{}


Interval&
Classifier::operator [](unsigned int index)
{
  return _intervals[index];
}

bool
Classifier::match(const Context& context) const
{
  if (context.size() != _intervals.size()) {
    stringstream message;
    message << "Invalid context! Expecting " << _intervals.size()
	    << " dimensions, but found only " << context.size()
	    << " instead."; 
    throw std::invalid_argument(message.str());
  }
  
  for(unsigned int index=0 ; index<_intervals.size() ; index++) {
    if (!_intervals[index].contains(context[index])) {
      return false;
    }
  }
  
  return true;
}


const vector<Value>&
Classifier::outputs(void) const
{
  return _outputs;
}
