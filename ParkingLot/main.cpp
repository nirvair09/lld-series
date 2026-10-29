#include <iostream>
#include<string>
#include<vector>
using namespace std;

enum class VehicleType{
    BIKE,
    CAR,
    TRUCK
};

enum class SpotType{
    SMALL,
    MEDIUM,
    LARGE,
};

enum class PaymentStatus{
   PENDING,
   PAID,
   FAILED,
};

class Vehicle{
    Private:
    string licensePlate;
    VehicleType type;

    public:
    Vehicle(const string& licensePlate,VehicleType type):licensePlate(licensePlate),type(type){}


    string getLicensePlate() const{
        return lisensePlate;
    }

    VehicleType getType() const{
        return type;
    }
};

int main() {
    Vehicle v("WB-02-1234", VehicleType::CAR);
    cout << "Plate: " << v.getLicensePlate() << endl;
    return 0;
}