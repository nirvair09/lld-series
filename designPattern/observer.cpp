#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
using namespace std;

class Observer {
public:
  virtual ~Observer() = default;
  virtual void update(const string &n) = 0;
};

class EmailNotifier : public Observer {
private:
  string email;

public:
  explicit EmailNotifier(string email) : email(move(email)) {};

  void update(const string &n) override {
    cout << "Email to" << email << " :" << n << "is send" << endl;
  }
};

class SmsNotifier : public Observer {
private:
  string SMS;

public:
  explicit SmsNotifier(string SMS) : SMS(move(SMS)) {};

  void update(const string &n) override {
    cout << "SMS to" << SMS << " :" << n << "is send" << endl;
  }
};

class Product {
private:
  string name;
  int stock = 0;
  vector<Observer *> observers;

  void notifyAll() {
    for (Observer *o : observers) {
      o->update(name);
    }
  }

public:
  explicit Product(string name) : name(move(name)) {};

  void subscribe(Observer *o) { observers.push_back(o); }

  void unsubscribe(Observer *o) {
    auto it = find(observers.begin(), observers.end(), o);
    if (it != observers.end()) {
      observers.erase(it);
    }
  }

  void restock(int count) {
    if (count <= 0)
      return;

    bool wasOutOfStock = (stock == 0);
    stock += count;
    if (wasOutOfStock) {
      notifyAll();
    }
  }

  void sell() {
    if (stock > 0)
      stock--;
  }
};

int main() {
  Product ps5("PS5");
  EmailNotifier alice("alice@mail.com");
  SmsNotifier bob("9876543210");

  ps5.subscribe(&alice);
  ps5.subscribe(&bob);

  cout << "--- restock(2) ---" << endl;
  ps5.restock(2); // stock 0 → 2: both notified

  cout << "--- restock(3) ---" << endl;
  ps5.restock(3); // stock 2 → 5: nothing prints

  ps5.unsubscribe(&bob);
  for (int i = 0; i < 5; i++)
    ps5.sell(); // stock 5 → 0

  cout << "--- restock(1) after unsubscribe ---" << endl;
  ps5.restock(1); // stock 0 → 1: only alice
  return 0;
}