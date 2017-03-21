

#include "environment.h"


Context::Context():
  _queue_length(0)
{}

Context::~Context()
{}

void
Context::queue_length(unsigned int queue_length)
{
  _queue_length = queue_length;
}

unsigned int
Context::queue_length(void) const
{
  return _queue_length;
}



Observer::Observer()
{}

Observer::~Observer()
{}



Observable::Observable():
  _observers(2)
{}


Observable::~Observable()
{}

void
Observable::subscribe(Observer* observer)
{ 
  _observers.push_back(observer);
}

void
Observable::notify_observers(Context* const context) const
{
  for (vector<Observer*>::const_iterator each = _observers.begin();
       each != _observers.end();
       ++each) {
    (*each)->update(context);
  }
}


Environment::Environment():
  Observable()
{}


Environment::~Environment()
{}
