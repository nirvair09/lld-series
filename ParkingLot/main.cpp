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
    private:
    string licensePlate;
    VehicleType type;

    public:
    Vehicle(const string& licensePlate,VehicleType type):licensePlate(licensePlate),type(type){}


    string getLicensePlate() const{
        return licensePlate;
    }

    VehicleType getType() const{
        return type;
    }
};

class ParkingSpot{
    private:
    int spotId;
    SpotType type;
    Vehicle* parkedVehicle; // Pointer to the vehicle parked in this spot null means no vehicle is parked

    public:
    ParkingSpot(int spotId, SpotType t):spotId(spotId),type(t),parkedVehicle(nullptr){};

    bool isFree() const{
        return parkedVehicle==nullptr;
    }

    bool canFit(const Vehicle& v) const {
        switch (v.getType()) {
            case VehicleType::BIKE:  return type == SpotType::SMALL;
            case VehicleType::CAR:   return type == SpotType::MEDIUM;
            case VehicleType::TRUCK: return type == SpotType::LARGE;
        }
        return false;
    }


    bool park(Vehicle* v){
        if(!isFree()||!canFit(*v)) {
            return false;
        }

        parkedVehicle=v;

        return true;
    }


    void unpark(){
        parkedVehicle=nullptr;
    }

     int getId() const { return spotId; };
    SpotType getType() const { return type; };
    Vehicle* getVehicle() const { return parkedVehicle; };

};
int main() {
    Vehicle car("WB-02-1234", VehicleType::CAR);
    Vehicle bike("WB-06-9999", VehicleType::BIKE);

    ParkingSpot spot1(1, SpotType::MEDIUM);

    cout << "Bike parks in MEDIUM? " << spot1.park(&bike) << endl;  // 0
    cout << "Car parks in MEDIUM?  " << spot1.park(&car)  << endl;  // 1
    cout << "Spot free now?        " << spot1.isFree()    << endl;  // 0

    spot1.unpark();
    cout << "After unpark, free?   " << spot1.isFree()    << endl;  // 1
    return 0;
}