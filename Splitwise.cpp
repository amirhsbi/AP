#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

struct Expense {
    string payer;
    vector<pair<string, float>> borrowers; 
    float amount;
};

class Splitwise {
private:
    map<string, float> balances; 
    vector<Expense> expenses; 

public:
    
    void addParticipant(string name) {
        if (balances.find(name) == balances.end()) {
            balances[name] = 0.0; 
            cout << "Participant '" << name << "' added successfully." << endl;
        } else {
            cout << "Participant '" << name << "' already exists." << endl;
        }
    }


    void removeParticipant(string name) {
        if (balances.find(name) != balances.end()) {
            balances.erase(name);
            cout << "Participant '" << name << "' removed successfully." << endl;
        } else {
            cout << "Participant '" << name << "' does not exist." << endl;
        }
    }


    void addExpense(const Expense& expense) {
        float share = expense.amount / expense.borrowers.size(); 
        
        balances[expense.payer] -= expense.amount; 
        for(const auto& borrower : expense.borrowers) {
            balances[borrower.first] += borrower.second * share;
        }
        expenses.push_back(expense); 
        cout << "Expense added successfully." << endl;
    }

    
    void editExpense(int index) {
        if (index >= 0 && index < expenses.size()) {
            Expense& expense = expenses[index];
            
            balances[expense.payer] += expense.amount;
            cout << "Enter new amount for the expense: ";
            cin >> expense.amount;
            float share = expense.amount / expense.borrowers.size();
           
            balances[expense.payer] -= expense.amount;
            for (auto& borrower : expense.borrowers) {
                balances[borrower.first] += borrower.second * share;
            }
            cout << "Expense edited successfully." << endl;
        } else {
            cout << "Invalid expense index." << endl;
        }
    }



void optimize() {
    vector<pair<string, float>> positive, negative;
    for(const auto& balance : balances) {
        if(balance.second > 0)
            positive.push_back(balance);
        else if(balance.second < 0)
            negative.push_back({balance.first, -balance.second});
    }
    
    sort(positive.begin(), positive.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    sort(negative.begin(), negative.end(), [](const auto& a, const auto& b) { return a.second < b.second; });
    
    for(auto& debtor : negative) {
        for(auto& creditor : positive) {
            float toSettle = min(debtor.second, creditor.second);
            cout << debtor.first << " pays " << toSettle << " to " << creditor.first << endl;
            balances[debtor.first] += toSettle; 
            balances[creditor.first] -= toSettle; 
            debtor.second -= toSettle;
            creditor.second -= toSettle;
            if(debtor.second == 0)
                break;
        }
    }
}


    void showExpenseSummary() {
        cout << "Expense Summary:" << endl;
        for (size_t i = 0; i < expenses.size(); ++i) {
            const Expense& expense = expenses[i];
            cout << i << ". Payer: " << expense.payer << ", Amount: " << expense.amount << ", Borrowers:";
            for (const auto& borrower : expense.borrowers) {
                cout << " (" << borrower.first << ": " << borrower.second << ")";
            }
            cout << endl;
        }
    }


    void showBalances() {
        cout << "Current balances:" << endl;
        for(const auto& balance : balances) {
            cout << balance.first << ": " << balance.second << endl;
        }
    }
};


int main() {
    Splitwise splitwise;
    int choice;
    do {
        cout << "\nSplitwise Menu:\n"
             << "1. Add participant\n"
             << "2. Remove participant\n"
             << "3. Add expense\n"
             << "4. Edit expense\n"
             << "5. Optimize debts\n"
             << "6. Show expense summary\n"
             << "7. Show balances\n"
             << "8. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name;
                cout << "Enter participant name: ";
                cin >> name;
                splitwise.addParticipant(name); 
                break;
            }
            case 2: {
                string name;
                cout << "Enter participant name to remove: ";
                cin >> name;
                splitwise.removeParticipant(name); 
                break;
            }
            case 3: {
                Expense expense;
                cout << "Enter payer name: ";
                cin >> expense.payer;
                cout << "Enter amount: ";
                cin >> expense.amount;
                int numBorrowers;
                cout << "Enter number of borrowers: ";
                cin >> numBorrowers;
                for(int i = 0; i < numBorrowers; ++i) {
                    string borrowerName;
                    float borrowerShare;
                    cout << "Enter borrower name and their share: ";
                    cin >> borrowerName >> borrowerShare;
                    expense.borrowers.push_back({borrowerName, borrowerShare});
                }
                splitwise.addExpense(expense); 
                break;
            }
            case 4: {
                int index;
                cout << "Enter index of expense to edit: ";
                cin >> index;
                splitwise.editExpense(index); 
                break;
            }
            case 5:
                splitwise.optimize(); 
                break;
            case 6:
                splitwise.showExpenseSummary(); 
                break;
            case 7:
                splitwise.showBalances(); 
                break;
            case 8:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please enter a number from 1 to 8." << endl;
        }
    } while (choice != 8);

    return 0;
}
