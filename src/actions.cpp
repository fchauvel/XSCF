
#include "actions.h"



Actuator::Actuator()
{}

Actuator::~Actuator()
{}


Printer::Printer(ostream& out):
  _out(out)
{}

Printer::~Printer()
{}

void
Printer::set(int value)
{
  _out << "SET" << " " << value << endl;
}


Action::Action(int value):
  _value(value)
{}


Action::~Action()
{}


bool
Action::operator==(const Action& other) const
{
  return true;
}

void
Action::execute_on(Actuator& destination) const
{
  destination.set(_value);
}


