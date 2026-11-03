// #include <iostream>
// using namespace std;

// class shipping {
// public:
//   virtual void ship(int weight) = 0;
// };

// class standardShipping : public shipping {
// public:
//   void ship(int weight) override {
//     cout << "Shipping with standard shipping and cost is $" << weight * 10
//          << endl;
//   }
// };

// class expressShipping : public shipping {
// public:
//   void ship(int weight) override {
//     cout << "Shipping with express shipping and cost is $" << weight * 20
//          << endl;
//   }
// };

// class overnightShipping : public shipping {
// public:
//   void ship(int weight) override {
//     cout << "Shipping with overnight shipping and cost is $" << weight * 30
//          << endl;
//   }
// };

// class orderCost {
// private:
//   shipping *ship;
//   int weight;

// public:
//   orderCost(shipping *ship) : ship(ship) {};

//   void setWeight(int weight) { this->weight = weight; }

//   void setShippingMethod(shipping *ship) { this->ship = ship; }

//   void calculateCost() { ship->ship(weight); }
// };

// int main() {

//   standardShipping standard;
//   expressShipping express;
//   overnightShipping overnight;

//   orderCost order(&standard);
//   order.setWeight(6);
//   order.calculateCost();
//   order.setShippingMethod(&express);
//   order.calculateCost();
//   order.setShippingMethod(&overnight);
//   order.calculateCost();

//   return 0;
// }

// #include <iostream>
// #include <memory>
// #include <utility>
// using namespace std;

// class ShippingStrategy {
// public:
//   virtual ~ShippingStrategy() = default;
//   virtual double calculateCost(double weight) = 0;
// };

// class StandardShipping : public ShippingStrategy {
// public:
//   double calculateCost(double weight) override { return weight * 5; }
// };

// class ExpressShipping : public ShippingStrategy {
// public:
//   double calculateCost(double weight) override { return weight * 10 + 20; }
// };

// class OverNightShipping : public ShippingStrategy {
// public:
//   double calculateCost(double weight) override { return weight * 20 + 50; }
// };

// class SameDayShipping : public ShippingStrategy {
// public:
//   double calculateCost(double weight) override { return weight * 30 + 80; }
// };

// class DiscountStrategy {
// public:
//   virtual ~DiscountStrategy() = default;
//   virtual double applyDiscount(double cost) = 0;
// };

// class NoDiscount : public DiscountStrategy {
// public:
//   double applyDiscount(double cost) override { return cost; }
// };

// // FIX: renamed percentageDiscount -> PercentageDiscount to match the other
// // class names
// class PercentageDiscount : public DiscountStrategy {
// private:
//   double percentage;

// public:
//   PercentageDiscount(double percentage) : percentage(percentage) {};
//   double applyDiscount(double cost) override {
//     return cost * (1 - percentage / 100.00);
//   }
// };

// class Order {
// private:
//   double weight;
//   unique_ptr<ShippingStrategy> shipping;
//   unique_ptr<DiscountStrategy> discount;

// public:
//   // FIX: default to NoDiscount so totalCost() doesn't dereference a null
//   // pointer
//   Order(double w, unique_ptr<ShippingStrategy> s)
//       : weight(w), shipping(std::move(s)),
//         discount(make_unique<NoDiscount>()) {};

//   void setShippingStrategy(unique_ptr<ShippingStrategy> s) {
//     shipping = std::move(s); // FIX: qualified std::move
//   }

//   void setDiscountStrategy(unique_ptr<DiscountStrategy> d) {
//     discount = std::move(d); // FIX: qualified std::move
//   }

//   double totalCost() const {

//     double cost = shipping->calculateCost(weight);

//     return discount->applyDiscount(cost);
//   }
// };

// int main() {
//   Order order(6, make_unique<StandardShipping>());
//   cout << "Standard:            $" << order.totalCost() << endl; // 30

//   order.setShippingStrategy(make_unique<ExpressShipping>());
//   cout << "Express:             $" << order.totalCost() << endl; // 80

//   order.setShippingStrategy(make_unique<OvernightShipping>());
//   cout << "Overnight:           $" << order.totalCost() << endl; // 170

//   order.setShippingStrategy(make_unique<SameDayShipping>());
//   cout << "Same-day:            $" << order.totalCost() << endl; // 260

//   order.setShippingStrategy(make_unique<ExpressShipping>());
//   order.setDiscountStrategy(make_unique<percentageDiscount>(10));
//   cout << "Express + 10% off:   $" << order.totalCost() << endl; // 72
// }

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
using namespace std;

class ShippingStrategy {
public:
  virtual ~ShippingStrategy() = default;
  virtual double calculateCost(double weight) = 0;
};

class StandardShipping : public ShippingStrategy {
public:
  double calculateCost(double w) override { return w * 5; }
};

class ExpressShipping : public ShippingStrategy {
public:
  double calculateCost(double w) override { return w * 10 + 20; }
};

class SameDayShipping : public ShippingStrategy {
public:
  double calculateCost(double w) override { return w * 30 + 80; }
};

class ShippingStrategyFactory {
public:
  static unique_ptr<ShippingStrategy> create(const string &type) {

    if (type == "standard") {
      return make_unique<StandardShipping>();
    } else if (type == "express") {
      return make_unique<ExpressShipping>();
    } else if (type == "same-day") {
      return make_unique<SameDayShipping>();
    } else {
      throw invalid_argument("Invalid shipping type");
    }
  }
};

int main() {

  try {

    auto shippingStrategy = ShippingStrategyFactory::create("express");
    cout << "Cost for 10kg: $" << shippingStrategy->calculateCost(10) << endl;

  } catch (const invalid_argument &e) {
    cerr << e.what() << endl;
  }

  return 0;
};