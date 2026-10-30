#include <iostream>
#include <string>
#include <unordered_map>
using namespace std;

enum class Coin {
  ONE = 1,
  TWO = 2,
  FIVE = 5,
  TEN = 10,
};

class Product {
private:
  string name;
  int price;

public:
  Product(string name, int price) : name(name), price(price) {};

  const string &getName() const { return name; }

  const int &getPrice() const { return price; }
};

class Slot {
private:
  string id;
  Product product;
  int quantity;

public:
  Slot(string id, Product product, int quantity)
      : id(id), product(product), quantity(quantity) {};

  const string &getId() const { return id; }

  bool isAvailable() const { return quantity > 0; }

  bool dispenseProduct() {
    if (isAvailable()) {
      quantity--;
      return true;
    }
    return false;
  }

  bool restock(int count) {
    if (count <= 0)
      return false;
    quantity += count;
    return true;
  }

  const Product &getProduct() const { return product; }
};

class Inventory {
private:
  unordered_map<string, Slot> slots;

public:
  bool addSlot(const string &id, const Product &product, int quantity) {
    Slot newSlot(id, product, quantity);
    auto it = slots.find(id);
    if (it == slots.end()) {
      slots.insert({id, newSlot});
      return true;
    } else {
      return false;
    }
  };

  bool isAvailable(const string &id) const {
    auto it = slots.find(id);

    if (it != slots.end()) {
      return it->second.isAvailable();
    }

    return false;
  }

  int getPrice(const string &id) const {
    auto it = slots.find(id);

    if (it != slots.end()) {
      return it->second.getProduct().getPrice();
    }

    return -1;
  }

  bool dispenseProduct(const string &id) {
    auto it = slots.find(id);

    if (it != slots.end()) {
      return it->second.dispenseProduct();
    }

    return false;
  }

  bool restockSlot(const string &id, int count) {
    auto it = slots.find(id);

    if (it != slots.end()) {
      return it->second.restock(count);
    }

    return false;
  }
};

int main() {
  Inventory inv;
  cout << inv.addSlot("A1", Product("Coke", 20), 2) << endl;  // 1
  cout << inv.addSlot("A1", Product("Pepsi", 20), 5) << endl; // 0 (duplicate)

  cout << inv.isAvailable("A1") << endl; // 1
  cout << inv.getPrice("A1") << endl;    // 20
  inv.dispenseProduct("A1");
  inv.dispenseProduct("A1");
  cout << inv.isAvailable("A1") << endl;     // 0 (empty)
  cout << inv.dispenseProduct("A1") << endl; // 0 (can't dispense)
  cout << inv.restockSlot("A1", 3) << endl;  // 1
  cout << inv.isAvailable("A1") << endl;     // 1 (restocked)
  cout << inv.isAvailable("Z9") << endl;     // 0 (doesn't exist)
  return 0;
}