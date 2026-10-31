#include <iostream>
#include <memory>
using namespace std;

class PaymentStrategy {
public:
  virtual ~PaymentStrategy() = default;
  virtual void pay(int amount) = 0;
};

class CardPayment : public PaymentStrategy {
public:
  void pay(int amount) override {
    int fee = amount * 2 / 100;
    cout << "Paid " << amount + fee << " via Card" << endl;
  }
};

class UPIPayment : public PaymentStrategy {
public:
  void pay(int amount) override {

    cout << "Paid " << amount << " via UPI" << endl;
  }
};

class CheckOut {
private:
  int amount;
  unique_ptr<PaymentStrategy> paymentMethod;

public:
  explicit CheckOut(int amount) : amount(amount) {};

  void setPaymentMethod(unique_ptr<PaymentStrategy> paymentMethod) {
    this->paymentMethod = move(paymentMethod);
  }

  void pay() {
    if (!paymentMethod) {
      cout << "Chose payment method please" << endl;
      return;
    }

    paymentMethod->pay(amount);
  }
};

int main() {
  CheckOut c(2000);

  c.setPaymentMethod(make_unique<CardPayment>());

  c.pay();

  c.setPaymentMethod(make_unique<UPIPayment>());

  c.pay();

  return 0;
}