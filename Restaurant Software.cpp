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

char** allocateMemory(int N) {
     char** matrix = new char*[N];
     for (int i = 0; i < N; i++) {
         matrix[i] = new char[N];
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

void processPastOrderLine(char** orders, int* orderQuantities, int& orderCount) {
    const char* ordersFileName = "Past orders.txt";
    ifstream ordersFile(ordersFileName);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file!" << endl;
        return;
    }

    char line[MAXSIZE];

    while (ordersFile.getline(line, MAXSIZE)) {
        int i = 0, j = 0;

        while (line[i] != '\0' && !(line[i] >= '0' && line[i] <= '9')) {
            orders[orderCount][j++] = line[i++];
        }
        orders[orderCount][j - 1] = '\0';

        orderQuantities[orderCount] = 0;
        while (line[i] >= '0' && line[i] <= '9') {
            orderQuantities[orderCount] = orderQuantities[orderCount] * 10 + (line[i] - '0');
            i++;
        }

        orderCount++;
    }
}

bool updateOrdersFile(const char* orderName, int quantity) {
    char** orders = allocateMemory(MAXSIZE);
    int* orderQuantities = new int[MAXSIZE];
    int orderCount = 0;

    processPastOrderLine(orders, orderQuantities, orderCount);

    for (int i = orderCount - 1; i >= 0; i--) {
        if (compareStrings(orders[i], orderName)) {
            if (orderQuantities[i] == quantity) {
                for (int j = i; j < orderCount - 1; j++) {
                    copyString(orders[j], orders[j + 1]);
                    orderQuantities[j] = orderQuantities[j + 1];
                }
                orderCount--;
            }
            else if (orderQuantities[i] > quantity) {
                orderQuantities[i] -= quantity;
            }
            else {
                cout << "Error: You are trying to cancel more than ordered!" << endl;
                deallocateMemory(orders, MAXSIZE);
                delete[] orderQuantities;
                return false;
            }
            break;
        }
    }

    const char* ordersFileName = "Past orders.txt";
    ofstream ordersOut(ordersFileName);
    if (!ordersOut.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        deallocateMemory(orders, MAXSIZE);
        delete[] orderQuantities;
        return false;
    }

    for (int i = 0; i < orderCount; i++) {
        ordersOut << orders[i] << " " << orderQuantities[i] << endl;
    }
    ordersOut.close();

    return true;
}

bool writeWarehouseToFile(const char* warehouseFileName, char** warehouseProducts, int* warehouseStock, char** warehouseUnits, int& warehouseCount) {
    ofstream warehouseOut(warehouseFileName);
    if (!warehouseOut.is_open()) {
        cout << "Error: Unable to open warehouse file for writing." << endl;
        return false;
    }

    for (int i = 0; i < warehouseCount; i++) {
        if (warehouseStock[i] > 0) {
            warehouseOut << warehouseProducts[i] << " " << warehouseStock[i] << " " << warehouseUnits[i] << endl;
        }
    }

    warehouseOut.close();
    return true;
}

void swapStrings(char* str1, char* str2) {
    char temp[MAXSIZE];
    copyString(temp, str1);
    copyString(str1, str2);
    copyString(str2, temp);
}

void swapIntegers(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int compareStringsForSorting(const char* str1, const char* str2) {
    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void bubbleSort(char** arr, int* quantities, int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareStringsForSorting(arr[j], arr[j + 1]) > 0) {
                swapStrings(arr[j], arr[j + 1]);
                swapIntegers(quantities[j], quantities[j + 1]);
            }
        }
    }
}

