#include <iostream>
#include <memory>
using namespace std;

enum class PaymentType { CARD, UPI };

class PaymentStrategy {
public:
  virtual ~PaymentStrategy() = default;
  virtual void pay(int amount) = 0;
};

class CardPayment : public PaymentStrategy {
public:
  void pay(int amount) override {
    int fee = amount / 100;
    cout << "Paid " << amount + fee << " using Card" << endl;
  }
};

class UPIPayment : public PaymentStrategy {
public:
  void pay(int amount) override {
    cout << "Paid " << amount << " using UPI" << endl;
  }
};

class PaymentFactory {
public:
  static unique_ptr<PaymentStrategy> create(PaymentType type) {
    switch (type) {
    case PaymentType::CARD:
      return make_unique<CardPayment>();
    case PaymentType::UPI:
      return make_unique<UPIPayment>();
    }
    return nullptr;
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

  c.setPaymentMethod(PaymentFactory::create(PaymentType::CARD));

  c.pay();

  c.setPaymentMethod(PaymentFactory::create(PaymentType::UPI));

  c.pay();

  return 0;
}