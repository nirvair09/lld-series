#include <iostream>
#include <string>
using namespace std;

class Logger {
private:
  int count = 0;
  Logger() {}

public:
  Logger(const Logger &) = default;

  Logger &operator=(const Logger &) = delete;

  static Logger &getInstance() {
    static Logger instance;
    return instance;
  }

  void log(const string &msg) {
    count++;
    cout << "[" << count << "]" << msg << endl;
  }
};

int main() {
  Logger::getInstance().log("App started");
  Logger::getInstance().log("User logged in");
  return 0;
}