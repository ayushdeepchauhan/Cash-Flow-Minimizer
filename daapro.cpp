#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

using namespace std;

struct Transaction {
    string counterparty;
    int amount;
    string type;
};

map<string, vector<pair<string, int>>> graph;
map<string, vector<Transaction>> history;
map<string, int> balance;

void addTransaction(const string& from, const string& to, int amount) {
    graph[from].push_back({to, amount});
    history[from].push_back({to, amount, "sent"});
    history[to].push_back({from, amount, "received"});
    balance[from] -= amount;
    balance[to] += amount;

    cout << "Transaction added: " << from << " paid " << to << " $" << amount << endl;
}

void showHistory(const string& name) {
    if (!history.count(name)) {
        cout << "No transactions found for " << name << endl;
        return;
    }

    cout << "\nTransaction History for " << name << ":\n";
    cout << left << setw(15) << "Counterparty"
         << setw(10) << "Amount"
         << setw(10) << "Type" << endl;
    cout << string(35, '-') << endl;

    for (const auto& t : history[name]) {
        cout << left << setw(15) << t.counterparty
             << setw(10) << t.amount
             << setw(10) << t.type << endl;
    }
}

void clearBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void settleDebts() {
    vector<pair<string, int>> debtors, creditors;

    for (const auto& [person, bal] : balance) {
        if (bal < 0)
            debtors.push_back({person, -bal});
        else if (bal > 0)
            creditors.push_back({person, bal});
    }

    int i = 0, j = 0, count = 0;

    cout << "\nMinimized Transactions:\n";
    while (i < debtors.size() && j < creditors.size()) {
        auto& [debtor, owed] = debtors[i];
        auto& [creditor, due] = creditors[j];

        int amt = min(owed, due);
        cout << debtor << " pays " << creditor << " $" << amt << endl;

        owed -= amt;
        due -= amt;

        if (owed == 0) i++;
        if (due == 0) j++;

        count++;
    }

    cout << "\nMinimum transactions required: " << count << endl;
}

int main() {
    int choice;
    string from, to;
    int amt;

    cout << "===== Cash Flow Minimizer =====\n";

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add a new transaction\n";
        cout << "2. View transaction history\n";
        cout << "3. Minimize cash flow\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            clearBuffer();
            continue;
        }

        clearBuffer();

        switch (choice) {
            case 1:
                cout << "Enter sender name: ";
                getline(cin, from);

                cout << "Enter receiver name: ";
                getline(cin, to);

                cout << "Enter amount: $";
                if (!(cin >> amt)) {
                    cout << "Invalid amount. Transaction cancelled.\n";
                    clearBuffer();
                    continue;
                }

                clearBuffer();

                if (amt <= 0) {
                    cout << "Amount must be positive. Transaction cancelled.\n";
                    continue;
                }

                addTransaction(from, to, amt);
                break;

            case 2:
                cout << "Enter name to view transaction history: ";
                getline(cin, from);
                showHistory(from);
                break;

            case 3:
                settleDebts();
                break;

            case 4:
                cout << "Exiting program. Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

 return 0;
}
