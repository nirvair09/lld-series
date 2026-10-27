#include <iostream>
#include <string>
#include <vector>
using namespace std;

class FoodItem{
    private:
         string name;
         double price;
         int quantity;
    public:
    FoodItem(string n, double p, int q)
    : name(n), price(p), quantity(q) {} 

    string getName()const{
        return name;
    }

    double getPrice()const{
        return price;
    }

    int getQuantity()const{
        return quantity;
    }
};


class Order{
    private:
        string orderId;
        vector<FoodItem> foodItems;

    public:
    Order(string id):orderId(id){}

    string getOrderId()const{
        return orderId;
    }

    void addFoodItem(const FoodItem& item){
        foodItems.push_back(item);
    }

    const vector<FoodItem>& getFoodItems()const{
        return foodItems;
    }

    double calculateTotalPrice()const{
        double total=0.0;

        for(const auto& item:foodItems){
            total+=item.getPrice()*item.getQuantity();
        }
        return total;

        //- it.getPrice() → object
// - it->getPrice() → pointer to object
    }
};


class User{
    private:
    string id;
    vector<Order>orders;
public:
       
    User(string id) : id(id) {}

    string getId()const{
        return id;
    }

    const vector<Order> getOrders()const{
        return orders;
    }


    void addOrder(const Order& order){
        orders.push_back(order);
    }
};


int main(){
    FoodItem pizza("Pizza", 10.0, 2);
    FoodItem burger("Burger", 5.0, 3);

    Order order("testOrder");
    order.addFoodItem(pizza);
    order.addFoodItem(burger);

    cout<<"Order Id"<<order.getOrderId()<<endl;
    cout<<"total price:"<<order.calculateTotalPrice()<<endl;

    return 0;
}