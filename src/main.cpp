


#include "actions.h"
#include "controller.h"


using namespace std;


class App
{
public:
  App():
    _agent(new Agent()),
    _actuator(new Printer(cout)),
    _controller(new Controller(_actuator, _agent))
  {};
  
  ~App()
  {
    delete _agent;
    delete _actuator;
    delete _controller;
  };

  void run(void) const {
    Context context;
    _controller->select_action(&context);
  }
  
private:
  Agent* const _agent;
  Actuator* const _actuator;
  Controller* const _controller;
};


int
main(int argc, char** argv)
{
  cout << APPLICATION << " v" << VERSION << endl; 

  App app;
  app.run();
  
}
