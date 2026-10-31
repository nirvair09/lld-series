// Build and run:
//   g++ -std=c++17 -Wall -Wextra test_vending.cpp -o test_vending && ./test_vending

// main.cpp defines its own main(); rename it so both can coexist in this TU.
#define main vending_demo_main
#include "main.cpp"
#undef main

#include <cstdlib>
#include <iostream>

static int passed = 0;
static int failed = 0;

#define CHECK(cond)                                                          \
  do {                                                                       \
    if (cond) {                                                              \
      ++passed;                                                              \
    } else {                                                                 \
      ++failed;                                                              \
      std::cerr << __FILE__ << ":" << __LINE__ << ": CHECK failed: " #cond   \
                << "\n";                                                     \
    }                                                                        \
  } while (0)

// ---------- Product ----------

static void testProductGetters() {
  Product p("Coke", 59);
  CHECK(p.getName() == "Coke");
  CHECK(p.getPrice() == 59);
}

static void testProductZeroPrice() {
  Product p("Water", 0);
  CHECK(p.getPrice() == 0);
}

// ---------- Slot ----------

static void testSlotInitialState() {
  Product p("Coke", 59);
  Slot s("A1", p, 3);
  CHECK(s.getId() == "A1");
  CHECK(s.getProduct().getName() == "Coke");
  CHECK(s.getProduct().getPrice() == 59);
  CHECK(s.isAvailable());
}

static void testSlotEmptyIsNotAvailable() {
  Product p("Coke", 59);
  Slot s("A1", p, 0);
  CHECK(!s.isAvailable());
  CHECK(!s.dispenseProduct());
}

static void testSlotDispenseDecrementsUntilEmpty() {
  Product p("Coke", 59);
  Slot s("A1", p, 2);
  CHECK(s.dispenseProduct());
  CHECK(s.isAvailable());
  CHECK(s.dispenseProduct());
  CHECK(!s.isAvailable());
  CHECK(!s.dispenseProduct());
  CHECK(!s.isAvailable());
}

static void testSlotRestockAddsStock() {
  Product p("Coke", 59);
  Slot s("A1", p, 0);
  CHECK(s.restock(5));
  CHECK(s.isAvailable());
  for (int i = 0; i < 5; ++i) CHECK(s.dispenseProduct());
  CHECK(!s.dispenseProduct());
}

static void testSlotRestockRejectsNonPositive() {
  Product p("Coke", 59);
  Slot s("A1", p, 1);
  CHECK(!s.restock(0));
  CHECK(!s.restock(-3));
  // Stock unchanged: exactly one dispense succeeds.
  CHECK(s.dispenseProduct());
  CHECK(!s.dispenseProduct());
}

// ---------- Inventory ----------

static void testInventoryMissingSlot() {
  Inventory inv;
  CHECK(!inv.isAvailable("Z9"));
  CHECK(inv.getPrice("Z9") == -1);
  CHECK(!inv.dispenseProduct("Z9"));
  CHECK(!inv.restockSlot("Z9", 1));
}

static void testInventoryAddAndQuery() {
  Inventory inv;
  Product coke("Coke", 59);
  CHECK(inv.addSlot("A1", coke, 2));
  CHECK(inv.isAvailable("A1"));
  CHECK(inv.getPrice("A1") == 59);
}

static void testInventoryMultipleSlotsIndependent() {
  Inventory inv;
  inv.addSlot("A1", Product("Coke", 59), 1);
  inv.addSlot("B2", Product("Chips", 25), 0);

  CHECK(inv.getPrice("A1") == 59);
  CHECK(inv.getPrice("B2") == 25);
  CHECK(inv.isAvailable("A1"));
  CHECK(!inv.isAvailable("B2"));
}

static void testInventoryDispenseThroughInventory() {
  Inventory inv;
  inv.addSlot("A1", Product("Coke", 59), 1);
  CHECK(inv.dispenseProduct("A1"));
  CHECK(!inv.isAvailable("A1"));
  CHECK(!inv.dispenseProduct("A1"));
}

static void testInventoryRestockThroughInventory() {
  Inventory inv;
  inv.addSlot("A1", Product("Coke", 59), 0);
  CHECK(!inv.isAvailable("A1"));
  CHECK(inv.restockSlot("A1", 2));
  CHECK(inv.isAvailable("A1"));
  CHECK(inv.dispenseProduct("A1"));
  CHECK(inv.dispenseProduct("A1"));
  CHECK(!inv.dispenseProduct("A1"));
  CHECK(!inv.restockSlot("A1", 0));
}

// Known issue: emplace() does not overwrite an existing key, so the second
// addSlot silently keeps the original slot yet still returns true. Expected
// behaviour is that a duplicate id is rejected (return false) and the
// original slot is preserved.
static void testInventoryDuplicateIdRejected() {
  Inventory inv;
  CHECK(inv.addSlot("A1", Product("Coke", 59), 1));
  CHECK(!inv.addSlot("A1", Product("Chips", 25), 5));
  CHECK(inv.getPrice("A1") == 59);
}

