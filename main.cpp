#include <iostream>
#include <iomanip>   // fixed, setprecision: show money with 2 decimals
#include <string>
#include <limits>    // numeric_limits, used when discarding a bad line
#include <cstdlib>   // exit()
#include <cctype>    // toupper, used to make promo codes case-insensitive
using namespace std;

// Ride types the user can choose from.
// Using an enum lets us "switch" on the ride type (switch cannot use strings).
enum RideType { ECONOMY = 1, PREMIUM = 2, BIKE = 3 };

// Bundles the three pricing numbers for one ride type, so a function can return them together.
struct RateCard {
    double baseFare;        // fixed starting charge (RM)
    double ratePerKm;       // charge for each kilometre (RM)
    double ratePerMinute;   // charge for each minute (RM)
};

// Peak hour surcharge: a percentage of the subtotal.
// NOTE: 20% is a MADE-UP SAMPLE VALUE for this assignment, not a real Grab price.
const double PEAK_SURCHARGE_RATE = 0.20;

// Promo codes and their discounts (percentage of the fare after surcharge).
// NOTE: these codes and percentages are MADE UP for this assignment, not real Grab promotions.
const string PROMO_CODE_SAVE10     = "SAVE10";
const string PROMO_CODE_STUDENT15  = "STUDENT15";
const string PROMO_CODE_WELCOME20  = "WELCOME20";
const double PROMO_RATE_SAVE10     = 0.10;
const double PROMO_RATE_STUDENT15  = 0.15;
const double PROMO_RATE_WELCOME20  = 0.20;
const string NO_PROMO              = "NONE";   // value used when the user enters no promo code

// ---- Function declarations (prototypes) ----
void showBanner();                 // program title + sample-rates disclaimer
RideType getRideType();            // ask the user for ride type (validated)
double getDistanceKm();            // ask for distance (validated)
double getTimeMinutes();           // ask for time (validated)
bool getPeakHour();                // ask if it is peak hour (validated y/n)
string getPromoCode();             // ask for promo code (Enter = none)

// Fare calculation functions
RateCard getRateCard(RideType rideType);    // switch: pick base fare and rates for a ride type
string getRideName(RideType rideType);      // switch: ride type -> readable name
double calculateSubtotal(const RateCard& rates, double distanceKm, double timeMinutes);
double calculatePeakSurcharge(double subtotal, bool isPeakHour);   // surcharge amount (0 if not peak)
double getPromoDiscountRate(const string& promoCode);              // discount rate for a code (0 if not valid)
double calculatePromoDiscount(double fareAfterSurcharge, double discountRate);   // discount amount

// Input helper functions (used by the input functions above)
void discardRestOfLine();          // throw away leftover text on the input line
void exitIfInputEnded();           // stop cleanly if input has ended (Ctrl+D / Ctrl+Z)
double readPositiveNumber(const string& prompt, const string& itemName);
string trimSpaces(const string& text);      // remove spaces/tabs from both ends of a string
string toUpperCase(const string& text);     // convert a string to UPPERCASE

int main() {
    showBanner();

    // Collect all the inputs from the user
    RideType rideType   = getRideType();
    double distanceKm   = getDistanceKm();
    double timeMinutes  = getTimeMinutes();
    bool isPeakHour     = getPeakHour();
    string promoCode    = getPromoCode();

    // Calculate the fare step by step
    RateCard rates            = getRateCard(rideType);
    double subtotal           = calculateSubtotal(rates, distanceKm, timeMinutes);
    double peakSurcharge      = calculatePeakSurcharge(subtotal, isPeakHour);
    double fareAfterSurcharge = subtotal + peakSurcharge;

    // Promo: work out the discount rate, then the discount amount.
    // A rate above 0 means the code was recognised.
    double discountRate   = getPromoDiscountRate(promoCode);
    bool promoIsValid     = (discountRate > 0);
    double promoDiscount  = calculatePromoDiscount(fareAfterSurcharge, discountRate);
    double total          = fareAfterSurcharge - promoDiscount;

    // Temporary check so we can see the values are correct.
    // This block gets replaced by the real fare breakdown in Step 7.
    cout << fixed << setprecision(2);   // always show money with 2 decimal places
    cout << "\n--- Temporary check ---" << endl;
    cout << "Ride type: " << getRideName(rideType) << endl;
    cout << "Distance (km): " << distanceKm << endl;
    cout << "Time (mins): " << timeMinutes << endl;
    cout << "Peak hour: " << (isPeakHour ? "Yes" : "No") << endl;
    cout << "Subtotal: RM " << subtotal << endl;
    cout << "Peak surcharge: RM " << peakSurcharge << endl;
    cout << "Fare after surcharge: RM " << fareAfterSurcharge << endl;

    if (promoIsValid) {
        cout << "Promo " << toUpperCase(promoCode) << " applied. Discount: RM "
             << promoDiscount << endl;
    } else if (promoCode != NO_PROMO) {
        // Something was typed, but it is not a known code: tell the user, continue at full price
        cout << "Promo code \"" << promoCode << "\" is not valid. No discount applied." << endl;
    }
    cout << "TOTAL: RM " << total << endl;

    return 0;
}

