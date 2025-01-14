#include <iostream>
using namespace std;

const int ROLEMAXSIZE = 2;

void startingMessages(char* role) {
    if (role == nullptr) {
        cout << "Error! You have problem with role!" << endl;
        return;
    }
    cout << "Welcome!" << endl;
    cout << "Today is 01.01.2025" << endl;
    cout << "Please select an option:" << endl;
    cout << "A - Manager or B - Waiter" << endl;
    while (true) {
        cout << "I am: ";
        cin.getline(role, ROLEMAXSIZE);
        if (role[0] == 'A' || role[0] == 'a') {
            cout << "You selected: Manager" << endl;
            break;
        }
        else if (role[0] == 'B' || role[0] == 'b') {
            cout << "You selected: Waiter" << endl;
            break;
        }
        else {
            cout << "Invalid option! Try again!" << endl;
        }
    }
}

int main() {
    char role[ROLEMAXSIZE];
    startingMessages(role);

    return 0;
}
