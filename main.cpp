#include <iostream>
#include <string>
#include <limits>    // numeric_limits, used when discarding a bad line
#include <cstdlib>   // exit()
using namespace std;

// Ride types the user can choose from.
// Using an enum lets us "switch" on the ride type later (switch cannot use strings).
enum RideType { ECONOMY = 1, PREMIUM = 2, BIKE = 3 };

// ---- Function declarations (prototypes) ----
void showBanner();                 // program title + sample-rates disclaimer
RideType getRideType();            // ask the user for ride type (validated)
double getDistanceKm();            // ask for distance (validated)
double getTimeMinutes();           // ask for time (validated)
bool getPeakHour();                // ask if it is peak hour (validated y/n)
string getPromoCode();             // ask for promo code

// Input helper functions (used by the functions above)
void discardRestOfLine();          // throw away leftover text on the input line
void exitIfInputEnded();           // stop cleanly if input has ended (Ctrl+D / Ctrl+Z)
double readPositiveNumber(const string& prompt, const string& itemName);
string trimSpaces(const string& text);   // remove spaces/tabs from both ends of a string
// Removes leading and trailing spaces/tabs, e.g. "  SAVE10 " becomes "SAVE10".
// A string with only spaces becomes an empty string.
string trimSpaces(const string& text) {
    size_t first = text.find_first_not_of(" \t");
    if (first == string::npos) {
        return "";              // the string was empty or only spaces
    }
    size_t last = text.find_last_not_of(" \t");
    return text.substr(first, last - first + 1);
}

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

// ---- Input helper functions ----

// Throws away everything left on the current input line,
// including the Enter keypress, so the next prompt starts clean.
void discardRestOfLine() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// If the input stream has ended, cin can never succeed again,
// so a retry loop would run forever. Exit cleanly instead.
void exitIfInputEnded() {
    if (cin.eof()) {
        cout << "\nInput ended. Exiting program." << endl;
        exit(1);
    }
}

// Keeps asking until the user enters a number greater than 0.
// Used for both distance and time, so the check is written only once.
double readPositiveNumber(const string& prompt, const string& itemName) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;

        if (cin.fail()) {
            // Wrong type (e.g. letters): reset cin, drop the bad text, ask again
            exitIfInputEnded();
            cin.clear();
            discardRestOfLine();
            cout << "Error: please enter a number for " << itemName << "." << endl;
        } else {
            discardRestOfLine();
            if (value > 0) {
                return value;   // valid: number and greater than 0
            }
            // Wrong value: a number, but zero or negative
            cout << "Error: " << itemName << " must be greater than 0." << endl;
        }
    }
}

// ---- Function definitions ----
void showBanner() {
    cout << "=== Fare Calculator ===" << endl;
}

// Shows a numbered menu and keeps asking until the user picks 1, 2 or 3.
RideType getRideType() {
    int choice;
    cout << "\nSelect ride type:" << endl;
    cout << "  1. Economy" << endl;
    cout << "  2. Premium" << endl;
    cout << "  3. Bike" << endl;

    while (true) {
        cout << "Enter choice (1-3): ";
        cin >> choice;

        if (cin.fail()) {
            // Wrong type (e.g. letters)
            exitIfInputEnded();
            cin.clear();
            discardRestOfLine();
            cout << "Error: please enter a number from 1 to 3." << endl;
        } else {
            discardRestOfLine();
            if (choice >= ECONOMY && choice <= BIKE) {
                return static_cast<RideType>(choice);   // safe: choice is now known to be valid
            }
            // Wrong value: a number outside the menu range
            cout << "Error: choice must be 1, 2 or 3." << endl;
        }
    }
}

// Asks for the trip distance in kilometres (must be greater than 0).
double getDistanceKm() {
    return readPositiveNumber("Enter distance in km: ", "distance");
}

// Asks for the trip duration in minutes (must be greater than 0).
double getTimeMinutes() {
    return readPositiveNumber("Enter time in minutes: ", "time");
}

// Asks whether the ride is during peak hour; only y/Y or n/N are accepted.
bool getPeakHour() {
    string answer;
    while (true) {
        cout << "Is it peak hour? (y/n): ";
        cin >> answer;
        exitIfInputEnded();     // cin only fails on a string when input has ended
        discardRestOfLine();

        if (answer == "y" || answer == "Y") return true;
        if (answer == "n" || answer == "N") return false;
        cout << "Error: please enter y or n." << endl;
    }
}

// Asks for a promo code. Pressing Enter (or typing only spaces) means "no promo code".
string getPromoCode() {
    string code;
    cout << "Enter promo code (press Enter if none): ";
    if (!getline(cin, code)) {
        exitIfInputEnded();     // getline fails only when input has ended
    }

    code = trimSpaces(code);
    if (code.empty()) {
        return "NONE";
    }
    return code;
}