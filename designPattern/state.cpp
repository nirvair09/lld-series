#include <iostream>
using namespace std;

class VendingMachine;

class State {
public:
  virtual ~State() = default;
  virtual void insertCoin(VendingMachine &m) = 0;
  virtual void pressButton(VendingMachine &m) = 0;
  virtual void refund(VendingMachine &m) = 0;
};

class IdleState : public State {
public:
  void insertCoin(VendingMachine &m) override;
  void pressButton(VendingMachine &m) override;
  void refund(VendingMachine &m) override;
};

class CoinState : public State {
public:
  void insertCoin(VendingMachine &m) override;
  void pressButton(VendingMachine &m) override;
  void refund(VendingMachine &m) override;
};

class SoldOutState : public State {
public:
  void insertCoin(VendingMachine &m) override;
  void pressButton(VendingMachine &m) override;
  void refund(VendingMachine &m) override;
};

class VendingMachine {
  int stock;
  IdleState idleState;
  CoinState coinState;
  SoldOutState soldOutState;
  State *currentState;

public:
  VendingMachine(int s) : stock(s) {
    if (stock > 0) {
      currentState = &idleState;
    } else {
      currentState = &soldOutState;
    }
  }
  void insertCoin() { currentState->insertCoin(*this); }

  void pressButton() { currentState->pressButton(*this); }

  void refund() { currentState->refund(*this); }

  void setHasCoin() { currentState = &coinState; }

  void setSoldOut() { currentState = &soldOutState; }

  void setIdle() { currentState = &idleState; }

  int getStock() const { return stock; }

  void dispenseItem() {
    if (stock > 0) {
      cout << "Item dispensed." << endl;
      stock--;
      cout << "Stock remaining: " << stock << endl;

      if (stock == 0) {
        cout << "Product sold out." << endl;
        setSoldOut();
      } else {
        setIdle();
      }
    }
  }

  void refundCoin() {
    cout << "Coin refunded." << endl;
    setIdle();
  }
};

// IdleState implementation
void IdleState::insertCoin(VendingMachine &m) {
  cout << "Coin inserted. Transitioning to CoinState." << endl;
  m.setHasCoin();
};

void IdleState::pressButton(VendingMachine &m) {
  cout << "Please insert a coin first." << endl;
};

void IdleState::refund(VendingMachine &m) {
  cout << "No coin to refund." << endl;
};

// CoinState implementation
void CoinState::insertCoin(VendingMachine &m) {
  cout << "Coin coin already inserted." << endl;
  m.setHasCoin();
};

void CoinState::pressButton(VendingMachine &m) {
  cout << "Button pressed. Dispensing item." << endl;
  m.dispenseItem();
};

void CoinState::refund(VendingMachine &m) {
  cout << "Refunding coin." << endl;
  m.refundCoin();
};

// SoldOutState implementation
void SoldOutState::insertCoin(VendingMachine &m) {
  cout << "Product sold out , cannot insert coin, please wait for the next "
          "product to be available."
       << endl;
};

void SoldOutState::pressButton(VendingMachine &m) {
  cout << "Product sold out." << endl;
};

void SoldOutState::refund(VendingMachine &m) {
  cout << "No coin to refund." << endl;
};

int main() {
  VendingMachine vm(2);
  vm.pressButton(); // no coin
  vm.insertCoin();
  vm.insertCoin();  // already has one
  vm.pressButton(); // dispense (stock 1)
  vm.insertCoin();
  vm.refund(); // get coin back
  vm.insertCoin();
  vm.pressButton(); // dispense (stock 0) -> SoldOut
  vm.insertCoin();  // rejected
}