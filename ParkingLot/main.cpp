#include <iostream>
#include<string>
#include<vector>
#include<unordered_map>
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

class ParkingFloor{
    private:
    int floorNumber;
    vector<ParkingSpot> spots;

    public:
    ParkingFloor(int number) : floorNumber(number) {}

    void addSpot(SpotType type) {
        int id = floorNumber * 100 + (int)spots.size() + 1;  // floor 1 -> 101, 102...
        spots.emplace_back(id, type);
    }


    ParkingSpot* findFreeSpot(const Vehicle& v){
        for(auto& spot:spots){
            if(spot.isFree()&&spot.canFit(v)){
                return &spot;
            }
        }
        return nullptr;
    }

    int countFreeSpots(SpotType type) const{
        int count=0;
        for(const auto& spot:spots){
            if(spot.getType()==type&&spot.isFree()){
                count++;
            }
        }
        return count;
    }

    int getFloorNumber() const {return floorNumber;}
};
// int main() {
//     Vehicle car("WB-02-1234", VehicleType::CAR);
//     Vehicle bike("WB-06-9999", VehicleType::BIKE);

//     ParkingSpot spot1(1, SpotType::MEDIUM);

//     cout << "Bike parks in MEDIUM? " << spot1.park(&bike) << endl;  // 0
//     cout << "Car parks in MEDIUM?  " << spot1.park(&car)  << endl;  // 1
//     cout << "Spot free now?        " << spot1.isFree()    << endl;  // 0

//     spot1.unpark();
//     cout << "After unpark, free?   " << spot1.isFree()    << endl;  // 1
//     return 0;
// }



class ParkingLot {
private:
    string name;
    vector<ParkingFloor> floors;
    unordered_map<string, ParkingSpot*> activeParkings;   // plate -> spot

public:
    ParkingLot(const string& lotName) : name(lotName) {}

    void addFloor(int floorNumber, int small, int medium, int large) {
        ParkingFloor floor(floorNumber);
        for (int i = 0; i < small;  i++) floor.addSpot(SpotType::SMALL);
        for (int i = 0; i < medium; i++) floor.addSpot(SpotType::MEDIUM);
        for (int i = 0; i < large;  i++) floor.addSpot(SpotType::LARGE);
        floors.push_back(move(floor));
    }

    ParkingSpot* parkVehicle(Vehicle* v) {
        if (activeParkings.count(v->getLicensePlate())) return nullptr;  // already parked

        for (auto& floor : floors) {
            ParkingSpot* spot = floor.findFreeSpot(*v);
            if (spot && spot->park(v)) {
                activeParkings[v->getLicensePlate()] = spot;
                return spot;
            }
        }
        return nullptr;   // lot is full for this vehicle type
    }

    bool unparkVehicle(const string& plate) {
        auto it = activeParkings.find(plate);
        if (it == activeParkings.end()) return false;   // no such car parked

        it->second->unpark();
        activeParkings.erase(it);
        return true;
    }

    void printAvailability() const {
        cout << "--- " << name << " ---" << endl;
        for (const auto& floor : floors) {
            cout << "Floor " << floor.getFloorNumber()
                 << " | S:" << floor.countFreeSpots(SpotType::SMALL)
                 << " M:"   << floor.countFreeSpots(SpotType::MEDIUM)
                 << " L:"   << floor.countFreeSpots(SpotType::LARGE) << endl;
        }
    }
};

// int main() {
//     ParkingFloor floor1(1);
//     floor1.addSpot(SpotType::SMALL);
//     floor1.addSpot(SpotType::MEDIUM);
//     floor1.addSpot(SpotType::MEDIUM);
//     floor1.addSpot(SpotType::LARGE);

//     Vehicle car1("WB-02-1111", VehicleType::CAR);
//     Vehicle car2("WB-02-2222", VehicleType::CAR);
//     Vehicle car3("WB-02-3333", VehicleType::CAR);

//     Vehicle* cars[] = { &car1, &car2, &car3 };
//     for (Vehicle* c : cars) {
//         ParkingSpot* spot = floor1.findFreeSpot(*c);
//         if (spot && spot->park(c))
//             cout << c->getLicensePlate() << " -> spot " << spot->getId() << endl;
//         else
//             cout << c->getLicensePlate() << " -> no spot available" << endl;
//     }

//     cout << "Free MEDIUM spots: " << floor1.countFreeSpots(SpotType::MEDIUM) << endl;
//     return 0;
// }



int main() {
    ParkingLot lot("TravPark");
    lot.addFloor(1, 1, 2, 1);   // floor 1: 1 small, 2 medium, 1 large
    lot.addFloor(2, 2, 1, 0);   // floor 2: 2 small, 1 medium, 0 large

    Vehicle c1("WB-01", VehicleType::CAR);
    Vehicle c2("WB-02", VehicleType::CAR);
    Vehicle c3("WB-03", VehicleType::CAR);
    Vehicle t1("WB-T1", VehicleType::TRUCK);

    Vehicle* arrivals[] = { &c1, &c2, &c3, &t1 };
    for (Vehicle* v : arrivals) {
        ParkingSpot* s = lot.parkVehicle(v);
        cout << v->getLicensePlate() << " -> "
             << (s ? "spot " + to_string(s->getId()) : "FULL") << endl;
    }

    lot.printAvailability();

    lot.unparkVehicle("WB-02");
    cout << "\nAfter WB-02 leaves:" << endl;
    lot.printAvailability();
    return 0;
}