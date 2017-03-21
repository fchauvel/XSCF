#ifndef XCSF_ENVIRONMENT_H
#define XCSF_ENVIRONMENT_H

#include <vector>


using namespace std;


class Context
{
 public:
  Context();
  ~Context();

  void queue_length(unsigned int queue_length);
  unsigned int queue_length(void) const;
  
 private:
  unsigned int _queue_length;
  
};


class Observer
{
 public:
  Observer();
  virtual ~Observer();
  
  virtual void update(Context* const context) = 0;

};


class Observable
{
 public:
  Observable();
  virtual ~Observable();

  void subscribe(Observer* observer);
 
 protected:
  virtual void notify_observers(Context* const context) const;
  
 private:
  vector<Observer*> _observers;

};


class Environment: public Observable
{

 public:
  Environment();
  ~Environment();
  
};

#endif
