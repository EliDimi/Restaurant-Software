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

 char** allocateMemory(int N, int M) {
     char** matrix = new char*[N];
     for (int i = 0; i < N; i++) {
         matrix[i] = new char[M];
     }
     return matrix;
 }

 void deallocateMemory(char** matrix, int N) {
     for (int i = 0; i < N; i++) {
         delete[] matrix[i];
     }
     delete[] matrix;
     matrix = nullptr;
 }

bool compareStrings(const char* str1, const char* str2) {
    int i = 0;
    while (str1[i] != '\0' && str2[i] != '\0') {
        if (str1[i] != str2[i]) {
            return false;
        }
        i++;
    }
    return str1[i] == '\0' && str2[i] == '\0';
}

void copyString(char* destination, const char* source) {
    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

void processWarehouseLine(const char* line, char* product, int& quantity, char* unit) {
    int i = 0, j = 0;
    while (line[i] != '\0') {
        if (line[i] >= '0' && line[i] <= '9') { 
            product[j] = '\0'; 
            break;
        }
        product[j++] = line[i++];
    }
    product[j - 1] = '\0';

    quantity = 0;
    while (line[i] >= '0' && line[i] <= '9') {
        quantity = quantity * 10 + (line[i] - '0');
        i++;
    }

    while (line[i] == ' ') {
        i++;
    }

    j = 0;
    while (line[i] != '\0') {
        unit[j++] = line[i++];
    }
    unit[j] = '\0';
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

bool findWordInFile(const char* word, const char* fileName) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file: " << fileName << endl;
        return false;
    }

    char line[MAXSIZE];
    char dishName[MAXSIZE];
    while (file.getline(line, MAXSIZE)) {
        int i = 0, j = 0;
        while (line[i] != '\0') {
            if (line[i] >= '0' && line[i] <= '9') {
                dishName[j] = '\0';
                break;
            }
            dishName[j++] = line[i++];
        }
        dishName[j - 1] = '\0';

        if (compareStrings(dishName, word)) {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

void removeProductsFromWarehouse(char* orderName, int quantity) {
    const char* recipe = "Recipes.txt";
    ifstream recipeFile(recipe);
    if (!recipeFile.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    recipeFile.seekg(0, ios::end);
    size_t recipeFileSize = recipeFile.tellg();
    recipeFile.seekg(0, ios::beg);
    recipeFile.clear();

    char* recipeContent = new char[recipeFileSize + 1];

    recipeFile.getline(recipeContent, recipeFileSize, '\0');
    recipeFile.close();
    
    const char* warehouse = "Warehouse.txt";
    ifstream warehouseFile(warehouse);
    if (!warehouseFile.is_open()) {
        cout << "Error: Unable to open file." << endl;
        return;
    }

    warehouseFile.seekg(0, ios::end);
    size_t warehouseFileSize = warehouseFile.tellg();
    warehouseFile.seekg(0, ios::beg);
    warehouseFile.clear();

    char* warehouseContent = new char[warehouseFileSize + 1];

    warehouseFile.getline(warehouseContent, warehouseFileSize, '\0');
    warehouseFile.close();



    delete[] recipeContent;
    delete[] warehouseContent;
}

void makeOrder() {
    char orderName[MAXSIZE];
    cout << "Add order's name: ";
    cin.ignore();
    cin.getline(orderName, MAXSIZE);
    clearInputBuffer();

    int quantity = 0;
    cout << "How many: ";
    cin >> quantity;
    clearInputBuffer();

    const char* menuFileName = "Menu.txt";
    bool dishIsFound = findWordInFile(orderName, menuFileName);
    if (!dishIsFound) {
        cout << "Error: Dish not found in the menu!" << endl;
        return;
    }
    const char* recipeFileName = "Recipes.txt";
    bool recipeIsFound = findWordInFile(orderName, recipeFileName);
    if (!recipeIsFound) {
        cout << "Error: Recipe not found!" << endl;
        return;
    }

    cout << "Success" << endl;
    removeProductsFromWarehouse(orderName, quantity);
}

void writeOffProduct() {
    char productName[MAXSIZE];
    cout << "Product to be removed: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(productName, MAXSIZE);
    clearInputBuffer();

    const char* warehouseFileName = "Warehouse.txt";

    ifstream warehouseFile(warehouseFileName);
    if (!warehouseFile.is_open()) {
        cout << "Error: Unable to open warehouse file." << endl;
        return;
    }

    char** products = allocateMemory(MAXSIZE, MAXSIZE);
    int* stock = new int[MAXSIZE];
    char** unit = allocateMemory(MAXSIZE, MAXSIZE);
    int productCount = 0;
    char* line = new char[MAXSIZE];

    while (warehouseFile.getline(line, MAXSIZE)) {
        processWarehouseLine(line, products[productCount], stock[productCount], unit[productCount]);
        productCount++;
    }
    warehouseFile.close();

    bool productFound = false;
    int removeIndex = -1;

    for (int i = 0; i < productCount; i++) {
        if (compareStrings(products[i], productName)) {
            productFound = true;
            removeIndex = i;
            break;
        }
    }
    if (!productFound) {
        cout << "Product not found in the warehouse!" << endl;
        return;
    }

    for (int i = removeIndex; i < productCount - 1; i++) {
        copyString(products[i], products[i + 1]);
        stock[i] = stock[i + 1];
        copyString(unit[i], unit[i + 1]);
    }
    productCount--;

    ofstream warehouseOut(warehouseFileName, ios::trunc);
    if (!warehouseOut.is_open()) {
        cout << "Error: Unable to open warehouse file for writing." << endl;
        return;
    }

    for (int i = 0; i < productCount; i++) {
        warehouseOut << products[i] << " " << stock[i] << " " << unit[i] << endl;
    }
    warehouseOut.close();

    cout << "Product \"" << productName << "\" removed from the warehouse successfully!" << endl;

    deallocateMemory(products, MAXSIZE);
    deallocateMemory(unit, MAXSIZE);
    delete[] stock;
    delete[] line;
}

void stockProduct() {
    char productToAdd[MAXSIZE];
    cout << "Format: 'product name*space*quintity*space*unit'" << endl;
    cout << "Enter product to be added: ";
    cin.ignore();
    cin.getline(productToAdd, MAXSIZE);
    clearInputBuffer();

    char product[MAXSIZE];
    int quantity;
    char unit[MAXSIZE];
    processWarehouseLine(productToAdd, product, quantity, unit);

    const char* warehouseFileName = "Warehouse.txt";

    ifstream warehouseFile(warehouseFileName);
    if (!warehouseFile.is_open()) {
        cout << "Error: Unable to open warehouse file." << endl;
        return;
    }
    
    char** productInWarehouse = allocateMemory(MAXSIZE, MAXSIZE);
    int* stockInWarehouse = new int[MAXSIZE];
    char** unitInWarehouse = allocateMemory(MAXSIZE, MAXSIZE);
    int productCount = 0;
    char* line = new char[MAXSIZE];

    while (warehouseFile.getline(line, MAXSIZE)) {
        processWarehouseLine(line, productInWarehouse[productCount], stockInWarehouse[productCount], unitInWarehouse[productCount]);
        productCount++;
    }
    warehouseFile.close();

    bool productFound = false;
    for (int i = 0; i < productCount; i++) {
        if (compareStrings(productInWarehouse[i], product) && compareStrings(unitInWarehouse[i], unit)) {
            productFound = true;
            stockInWarehouse[i] += quantity;
            break;
        }
    }

    if (!productFound) {
        copyString(productInWarehouse[productCount], product);
        stockInWarehouse[productCount] = quantity;
        copyString(unitInWarehouse[productCount], unit);
        productCount++;
    }

    ofstream warehouseOut(warehouseFileName);
    if (!warehouseOut.is_open()) {
        cout << "Error: Unable to open warehouse file for writing." << endl;
        return;
    }
    for (int i = 0; i < productCount; i++) {
        warehouseOut << productInWarehouse[i] << ' ' << stockInWarehouse[i] << " " << unitInWarehouse[i] << endl;
    }
    warehouseOut.close();

    cout << "Product \"" << product << "\" updated successfully in the warehouse!" << endl;

    deallocateMemory(productInWarehouse, MAXSIZE);
    deallocateMemory(unitInWarehouse, MAXSIZE);
    delete[] stockInWarehouse;
    delete[] line;
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
        cout << "7) Write-off a product" << endl;
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
            writeOffProduct();
            break;
        case 8:
            stockProduct();
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
