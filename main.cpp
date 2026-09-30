#include <iostream>
#include <string>
using namespace std;

// Ride types the user can choose from.
// Using an enum lets us "switch" on the ride type later (switch cannot use strings).
enum RideType { ECONOMY = 1, PREMIUM = 2, BIKE = 3 };

// ---- Function declarations (each one gets filled in during a later step) ----
void showBanner();                 // Step 7: program title + sample-rates disclaimer
RideType getRideType();            // Step 2: ask the user for ride type
double getDistanceKm();            // Step 2: ask for distance
double getTimeMinutes();           // Step 2: ask for time
bool getPeakHour();                // Step 2: ask if it is peak hour (y/n)
string getPromoCode();             // Step 2: ask for promo code

int main() {
    showBanner();

    // Placeholder: later steps will collect input, validate, calculate and display.

    return 0;
}

// ---- Function definitions (empty stubs for now) ----
void showBanner() {
    cout << "=== Fare Calculator (Skeleton) ===" << endl;
}

RideType getRideType()   { return ECONOMY; }
double getDistanceKm()   { return 0.0; }
double getTimeMinutes()  { return 0.0; }
bool getPeakHour()       { return false; }
string getPromoCode()    { return ""; }