void sortPastOrders() {
    char** orders = allocateMemory(MAXSIZE);
    int* orderQuantities = new int[MAXSIZE];
    int orderCount = 0;

    processPastOrderLine(orders, orderQuantities, orderCount);

    char** sortedOrders = allocateMemory(MAXSIZE);
    int* sortedQuantities = new int[MAXSIZE];
    int sortedCount = 0;

    for (int i = 0; i < orderCount; i++) {
        bool found = false;
        for (int j = 0; j < sortedCount; j++) {
            if (compareStrings(sortedOrders[j], orders[i])) {
                sortedQuantities[j] += orderQuantities[i];
                found = true;
                break;
            }
        }
        if (!found) {
            copyString(sortedOrders[sortedCount], orders[i]);
            sortedQuantities[sortedCount] = orderQuantities[i];
            sortedCount++;
        }
    }

    bubbleSort(sortedOrders, sortedQuantities, sortedCount);

    const char* sortedOrdersFile = "Past orders sorted.txt";
    ofstream sortedOrdersOut(sortedOrdersFile);
    if (!sortedOrdersOut.is_open()) {
        cout << "Error: Unable to open sorted orders file for writing!" << endl;
        deallocateMemory(orders, MAXSIZE);
        delete[] orderQuantities;
        deallocateMemory(sortedOrders, MAXSIZE);
        delete[] sortedQuantities;
        return;
    }

    for (int i = 0; i < sortedCount; i++) {
        sortedOrdersOut << sortedOrders[i] << " " << sortedQuantities[i] << endl;
    }
    sortedOrdersOut.close();

    deallocateMemory(orders, MAXSIZE);
    delete[] orderQuantities;
    deallocateMemory(sortedOrders, MAXSIZE);
    delete[] sortedQuantities;
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

void extractRecipeIngredients(const char* orderName, char** productsFromRecipe, int* quantities, int& productCount) {
    const char* recipeFileName = "Recipes.txt";
    ifstream recipeFile(recipeFileName);
    if (!recipeFile.is_open()) {
        cout << "Error: Unable to open recipe file." << endl;
        return;
    }

    char line [MAXSIZE];
    while (recipeFile.getline(line, MAXSIZE)) {
        char dishFound[MAXSIZE];
        int i = 0, j = 0;

        while (line[i] != '\0' && !(line[i] >= '0' && line[i] <= '9')) {
            dishFound[j++] = line[i++];
        }
        dishFound[j - 1] = '\0';

        if (!compareStrings(dishFound, orderName)) {
            continue;
        }

        while (line[i] != '\0') {
            quantities[productCount] = 0;
            while (line[i] >= '0' && line[i] <= '9') {
                quantities[productCount] = quantities[productCount] * 10 + (line[i] - '0');
                i++;
            }

            j = 0;
            while (line[i] != '\0' && !(line[i] >= '0' && line[i] <= '9')) {
                productsFromRecipe[productCount][j++] = line[i++];
            }
            if (line[i] == '\0') {
                productsFromRecipe[productCount][j] = '\0';
            }
            else {
                productsFromRecipe[productCount][j - 1] = '\0';
            }
            productCount++;
        }
        break;
    }
    recipeFile.close();
}

bool removeProductsFromWarehouse(char** products, int* quantities, int& productCount, int quantity) {
    const char* warehouseFileName = "Warehouse.txt";
    ifstream warehouseFile(warehouseFileName);
    if (!warehouseFile.is_open()) {
        cout << "Error: Unable to open warehouse file." << endl;
        return false;
    }

    char** warehouseProducts = allocateMemory(MAXSIZE);
    int* warehouseStock = new int[MAXSIZE];
    char** warehouseUnits = allocateMemory(MAXSIZE);
    int warehouseCount = 0;
    char line[MAXSIZE];

    while (warehouseFile.getline(line, MAXSIZE)) {
        processWarehouseLine(line, warehouseProducts[warehouseCount], warehouseStock[warehouseCount], warehouseUnits[warehouseCount]);
        warehouseCount++;
    }
    warehouseFile.close();

    for (int i = 0; i < productCount; i++) {
        bool found = false;
        for (int j = 0; j < warehouseCount; j++) {
            if (compareStrings(products[i], warehouseProducts[j])) {
                found = true;
                if (warehouseStock[j] < (quantities[i] * quantity)) {
                    cout << "Error: Not enough " << products[i] << " in the warehouse!" << endl;
                    deallocateMemory(warehouseProducts, MAXSIZE);
                    deallocateMemory(warehouseUnits, MAXSIZE);
                    delete[] warehouseStock;
                    return false; 
                }
                break;
            }
        }
        if (!found) {
            cout << "Error: Product " << products[i] << " not found in the warehouse!" << endl;
            deallocateMemory(warehouseProducts, MAXSIZE);
            deallocateMemory(warehouseUnits, MAXSIZE);
            delete[] warehouseStock;
            return false; 
        }
    }

    for (int i = 0; i < productCount; i++) {
        for (int j = 0; j < warehouseCount; j++) {
            if (compareStrings(products[i], warehouseProducts[j])) {
                warehouseStock[j] -= (quantities[i] * quantity);
                break;
            }
        }
    }

    if (!writeWarehouseToFile(warehouseFileName, warehouseProducts, warehouseStock, warehouseUnits, warehouseCount)) {
        deallocateMemory(warehouseProducts, MAXSIZE);
        deallocateMemory(warehouseUnits, MAXSIZE);
        delete[] warehouseStock;
        return false;
    }
    deallocateMemory(warehouseProducts, MAXSIZE);
    deallocateMemory(warehouseUnits, MAXSIZE);
    delete[] warehouseStock;
    return true;
}

void removeLineFromFile(const char* fileName, const char* searchLine) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file: " << fileName << endl;
        return;
    }

    char lines[MAXSIZE][MAXSIZE];
    int lineCount = 0;
    char line[MAXSIZE];

    while (file.getline(line, MAXSIZE)) {
        if (!compareStrings(line, searchLine)) {
            copyString(lines[lineCount], line);
            lineCount++;
        }
    }
    file.close();

    ofstream outFile(fileName);
    if (!outFile.is_open()) {
        cout << "Error: Unable to open file for writing: " << fileName << endl;
        return;
    }

    for (int i = 0; i < lineCount; i++) {
        outFile << lines[i] << endl;
    }
    outFile.close();
}

