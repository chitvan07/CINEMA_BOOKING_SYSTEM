#include <iostream>
#include <fstream>
#include <string>
using namespace std;


template <class T>
T giveDiscount(T amt, T p) {
    return amt - (amt * p / 100.0);
}

class Person {
protected:
    string name;
public:
    Person(string n="Guest") { name = n; }

    virtual void display() {
        cout << "Name: " << name << endl;
    }

    string getName() const { 
        return name;
    }
};

class Movie {
    string title;
    int time;
public:
    Movie(string t="No Movie", int tm=120) { 
        title = t; 
        time = tm; 
    }

    string getTitle() const { return title; }
};


class Customer : public Person {
    int age;
public:
    Customer(string n="Guest", int a=18) : Person(n) {
        age = a;
    }

    void display() {
        cout << "Customer Name: " << name << " | Age: " << age << endl;
    }
};


class Booking;

class SeatManager {
public:
    friend class Booking;

private:
    bool seats[10];
public:
    SeatManager() {
        for (int i = 0; i < 10; i++) seats[i] = false;
    }
};


class Booking {
    static int totalBookings;
    int bookingID;
    Customer customer;
    Movie movie;
    SeatManager *sm;
    int seatNo;
    float price;

public:

    Booking(Customer c, Movie m, int s, float p)
        : customer(c), movie(m)
    {
        sm = new SeatManager;
        seatNo = s;
        price = p;
        bookingID = ++totalBookings;
        sm->seats[s] = true;
    }

    ~Booking() {
        delete sm;
    }

    bool operator==(Booking &b) {
        return this->seatNo == b.seatNo;
    }

    void saveToFile() {
        ofstream fout("bookings.txt", ios::app);
        fout << "BookingID: " << bookingID 
             << " | Name: " << customer.getName()
             << " | Movie: " << movie.getTitle()
             << " | Seat: " << seatNo
             << " | Price: " << price 
             << endl;
        fout.close();
    }

    void showBooking() {
        cout << "\n----- BOOKING DETAILS -----\n";
        cout << "Booking ID: " << bookingID << endl;
        customer.display();
        cout << "Movie: " << movie.getTitle()
             << " | Seat: " << seatNo
             << " | Price: " << price << endl;
    }

    static void showTotalBookings() {
        cout << "\nTotal Bookings: " << totalBookings << endl;
    }
};

int Booking::totalBookings = 0;


Movie showMovieMenu(float &price) {
    int choice;

    cout << "\n====== MOVIE MENU ======\n";
    cout << "1. Avengers: Endgame   - Rs. 300\n";
    cout << "2. Pushpa 2            - Rs. 250\n";

    cout << "\nEnter movie choice (1-2): ";
    cin >> choice;

    switch (choice) {
        case 1: price = 300; return Movie("Avengers: Endgame", 180);
        case 2: price = 250; return Movie("Pushpa 2", 170);

        default:
            cout << "Invalid choice! Default movie selected.\n";
            price = 200;
            return Movie("Default Movie", 120);
    }
}


int main() {
    try {
        cout << "---- - BOOKING SYSTEM ----\n";

        string name;
        int age, seat;
        float price, moviePrice;

        cout << "Enter Your Name: ";
        getline(cin, name);

        cout << "Enter Your Age: ";
        cin >> age;

        Movie m = showMovieMenu(moviePrice);
        price = moviePrice;

        cout << "Enter Seat Number (0-9): ";
        cin >> seat;

        if (seat < 0 || seat > 9)
            throw "Invalid seat number! Must be between 0 and 9.";

        Customer c(name, age);

       
        Booking b(c, m, seat, price);

       
        float finalPrice = giveDiscount(price, 10.0f);
        cout << "\nPrice after 10% discount: " << finalPrice << endl;

        
        b.saveToFile();
        b.showBooking();
        Booking::showTotalBookings();

        return 0;
    }
    catch (const char* msg) {
        cout << "Error: " << msg << endl;
    }
}
