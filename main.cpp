#include <iostream>
#include <string>
using namespace std;

// Ride types the user can choose from.
// Using an enum lets us "switch" on the ride type later (switch cannot use strings).
enum RideType { ECONOMY = 1, PREMIUM = 2, BIKE = 3 };

// ---- Function declarations (prototypes) ----
void showBanner();                 // Step 7: program title + sample-rates disclaimer
RideType getRideType();            // ask the user for ride type
double getDistanceKm();            // ask for distance
double getTimeMinutes();           // ask for time
bool getPeakHour();                // ask if it is peak hour (y/n)
string getPromoCode();             // ask for promo code

int main() {
    showBanner();

    // Collect all the inputs from the user
    RideType rideType   = getRideType();
    double distanceKm   = getDistanceKm();
    double timeMinutes  = getTimeMinutes();
    bool isPeakHour     = getPeakHour();
    string promoCode    = getPromoCode();

    // Temporary check so we can see the inputs were stored correctly.
    // This block gets replaced by the real calculation in later steps.
    cout << "\n--- Inputs received (temporary check) ---" << endl;
    cout << "Ride type (number): " << rideType << endl;
    cout << "Distance (km): " << distanceKm << endl;
    cout << "Time (mins): " << timeMinutes << endl;
    cout << "Peak hour: " << (isPeakHour ? "Yes" : "No") << endl;
    cout << "Promo code: " << promoCode << endl;

    return 0;
}

// ---- Function definitions ----
void showBanner() {
    cout << "=== Fare Calculator ===" << endl;
}

// Shows a numbered menu and converts the user's number into a RideType.
RideType getRideType() {
    int choice;
    cout << "\nSelect ride type:" << endl;
    cout << "  1. Economy" << endl;
    cout << "  2. Premium" << endl;
    cout << "  3. Bike" << endl;
    cout << "Enter choice (1-3): ";
    cin >> choice;
    return static_cast<RideType>(choice);   // number -> enum (validated in Step 3)
}

// Asks for the trip distance in kilometres.
double getDistanceKm() {
    double distance;
    cout << "Enter distance in km: ";
    cin >> distance;
    return distance;
}

// Asks for the trip duration in minutes.
double getTimeMinutes() {
    double minutes;
    cout << "Enter time in minutes: ";
    cin >> minutes;
    return minutes;
}

// Asks whether the ride is during peak hour; 'y' or 'Y' means yes.
bool getPeakHour() {
    char answer;
    cout << "Is it peak hour? (y/n): ";
    cin >> answer;
    return (answer == 'y' || answer == 'Y');
}

// Asks for a promo code. The user types NONE if they don't have one.
string getPromoCode() {
    string code;
    cout << "Enter promo code (or NONE): ";
    cin >> code;
    return code;
}