bool restoreProductsToWarehouse(char** products, int* quantities, int productCount, int restoreQuantity) {
    const char* warehouseFileName = "Warehouse.txt";
    ifstream warehouseFile(warehouseFileName);
    if (!warehouseFile.is_open()) {
        cout << "Error: Unable to open warehouse file!" << endl;
        return false;
    }

    char** warehouseProducts = allocateMemory(MAXSIZE);
    int* warehouseStock = new int[MAXSIZE];
    char** warehouseUnits = allocateMemory(MAXSIZE);
    int warehouseCount = 0;
    char line[MAXSIZE];

    while (warehouseFile.getline(line, MAXSIZE)) {
        processWarehouseLine(line, warehouseProducts[warehouseCount], warehouseStock[warehouseCount], warehouseUnits[warehouseCount]);
        warehouseCount++;
    }
    warehouseFile.close();

    for (int i = 0; i < productCount; i++) {
        bool found = false;
        for (int j = 0; j < warehouseCount; j++) {
            if (compareStrings(products[i], warehouseProducts[j])) {
                warehouseStock[j] += (quantities[i] * restoreQuantity);
                found = true;
                break;
            }
        }

        if (!found) {
            copyString(warehouseProducts[warehouseCount], products[i]);
            warehouseStock[warehouseCount] = quantities[i] * restoreQuantity;
            warehouseCount++;
        }
    }

    if (!writeWarehouseToFile(warehouseFileName, warehouseProducts, warehouseStock, warehouseUnits, warehouseCount)) {
        deallocateMemory(warehouseProducts, MAXSIZE);
        deallocateMemory(warehouseUnits, MAXSIZE);
        delete[] warehouseStock;
        return false;
    }

    deallocateMemory(warehouseProducts, MAXSIZE);
    deallocateMemory(warehouseUnits, MAXSIZE);
    delete[] warehouseStock;
    return true;
}

void saveOrderToFile(const char* orderName, const int quantity) {
    const char* ordersFileName("Past Orders.txt");
    ofstream ordersFile(ordersFileName, ios::app);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    ordersFile << orderName << " " << quantity << endl;
    ordersFile.close();

    cout << "Order done!" << endl;
}

void saveDishToFile(const char* dish, const char* price) {
    const char* ordersFileName("Menu.txt");
    ofstream ordersFile(ordersFileName, ios::app);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    ordersFile << dish << " " << price << endl;
    ordersFile.close();

    cout << "Dish is added!" << endl;
}

void saveRecipeToFile(const char* recipe) {
    const char* ordersFileName("Recipes.txt");
    ofstream ordersFile(ordersFileName, ios::app);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    ordersFile << recipe << endl;
    ordersFile.close();

    cout << "Recipe is added!" << endl;
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

    char** productsFromRecipe = allocateMemory(MAXSIZE);
    int* quantitiesFromRecipe = new int[MAXSIZE];
    int productCount = 0;

    extractRecipeIngredients(orderName, productsFromRecipe, quantitiesFromRecipe, productCount);

    if (!removeProductsFromWarehouse(productsFromRecipe, quantitiesFromRecipe, productCount, quantity)) {
        cout << "Order cannot be completed due to insufficient ingredients." << endl;
    }
    else {
        cout << "Order is cooking...!" << endl;
    }

    saveOrderToFile(orderName, quantity);

    deallocateMemory(productsFromRecipe, MAXSIZE);
    delete[] quantitiesFromRecipe;
}

void cancelOrder() {
    char orderName[MAXSIZE];
    int quantity;
    cout << "Which order would you like to cancel: ";
    cin.ignore();
    cin.getline(orderName, MAXSIZE);
    clearInputBuffer();
    cout << "How many times it was ordered: ";
    cin >> quantity;

    updateOrdersFile(orderName, quantity);

    char** productsFromRecipe = allocateMemory(MAXSIZE);
    int* quantitiesFromRecipe = new int[MAXSIZE];
    int productCount = 0;

    extractRecipeIngredients(orderName, productsFromRecipe, quantitiesFromRecipe, productCount);

    if (!restoreProductsToWarehouse(productsFromRecipe, quantitiesFromRecipe, productCount, quantity)) {
        cout << "Error: Could not restore products to warehouse!" << endl;
    }
    else {
        cout << "Order successfully canceled and products restored!" << endl;
    }

    deallocateMemory(productsFromRecipe, MAXSIZE);
    delete[] quantitiesFromRecipe;

}