// ---- Fare calculation functions ----

// Returns the pricing numbers for the chosen ride type.
// NOTE: these are MADE-UP SAMPLE RATES for this assignment, not real Grab prices.
RateCard getRateCard(RideType rideType) {
    RateCard rates;

    switch (rideType) {
        case ECONOMY:
            rates.baseFare      = 3.00;
            rates.ratePerKm     = 1.20;
            rates.ratePerMinute = 0.25;
            break;
        case PREMIUM:
            rates.baseFare      = 5.00;
            rates.ratePerKm     = 1.80;
            rates.ratePerMinute = 0.40;
            break;
        case BIKE:
            rates.baseFare      = 2.00;
            rates.ratePerKm     = 0.80;
            rates.ratePerMinute = 0.15;
            break;
        default:
            // Should never happen because the input is validated,
            // but this keeps every value initialised.
            rates.baseFare      = 0.0;
            rates.ratePerKm     = 0.0;
            rates.ratePerMinute = 0.0;
            break;
    }

    return rates;
}

// Converts a ride type into text for display.
string getRideName(RideType rideType) {
    switch (rideType) {
        case ECONOMY: return "Economy";
        case PREMIUM: return "Premium";
        case BIKE:    return "Bike";
        default:      return "Unknown";
    }
}

// Subtotal = base fare + (distance x rate per km) + (time x rate per minute).
double calculateSubtotal(const RateCard& rates, double distanceKm, double timeMinutes) {
    return rates.baseFare
         + (distanceKm * rates.ratePerKm)
         + (timeMinutes * rates.ratePerMinute);
}

// Peak hour surcharge = a percentage of the subtotal, or 0 if it is not peak hour.
// Returns the surcharge AMOUNT (not the new total) so the receipt can show it as its own line.
double calculatePeakSurcharge(double subtotal, bool isPeakHour) {
    if (isPeakHour) {
        return subtotal * PEAK_SURCHARGE_RATE;
    }
    return 0.0;
}

// Returns the discount rate for a promo code, or 0 if the code is not recognised.
// Codes are case-insensitive (save10 works the same as SAVE10).
// An if/else chain is used here because switch cannot work on strings.
double getPromoDiscountRate(const string& promoCode) {
    string code = toUpperCase(promoCode);

    if (code == PROMO_CODE_SAVE10) {
        return PROMO_RATE_SAVE10;
    } else if (code == PROMO_CODE_STUDENT15) {
        return PROMO_RATE_STUDENT15;
    } else if (code == PROMO_CODE_WELCOME20) {
        return PROMO_RATE_WELCOME20;
    }

    return 0.0;     // "NONE" or any unrecognised code: no discount
}

// Discount = a percentage of the fare after the peak surcharge.
// Returns the discount AMOUNT so the receipt can show it as its own line.
double calculatePromoDiscount(double fareAfterSurcharge, double discountRate) {
    return fareAfterSurcharge * discountRate;
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

// Converts every letter in the text to uppercase, e.g. "save10" becomes "SAVE10".
string toUpperCase(const string& text) {
    string result = text;
    for (size_t i = 0; i < result.length(); i++) {
        // cast to unsigned char first: toupper is only safe for non-negative values
        result[i] = static_cast<char>(toupper(static_cast<unsigned char>(result[i])));
    }
    return result;
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

// ---- Input and display functions ----
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
// Returns "NONE" in that case, so later steps have one clear value to check.
// (Whether a code is actually valid is checked by getPromoDiscountRate.)
string getPromoCode() {
    string code;
    cout << "Enter promo code (press Enter if none): ";
    if (!getline(cin, code)) {
        exitIfInputEnded();     // getline fails only when input has ended
    }

    code = trimSpaces(code);
    if (code.empty()) {
        return NO_PROMO;
    }
    return code;
}