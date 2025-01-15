#include <iostream>
#include <fstream>
using namespace std;

void clearConsole() {
    cout << "\033[;H"; // Moves cursor to the top left
    cout << "\033[J"; // Clears the console
}


void startingMessages(char& role) {
    cout << "Welcome!" << endl;
    cout << "Today is 01.01.2025" << endl;
    cout << "Please select an option:" << endl;
    cout << "A - Manager or B - Waiter" << endl;
    while (true) {
        cout << "I am: ";
        cin >> role;
        if (role == 'A' || role == 'a') {
            clearConsole();
            cout << "You selected: Manager" << endl;
            break;
        }
        else if (role == 'B' || role == 'b') {
            clearConsole();
            cout << "You selected: Waiter" << endl;
            break;
        }
        else {
            cout << "Invalid option! Try again!" << endl;
        }
    }
}

void printOptionsForManager() {
    cout << "Select an option:" << endl;
    cout << "1) See menu" << endl;
    cout << "2) Make an order" << endl;
    cout << "3) Remove an order" << endl;
    cout << "4) See past orders" << endl;
    cout << "5) See past oders list" << endl;
    cout << "6) See how much products are left" << endl;
    cout << "7) Write- off a product" << endl;
    cout << "8) Stock a product" << endl;
    cout << "9) See daily revenue" << endl;
    cout << "10) Generate a daily revenue" << endl;
    cout << "11) See all revenues" << endl;
    cout << "12) Add a new item in the menu" << endl;
    cout << "13) Remove an item from the menu" << endl;
    cout << "14) See all options " << endl;
    cout << "15) Exit" << endl;

    int option;
    while (true) {
        cout << "Choosing: ";
        cin >> option;
        if (option < 1 || option > 15) {
            cout << "Invalid option! Select again!" << endl;
        }
        else {
            clearConsole();
            break;
        }
    }

    switch (option) {
    case 1:
        
    case 2:

    case 3:
        
    case 4:
        
    case 5:

    case 6:
        
    case 7:
        
    case 8:

    case 9:
    
    case 10:

    case 11:
        
    case 12:
        
    case 13:

    case 14:
        printOptionsForManager();
    case 15:
        return;
    }
}

void printOptionsForWaiter() {
    while (true) {
        cout << "Select an option:" << endl;
        cout << "1) See menu" << endl;
        cout << "2) Make an order" << endl;
        cout << "3) Remove an order" << endl;
        cout << "4) See past orders" << endl;
        cout << "5) See past orders list" << endl;
        cout << "6) See daily revenue" << endl;
        cout << "7) See all options" << endl;
        cout << "8) Exit" << endl;

        int option;
        bool validOption = false;

        while (!validOption) {
            cout << "Choosing: ";
            cin >> option;

            if (option < 1 || option > 8) {
                cout << "Invalid option! Select again!" << endl;
            }
            else {
                validOption = true;
            }
        }

        clearConsole();

        switch (option) {
        case 1:
            //seeMenu();
            break;
        case 2:
            //makeOrder();
            break;
        case 3:
            //removeOrder();
            break;
        case 4:
            //seePastOrders();
            break;
        case 5:
            //seePastOrdersList();
            break;
        case 6:
            //seeDailyRevenue();
            break;
        case 7:
            continue;
        case 8:
            // Exit the function
            return;
        }
    }
}

int main() {
    char role;
    startingMessages(role);
    if (role == 'A' || role == 'a') {
        printOptionsForManager();
    }
    else if (role == 'B' || role == 'b') {
        printOptionsForWaiter();
    }


    return 0;
}
