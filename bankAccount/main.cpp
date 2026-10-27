#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Transaction{
    private:
    string type;
    double amount;
    string date;    
   public:     
    Transaction(string type,double amount,string date){
        this->type = type;
        this->amount = amount;
        this->date = date;
    }

    void printTransaction() const {
        cout << "Transaction: " << type << ", Amount: " << amount << ", Date: " << date << endl;
    }
};

class BankAccount{
    private:
    string owner;
    double balance;

    vector<Transaction> myTransactions;

    public:
    BankAccount(string owner,double balance){
        this->owner = owner;
        this->balance = balance;
    }

    void deposit(double amount){
        if(amount<=0){
            cout<< "Invalid deposit amount." << endl;
        }else{
            balance+=amount;
            cout<< "Deposited: " << amount << endl;
        }
    }

    
    bool withdraw(double amount){
        if(amount<=0||amount>balance){
            cout<<"Invalid withdraw amount"<<endl;
            return false;
        }else{
            balance-=amount;
            cout<<"Withdrawn: "<<amount<<endl;
            return true;
        }
    }
    void transfer(BankAccount &receiver,double amount){
        if(withdraw(amount)){
            receiver.deposit(amount);
            cout<<"Transferred: "<<amount<<" to "<<receiver.getOwner()<<endl;
            cout<<"New balance: "<<balance<<endl;
            myTransactions.push_back(Transaction("Transfer",amount,"2023-01-01"));
        }
    }
    
    double getBalance() const {
        return balance;
    }

    string getOwner(){
        return owner;
    }

    void printTransactions()const {
        for(const auto& transaction:myTransactions){
            transaction.printTransaction();
        }
    }
};


int main() {
    BankAccount account("John", 10000);
    BankAccount other("Asha", 0);

    account.deposit(9090);
    account.withdraw(5000);
    account.transfer(other, 1000);

    cout << "Current balance: " << account.getBalance() << endl;
    cout << "Asha balance: " << other.getBalance() << endl;


    account.printTransactions();

    return 0;
}