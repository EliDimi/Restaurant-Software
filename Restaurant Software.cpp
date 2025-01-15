#include <iostream>
#include <fstream>
using namespace std;

const int MAXSIZE = 100;

void clearConsole() {
    cout << "\033[;H"; // Moves cursor to the top left
    cout << "\033[J"; // Clears the console
}

void clearInputBuffer() {
    // because of using both getline and cin we have to cin.ignore;
    // cin leaves the newline character in the stream which will be read as input from the getline

    cin.clear();	// clears errors flags from the cin
    cin.sync();		// discard unread characters from the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');	// discard characters from the input buffer
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

void readAndPrintFile(const char* List) {
    ifstream file(List);
    if (!file.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    file.seekg(0, ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, ios::beg);
    file.clear();

    char* fileContent = new char[fileSize + 1];

    file.getline(fileContent, fileSize, '\0');

    cout << "The content is: " << endl;
    cout << fileContent << endl;
    cout << endl;


    delete[] fileContent;
    file.close();
}

void seeMenu() {
    const char* menu = "Menu.txt";
    readAndPrintFile(menu);
}

void seeProductsLeft() {
    const char* warehouse = "Warehouse.txt";
    readAndPrintFile(warehouse);
}

void makeOrder() {
    char orderName[MAXSIZE];
    cout << "Add order's name: ";
    cin.getline(orderName, MAXSIZE);
    clearInputBuffer();

    int quantity = 0;
    cout << "How many: ";
    cin >> quantity;
    clearInputBuffer();

}

void printOptionsForManager() {
    while (true) {
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
        bool validOption = false;

        while (!validOption) {
            cout << "Choosing: ";
            cin >> option;

            if (cin.fail()) { 
                cin.clear(); 
                cin.ignore();
                cout << "Invalid input! Please enter a number." << endl;
            } else if (option < 1 || option > 15) {
                cout << "Invalid option! Select again!" << endl;
            }
            else {
                validOption = true;
            }
        }

        clearConsole();

        switch (option) {
        case 1:
            seeMenu();
            break;
        case 2:
            makeOrder();
            break;
        case 3:
            // removeOrder();
            break;
        case 4:
            // seePastOrders();
            break;
        case 5:
            // seePastOrdersList();
            break;
        case 6:
            seeProductsLeft();
            break;
        case 7:
            // writeOffProduct();
            break;
        case 8:
            // stockProduct();
            break;
        case 9:
            // seeDailyRevenue();
            break;
        case 10:
            // generateDailyRevenue();
            break;
        case 11:
            // seeAllRevenues();
            break;
        case 12:
            // addNewItem();
            break;
        case 13:
            // removeItemFromMenu();
            break;
        case 14:
            printOptionsForManager();
            break;
        case 15:
            return;
        }
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

            if (cin.fail()) {
                cin.clear();
                cin.ignore();
                cout << "Invalid input! Please enter a number." << endl;
            }
            else if (option < 1 || option > 15) {
                cout << "Invalid option! Select again!" << endl;
            }
            else {
                validOption = true;
            }
        }

        clearConsole();

        switch (option) {
        case 1:
            seeMenu();
            break;
        case 2:
            makeOrder();
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