// ---------- CoinManager ----------

static void testCoinValues() {
  CHECK(coinValue(Coin::ONE) == 1);
  CHECK(coinValue(Coin::TWO) == 2);
  CHECK(coinValue(Coin::FIVE) == 5);
  CHECK(coinValue(Coin::TEN) == 10);
}

static void testCoinManagerInsertAndBalance() {
  CoinManager cm;
  CHECK(cm.getBalance() == 0);
  cm.insertCoin(Coin::FIVE);
  cm.insertCoin(Coin::TEN);
  CHECK(cm.getBalance() == 15);
}

static void testCoinManagerAddToReserveRejectsNonPositive() {
  CoinManager cm;
  CHECK(!cm.addToReserve(Coin::ONE, 0));
  CHECK(!cm.addToReserve(Coin::ONE, -1));
  CHECK(cm.addToReserve(Coin::ONE, 3));
}

static void testCoinManagerRefundReturnsInsertedCoins() {
  CoinManager cm;
  cm.insertCoin(Coin::TWO);
  cm.insertCoin(Coin::FIVE);
  vector<Coin> refunded = cm.refund();
  CHECK(refunded.size() == 2);
  CHECK(refunded.size() == 2 && refunded[0] == Coin::TWO);
  CHECK(refunded.size() == 2 && refunded[1] == Coin::FIVE);
  CHECK(cm.getBalance() == 0);
  CHECK(cm.refund().empty());
}

static void testCoinManagerSaleInsufficientBalance() {
  CoinManager cm;
  cm.insertCoin(Coin::FIVE);
  auto result = cm.completeSale(10);
  CHECK(!result.has_value());
  // Failed sale must not consume the inserted coins.
  CHECK(cm.getBalance() == 5);
}

static void testCoinManagerExactPaymentNoChange() {
  CoinManager cm;
  cm.insertCoin(Coin::TEN);
  auto result = cm.completeSale(10);
  CHECK(result.has_value());
  CHECK(result.has_value() && result->empty());
  CHECK(cm.getBalance() == 0);
}

static void testCoinManagerSaleReturnsChangeFromReserve() {
  CoinManager cm;
  CHECK(cm.addToReserve(Coin::TWO, 5));
  CHECK(cm.addToReserve(Coin::ONE, 5));
  cm.insertCoin(Coin::TEN);
  auto result = cm.completeSale(7);  // change due: 3
  CHECK(result.has_value());
  if (result.has_value()) {
    int total = 0;
    for (Coin c : *result) total += coinValue(c);
    CHECK(total == 3);
  }
  CHECK(cm.getBalance() == 0);
}

static void testCoinManagerInsufficientChangeRejected() {
  CoinManager cm;
  // Reserve is empty, so 3 in change cannot be made.
  cm.insertCoin(Coin::TEN);
  auto result = cm.completeSale(7);
  CHECK(!result.has_value());
  // Sale is atomic: coins remain inserted so the user can be refunded.
  CHECK(cm.getBalance() == 10);
  CHECK(cm.refund().size() == 1);
}

static void testCoinManagerPaymentsFeedReserve() {
  CoinManager cm;
  // Exact payment of 5 stores the FIVE in the reserve...
  cm.insertCoin(Coin::FIVE);
  CHECK(cm.completeSale(5).has_value());
  // ...so a later overpayment can take it back as change.
  cm.insertCoin(Coin::TEN);
  auto result = cm.completeSale(5);
  CHECK(result.has_value());
  CHECK(result.has_value() && result->size() == 1);
  CHECK(result.has_value() && result->size() == 1 && (*result)[0] == Coin::FIVE);
}

int main() {
  testCoinValues();
  testCoinManagerInsertAndBalance();
  testCoinManagerAddToReserveRejectsNonPositive();
  testCoinManagerRefundReturnsInsertedCoins();
  testCoinManagerSaleInsufficientBalance();
  testCoinManagerExactPaymentNoChange();
  testCoinManagerSaleReturnsChangeFromReserve();
  testCoinManagerInsufficientChangeRejected();
  testCoinManagerPaymentsFeedReserve();

  testProductGetters();
  testProductZeroPrice();

  testSlotInitialState();
  testSlotEmptyIsNotAvailable();
  testSlotDispenseDecrementsUntilEmpty();
  testSlotRestockAddsStock();
  testSlotRestockRejectsNonPositive();

  testInventoryMissingSlot();
  testInventoryAddAndQuery();
  testInventoryMultipleSlotsIndependent();
  testInventoryDispenseThroughInventory();
  testInventoryRestockThroughInventory();
  testInventoryDuplicateIdRejected();

  std::cout << passed << " passed, " << failed << " failed\n";
  return failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
