#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <ctime>
#include <vector>
#include <map> // Changed from unordered_map to map

using namespace std;

struct Transaction {
    int userId;
    double amount;
    time_t timestamp;
};

class FraudMonitor {
private:
    queue<Transaction> buffer;
    // Map is compatible with older Dev-C++ versions
    map<int, stack<Transaction> > userHistories; 
    
    const int TIME_THRESHOLD = 2;
    const double VOLUME_THRESHOLD = 5.0;

public:
    void receiveRequest(int id, double amt) {
        Transaction t;
        t.userId = id;
        t.amount = amt;
        t.timestamp = time(0);
        
        buffer.push(t);
        cout << ">> Transaction queued for User " << id << "..." << endl;
    }

    void processNext() {
        if (buffer.empty()) return;

        Transaction current = buffer.front();
        buffer.pop();

        cout << "\nAnalyzing Transaction for User " << current.userId << ":" << endl;

        // Fetch the stack for this specific user
        stack<Transaction>& history = userHistories[current.userId];

        if (!history.empty()) {
            Transaction last = history.top();

            // Compare only against this user's previous transaction
            double timeGap = difftime(current.timestamp, last.timestamp);
            if (timeGap <= TIME_THRESHOLD) {
                cout << "[ALERT] High-Frequency! Gap: " << timeGap << "s" << endl;
            }

            if (current.amount >= last.amount * VOLUME_THRESHOLD) {
                cout << "[ALERT] Volume Shift! (Current: $" << current.amount 
                     << " vs Last: $" << last.amount << ")" << endl;
            }
        }

        history.push(current);
        cout << "Transaction processed successfully." << endl;
    }
};

int main() {
    FraudMonitor monitor;
    int choice;
    
    cout << "--- Fraud Detection System Simulation ---" << endl;

    while (true) {
        cout << "\n1. New Transaction\n2. Process Pending\n3. Exit\nChoice: ";
        if (!(cin >> choice)) break;

        if (choice == 1) {
            int id;
            double amt;
            cout << "Enter User ID and Amount: ";
            cin >> id >> amt;
            monitor.receiveRequest(id, amt);
        } else if (choice == 2) {
            monitor.processNext();
        } else {
            break;
        }
    }

    return 0;
}

