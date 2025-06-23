#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip> // For formatting, though not strictly needed for JSON output
#include <limits>  // For numeric_limits, though not strictly needed for JSON output
#include <algorithm>
#include "json.hpp"// Include the JSON library

using namespace std;
using json = nlohmann::json; // Alias for convenience

// This struct is fine as is
struct Transaction {
    string counterparty;
    int amount;
    string type;
};

// We will no longer rely on global 'graph', 'history', 'balance' for the minimization logic
// Instead, we'll calculate balances from the input transactions.

// Function to calculate balances from a list of transactions (new for this setup)
map<string, int> calculateBalancesFromInput(const vector<json>& input_transactions) {
    map<string, int> currentBalances;
    for (const auto& tx_json : input_transactions) {
        // Ensure keys exist and are of correct type
        string payer = tx_json.at("payer").get<string>();
        string receiver = tx_json.at("receiver").get<string>();
        int amount = tx_json.at("amount").get<int>();

        currentBalances[payer] -= amount;
        currentBalances[receiver] += amount;
    }
    return currentBalances;
}

// Function to minimize cash flow and return settlements as JSON
json minimizeCashFlow(const map<string, int>& balances) {
    vector<pair<string, int>> debtors, creditors;

    // Separate into debtors (negative balance) and creditors (positive balance)
    for (const auto& i : balances) {
        if (i.second < 0)
            debtors.push_back({i.first , -i.second}); // Store positive amount owed
        else if (i.second > 0)
            creditors.push_back({i.first, i.second}); // Store positive amount due
    }

    // Sort to potentially optimize (though not strictly necessary for correctness)
    // and make the two-pointer approach efficient.
    // Sorting by amount in descending order helps resolve larger debts/credits first.
    sort(debtors.begin(), debtors.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
    sort(creditors.begin(), creditors.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });


    int i = 0, j = 0;
    json settlements_json = json::array(); // JSON array to store the minimized transactions

    // Two-pointer approach to settle debts
    while (i < debtors.size() && j < creditors.size()) {

                    // For debtors
        string& debtor = debtors[i].first;
        int& owed = debtors[i].second;

        // For creditors
        string& creditor = creditors[j].first;
        int& due = creditors[j].second;

        int settlementAmount = min(owed, due); // The amount to settle in this transaction

        // Record the settlement
        settlements_json.push_back({
            {"from", debtor},
            {"to", creditor},
            {"amount", settlementAmount}
        });

        // Update remaining owed/due amounts
        owed -= settlementAmount;
        due -= settlementAmount;

        // Move to the next debtor/creditor if their amount is fully settled
        if (owed == 0) i++;
        if (due == 0) j++;
    }

    return settlements_json;
}

int main() {
    // This C++ program will now expect JSON input from stdin
    // and will output JSON to stdout.
    try {
        string line;
        string json_input_str;
        // Read all lines from stdin until EOF
        while (getline(cin, line)) {
            json_input_str += line;
        }

        // Parse the incoming JSON string
        json input_data = json::parse(json_input_str);

        // Extract the "transactions" array from the input JSON
        // Ensure that the key "transactions" exists and is an array
        vector<json> transactions_list = input_data.at("transactions").get<vector<json>>();

        // Calculate balances based on the received transactions
        map<string, int> currentBalances = calculateBalancesFromInput(transactions_list);

        // Minimize cash flow using the calculated balances
        json minimized_settlements = minimizeCashFlow(currentBalances);

        // Output the minimized settlements as JSON to stdout
        cout << minimized_settlements.dump() << endl; // `dump()` without argument is compact JSON

    } catch (const json::parse_error& e) {
        // Handle JSON parsing errors
        cerr << "JSON parsing error: " << e.what() << endl;
        return 1; // Indicate error
    } catch (const json::out_of_range& e) {
        // Handle cases where required keys are missing
        cerr << "Missing JSON key or invalid type: " << e.what() << endl;
        return 1; // Indicate error
    } catch (const exception& e) {
        // Catch any other standard exceptions
        cerr << "Runtime error: " << e.what() << endl;
        return 1; // Indicate error
    }

    return 0; // Indicate success
}