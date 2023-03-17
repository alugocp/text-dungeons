#include <functional>
#include <string>
#include <vector>

// Option type used for player choice
struct Option {
  std::function<void()> *func;
  std::string text;
};

// View type used to present player with information
struct View {
  std::vector<Option> options;
  std::string description;
};

// Game abstract class
class Game {
protected:
  virtual void display(View v) = 0;

public:
  virtual void start() = 0;
};