void seePastOrders() {
    const char* pastOrders = "Past orders.txt";
    readAndPrintFile(pastOrders);
}

void seePastOrdersSorted() {
    sortPastOrders();
    const char* sortedPastOrdersFile = "Past orders sorted.txt";
    readAndPrintFile(sortedPastOrdersFile);
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

    char** products = allocateMemory(MAXSIZE);
    int* stock = new int[MAXSIZE];
    char** unit = allocateMemory(MAXSIZE);
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
        deallocateMemory(products, MAXSIZE);
        deallocateMemory(unit, MAXSIZE);
        delete[] stock;
        delete[] line;
        return;
    }

    for (int i = removeIndex; i < productCount - 1; i++) {
        copyString(products[i], products[i + 1]);
        stock[i] = stock[i + 1];
        copyString(unit[i], unit[i + 1]);
    }
    productCount--;

    writeWarehouseToFile(warehouseFileName, products, stock, unit, productCount);

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
    
    char** productInWarehouse = allocateMemory(MAXSIZE);
    int* stockInWarehouse = new int[MAXSIZE];
    char** unitInWarehouse = allocateMemory(MAXSIZE);
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

    writeWarehouseToFile(warehouseFileName, productInWarehouse, stockInWarehouse, unitInWarehouse, productCount);

    cout << "Product \"" << product << "\" updated successfully in the warehouse!" << endl;

    deallocateMemory(productInWarehouse, MAXSIZE);
    deallocateMemory(unitInWarehouse, MAXSIZE);
    delete[] stockInWarehouse;
    delete[] line;
}

void seeDailyRevenue() {

}

void addNewItemToMenu() {
    char dish[MAXSIZE];
    char price[MAXSIZE];
    cout << "What new dish do you want to add: ";
    cin.ignore();
    cin.getline(dish, MAXSIZE);
    clearInputBuffer();
    cout << "Choose price: ";
    cin.getline(price, MAXSIZE);
    clearInputBuffer();

    const char* menuFileName = "Menu.txt";
    if (findWordInFile(dish, menuFileName)) {
        cout << "Error: This dish is already in the menu!" << endl;
        return;
    }
    saveDishToFile(dish, price);

    char recipe[MAXSIZE];
    cout << endl;
    cout << "Write recipe for the cookbook: ";
    cin.getline(recipe, MAXSIZE);
    clearInputBuffer();

    const char* recipeFileName = "Recipes.txt";
    if (findWordInFile(recipe, recipeFileName)) {
        cout << "Recipe is already in the cookbook! Glad is back on menu!" << endl;
        return;
    }

    saveRecipeToFile(recipe);

    cout << "***" << endl;
    cout << "Do not forget to order ingredients for the new dish!" << endl;
    cout << "***" << endl;

}

void removeItemFromMenu() {
    char dish[MAXSIZE];
    cout << "What item do you want to remove menu: ";
    cin.ignore();
    cin.getline(dish, MAXSIZE);
    clearInputBuffer();

    const char* menuFileName = "Menu.txt";
    if (!findWordInFile(dish, menuFileName)) {
        cout << "Dish is not in the menu! Consider adding it!" << endl;
        return;
    }

    removeLineFromFile(menuFileName, dish);
    cout << "Dish '" << dish << "' successfully removed from the menu!" << endl;
}

void printOptionsForManager() {
    while (true) {
        cout << "Select an option:" << endl;
        cout << "1) See menu" << endl;
        cout << "2) Make an order" << endl;
        cout << "3) Cancel an order" << endl;
        cout << "4) See past orders" << endl;
        cout << "5) See past orders sorted" << endl;
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
            cancelOrder();
            break;
        case 4:
            seePastOrders();
            break;
        case 5:
            seePastOrdersSorted();
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
            //seeDailyRevenue();
            break;
        case 10:
            // generateDailyRevenue();
            break;
        case 11:
            // seeAllRevenues();
            break;
        case 12:
            addNewItemToMenu();
            break;
        case 13:
            removeItemFromMenu();
            break;
        case 14:
            continue;
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
        cout << "3) Cancel an order" << endl;
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
            cancelOrder();
            break;
        case 4:
            seePastOrders();
            break;
        case 5:
            seePastOrdersSorted();
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
