#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

enum class Coin {
  ONE = 1,
  TWO = 2,
  FIVE = 5,
  TEN = 10,
};

int coinValue(Coin c) { return static_cast<int>(c); };
const vector<Coin> DENOMINATIONS = {Coin::TEN, Coin::FIVE, Coin::TWO,
                                    Coin::ONE};

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

class CoinManager {
private:
  map<Coin, int> reserve;
  vector<Coin> insertedCoins;
  int balance = 0;

public:
  bool addToReserve(Coin c, int count) {
    if (count <= 0)
      return false;

    int currentCount = reserve[c];
    reserve[c] = currentCount + count;
    return true;
  }

  void insertCoin(Coin c) {
    balance += coinValue(c);
    insertedCoins.push_back(c);
  }

  int getBalance() const { return balance; }

  vector<Coin> refund() {
    vector<Coin> coinsToRefund = insertedCoins;
    insertedCoins.clear();
    balance = 0;
    return coinsToRefund;
  }

  optional<vector<Coin>> completeSale(int price) {
    if (balance < price) {
      return nullopt;
    }

    map<Coin, int> temp = reserve;
    for (Coin c : insertedCoins) {
      temp[c]++;
    }

    int remaining = balance - price;
    vector<Coin> change;

    for (Coin c : DENOMINATIONS) {
      while (remaining >= coinValue(c) && temp[c] > 0) {
        change.push_back(c);
        temp[c]--;
        remaining -= coinValue(c);
      }
    }

    if (remaining != 0) {
      return nullopt;
    }

    reserve = temp;
    insertedCoins.clear();
    balance = 0;
    return change;
  }
};

// int main() {
//   Inventory inv;
//   cout << inv.addSlot("A1", Product("Coke", 20), 2) << endl;  // 1
//   cout << inv.addSlot("A1", Product("Pepsi", 20), 5) << endl; // 0
//   (duplicate)

//   cout << inv.isAvailable("A1") << endl; // 1
//   cout << inv.getPrice("A1") << endl;    // 20
//   inv.dispenseProduct("A1");
//   inv.dispenseProduct("A1");
//   cout << inv.isAvailable("A1") << endl;     // 0 (empty)
//   cout << inv.dispenseProduct("A1") << endl; // 0 (can't dispense)
//   cout << inv.restockSlot("A1", 3) << endl;  // 1
//   cout << inv.isAvailable("A1") << endl;     // 1 (restocked)
//   cout << inv.isAvailable("Z9") << endl;     // 0 (doesn't exist)
//   return 0;
// }

class VendingMachine;

class State {
public:
  virtual ~State() = default;
  virtual void insertCoin(VendingMachine &m, Coin c) = 0;
  virtual void selectProduct(VendingMachine &m, const string &id) = 0;
  virtual void cancel(VendingMachine &m) = 0;
  virtual string name() const = 0;
};

class IdleState : public State {
public:
  void insertCoin(VendingMachine &m, Coin c) override;
  void selectProduct(VendingMachine &m, const string &id) override;
  void cancel(VendingMachine &m) override;
  string name() const override { return "Idle"; }
};

class HasMoneyState : public State {
public:
  void insertCoin(VendingMachine &m, Coin c) override;
  void selectProduct(VendingMachine &m, const string &id) override;
  void cancel(VendingMachine &m) override;
  string name() const override { return "HasMoney"; }
};

class VendingMachine {
private:
  Inventory inventory;
  CoinManager coinManager;
  IdleState idleState;
  HasMoneyState hasMoneyState;
  State *currentState;

public:
  VendingMachine() : currentState(&idleState) {}

  void insertCoin(Coin c) { currentState->insertCoin(*this, c); }
  void selectProduct(const string &id) {
    currentState->selectProduct(*this, id);
  }
  void cancel() { currentState->cancel(*this); }

  bool addSlot(const string &id, const Product &p, int qty) {
    return inventory.addSlot(id, p, qty);
  }
  bool addCoinsToReserve(Coin c, int count) {
    return coinManager.addToReserve(c, count);
  }

  Inventory &getInventory() { return inventory; }
  CoinManager &getCoinManager() { return coinManager; }
  void setState(State *s) { currentState = s; }
  State *getIdleState() { return &idleState; }
  State *getHasMoneyState() { return &hasMoneyState; }
  string getStateName() const { return currentState->name(); }
};

// ===== Idle mood =====
void IdleState::insertCoin(VendingMachine &m, Coin c) {
  m.getCoinManager().insertCoin(c);
  cout << "Inserted " << coinValue(c)
       << ", balance: " << m.getCoinManager().getBalance() << endl;
  m.setState(m.getHasMoneyState());
}

void IdleState::selectProduct(VendingMachine &m, const string &id) {
  cout << "Please insert coins first" << endl;
}

void IdleState::cancel(VendingMachine &m) {
  cout << "Nothing to refund" << endl;
}

// ===== HasMoney mood =====
void HasMoneyState::insertCoin(VendingMachine &m, Coin c) {
  m.getCoinManager().insertCoin(c);
  cout << "Inserted " << coinValue(c)
       << ", balance: " << m.getCoinManager().getBalance() << endl;
}

void HasMoneyState::cancel(VendingMachine &m) {
  vector<Coin> coins = m.getCoinManager().refund();
  cout << "Refunded: ";
  for (Coin c : coins)
    cout << coinValue(c) << " ";
  cout << endl;
  m.setState(m.getIdleState());
}

void HasMoneyState::selectProduct(VendingMachine &m, const string &id) {
  Inventory &inv = m.getInventory();
  CoinManager &cm = m.getCoinManager();

  if (!inv.isAvailable(id)) {
    cout << "Product " << id << " not available" << endl;
    return;
  }

  int price = inv.getPrice(id);
  if (cm.getBalance() < price) {
    cout << "Insert " << price - cm.getBalance() << " more" << endl;
    return;
  }

  optional<vector<Coin>> change = cm.completeSale(price);
  if (!change) {
    cout << "Cannot give exact change." << endl;
    cancel(m);
    return;
  }

  inv.dispenseProduct(id);
  cout << "Dispensed " << id << ". Change: ";
  for (Coin c : *change)
    cout << coinValue(c) << " ";
  cout << endl;
  m.setState(m.getIdleState());
}

// ===== Test =====
int main() {
  VendingMachine vm;
  vm.addSlot("A1", Product("Coke", 20), 1);
  vm.addCoinsToReserve(Coin::ONE, 5);
  vm.addCoinsToReserve(Coin::TWO, 5);

  vm.selectProduct("A1");   // Idle → "insert coins first"
  vm.insertCoin(Coin::TEN); // → HasMoney
  vm.selectProduct("A1");   // "Insert 10 more"
  vm.insertCoin(Coin::TEN);
  vm.selectProduct("A1"); // Dispensed → Idle
  cout << "State: " << vm.getStateName() << endl;

  vm.insertCoin(Coin::FIVE);
  vm.cancel(); // Refunded 5 → Idle
  cout << "State: " << vm.getStateName() << endl;
  return 0;
}
