/**
*
* Solution to course project #1
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2024/2025
*
* @author Elena Dimitrova
* @idnumber 0MI0600553
* @compiler VS
*
* <file with all funcions>
*
*/

#include <iostream>
#include <fstream>
#include "Constants.h"
using namespace std;

void clearConsole() {
    system("cls");
}

void clearInputBuffer() {
    // because of using both getline and cin we have to cin.ignore;
    // cin leaves the newline character in the stream which will be read as input from the getline

    cin.clear();	// clears errors flags from the cin
    cin.sync();		// discard unread characters from the input buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');	// discard characters from the input buffer
}

char** allocateMemory(int N) {
    char** matrix = new char* [N];
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
    if (str1 == nullptr || str2 == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

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
    if (destination == nullptr || source == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    int i = 0;
    while (source[i] != '\0') {
        destination[i] = source[i];
        i++;
    }
    destination[i] = '\0';
}

bool isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}

//Gets product name, quanity and unit
void processWarehouseLine(const char* line, char* product, int& quantity, char* unit) {
    if (line == nullptr || product == nullptr || unit == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    int i = 0, j = 0;
    while (line[i] != '\0') {
        if (isDigit(line[i])) {
            product[j] = '\0';
            break;
        }
        product[j++] = line[i++];
    }
    product[j - 1] = '\0';

    quantity = 0;
    while (isDigit(line[i])) {
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

//Gets product name, quantity and counts the lines 
void processPastOrderLine(char** orders, int* orderQuantities, int& orderCount, const char* List) {
    if (orders == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ifstream ordersFile(List);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file!" << endl;
        return;
    }

    char line[MAXSIZE];

    while (ordersFile.getline(line, MAXSIZE)) {
        int i = 0, j = 0;

        while (line[i] != '\0' && !isDigit(line[i])) {
            orders[orderCount][j++] = line[i++];
        }
        orders[orderCount][j - 1] = '\0';

        orderQuantities[orderCount] = 0;
        while (isDigit(line[i])) {
            orderQuantities[orderCount] = orderQuantities[orderCount] * 10 + (line[i] - '0');
            i++;
        }

        orderCount++;
    }
}

int findPriceInMenu(const char* dish) {
    if (dish == nullptr) {
        cout << "Error!" << endl;
        return -1;
    }

    ifstream menuFile(MENU_FILE);
    if (!menuFile.is_open()) {
        cout << "Error: Unable to open menu file!" << endl;
        return -1;
    }

    char line[MAXSIZE];
    char menuDish[MAXSIZE];
    int price;

    while (menuFile.getline(line, MAXSIZE)) {
        int i = 0, j = 0;

        while (!isDigit(line[i])) {
            menuDish[j++] = line[i++];
        }
        menuDish[j - 1] = '\0';

        int price = 0;
        while (isDigit(line[i])) {
            price = price * 10 + (line[i] - '0');
            i++;
        }

        if (compareStrings(menuDish, dish)) {
            menuFile.close();
            return price;
        }
    }

    menuFile.close();
    return -1;
}

bool updateOrdersFile(const char* orderName, int quantity) {
    if (orderName == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

    char** orders = allocateMemory(MAXSIZE);
    int* orderQuantities = new int[MAXSIZE];
    int orderCount = 0;

    processPastOrderLine(orders, orderQuantities, orderCount, ORDERS_FILE);

    for (int i = orderCount - 1; i >= 0; i--) {
        if (!compareStrings(orders[i], orderName)) {
            continue;
        }

        if (orderQuantities[i] > quantity) {
            orderQuantities[i] -= quantity;
            break;
        }

        if (orderQuantities[i] < quantity) {
            cout << "Error: You are trying to cancel more than ordered!" << endl;
            deallocateMemory(orders, MAXSIZE);
            delete[] orderQuantities;
            return false;
        }

        for (int j = i; j < orderCount - 1; j++) {
            copyString(orders[j], orders[j + 1]);
            orderQuantities[j] = orderQuantities[j + 1];
        }

        orderCount--;
        break;
    }

    ofstream ordersOut(ORDERS_FILE);
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

    deallocateMemory(orders, MAXSIZE);
    delete[] orderQuantities;
    return true;
}

bool writeWarehouseToFile(char** warehouseProducts, int* warehouseStock, char** warehouseUnits, int& warehouseCount) {
    if (warehouseProducts == nullptr || warehouseUnits == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

    ofstream warehouseOut(WAREHOUSE_FILE);
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

void swapPointers(char** ptr1, char** ptr2) {
    if (ptr1 == nullptr || ptr2 == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    char* temp = *ptr1;
    *ptr1 = *ptr2;
    *ptr2 = temp;
}

void swapIntegers(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int compareStringsForSorting(const char* str1, const char* str2) {
    if (str1 == nullptr || str2 == nullptr) {
        cout << "Error!" << endl;
        return -1;
    }

    while (*str1 && *str2 && *str1 == *str2) {
        str1++;
        str2++;
    }
    return *str1 - *str2;
}

void bubbleSort(char** arr, int* quantities, int size) {
    if (arr == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (compareStringsForSorting(arr[j], arr[j + 1]) > 0) {
                swapPointers(&arr[j], &arr[j + 1]);
                swapIntegers(quantities[j], quantities[j + 1]);
            }
        }
    }
}

void sortPastOrders() {
    char** orders = allocateMemory(MAXSIZE);
    int* orderQuantities = new int[MAXSIZE];
    int orderCount = 0;

    processPastOrderLine(orders, orderQuantities, orderCount, ORDERS_FILE);

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

    ofstream sortedOrdersOut(SORTED_ORDERS_FILE);
    if (sortedOrdersOut.is_open()) {
        for (int i = 0; i < sortedCount; i++) {
            sortedOrdersOut << sortedOrders[i] << " " << sortedQuantities[i] << endl;
        }
        sortedOrdersOut.close();
    }
    else {
        cout << "Error: Unable to open sorted orders file for writing!" << endl;
    }

    deallocateMemory(orders, MAXSIZE);
    delete[] orderQuantities;
    deallocateMemory(sortedOrders, MAXSIZE);
    delete[] sortedQuantities;
}

void findDate(char* date) {
    if (date == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ifstream file(REVENUE_FILE);
    if (!file.is_open()) {
        cout << "Error: Unable to open file: " << REVENUE_FILE << endl;
        return;
    }
    char line[MAXSIZE];
    while (file.getline(line, MAXSIZE)) {
        copyString(date, line);
    }

    file.close();
}

void generateNextDate(char* date, char* nextDate) {
    if (date == nullptr || nextDate == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    int day = 0, month = 0, year = 0;
    int i = 0;

    while (date[i] != '.' && date[i] != '\0') {
        day = day * 10 + (date[i] - '0');
        i++;
    }
    i++;
    while (date[i] != '.' && date[i] != '\0') {
        month = month * 10 + (date[i] - '0');
        i++;
    }
    i++;
    while (date[i] != '\0') {
        year = year * 10 + (date[i] - '0');
        i++;
    }

    day++;
    int daysInMonth;
    if (month == 2) {
        daysInMonth = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) ? 29 : 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
    }
    else {
        daysInMonth = 31;
    }
    if (day > daysInMonth) {
        day = 1;
        month++;
        if (month > 12) {
            month = 1;
            year++;
        }
    }

    int index = 0;
    if (day < 10) nextDate[index++] = '0';
    int temp = day;
    if (temp >= 10) {
        nextDate[index++] = '0' + (temp / 10);
    }
    nextDate[index++] = '0' + (temp % 10);
    nextDate[index++] = '.';
    if (month < 10) nextDate[index++] = '0';
    temp = month;
    if (temp >= 10) {
        nextDate[index++] = '0' + (temp / 10);
    }
    nextDate[index++] = '0' + (temp % 10);
    nextDate[index++] = '.';
    temp = year;
    nextDate[index++] = '0' + (temp / 1000);
    nextDate[index++] = '0' + ((temp / 100) % 10);
    nextDate[index++] = '0' + ((temp / 10) % 10);
    nextDate[index++] = '0' + (temp % 10);

    nextDate[index] = '\0';

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

bool findWordInFile(const char* word, const char* fileName) {
    if (word == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

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
            if (isDigit(line[i])) {
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
    if (orderName == nullptr || productsFromRecipe == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ifstream recipeFile(RECIPES_FILE);
    if (!recipeFile.is_open()) {
        cout << "Error: Unable to open recipe file." << endl;
        return;
    }

    char line[MAXSIZE];
    while (recipeFile.getline(line, MAXSIZE)) {
        char dishFound[MAXSIZE];
        int i = 0, j = 0;

        while (line[i] != '\0' && !isDigit(line[i])) {
            dishFound[j++] = line[i++];
        }
        dishFound[j - 1] = '\0';

        if (!compareStrings(dishFound, orderName)) {
            continue;
        }

        while (line[i] != '\0') {
            quantities[productCount] = 0;
            while (isDigit(line[i])) {
                quantities[productCount] = quantities[productCount] * 10 + (line[i] - '0');
                i++;
            }

            j = 0;
            while (line[i] != '\0' && !isDigit(line[i])) {
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
    if (products == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

    ifstream warehouseFile(WAREHOUSE_FILE);
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

    if (!writeWarehouseToFile(warehouseProducts, warehouseStock, warehouseUnits, warehouseCount)) {
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

void removeLineFromFile(const char* fileName, const char* searchDish) {
    if (searchDish == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error: Unable to open file: " << fileName << endl;
        return;
    }

    char lines[MAXSIZE][MAXSIZE];
    int lineCount = 0;
    char line[MAXSIZE];
    bool dishRemoved = false;

    while (file.getline(line, MAXSIZE)) {
        char dish[MAXSIZE];
        int i = 0, j = 0;

        while (line[i] != '\0' && !isDigit(line[i])) {
            dish[j++] = line[i++];
        }
        dish[j - 1] = '\0';

        if (compareStrings(dish, searchDish)) {
            dishRemoved = true;
            continue;
        }

        copyString(lines[lineCount], line);
        lineCount++;
    }
    file.close();

    if (!dishRemoved) {
        cout << "Error: Dish not found in the file!" << endl;
        return;
    }

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
    if (products == nullptr) {
        cout << "Error!" << endl;
        return false;
    }

    ifstream warehouseFile(WAREHOUSE_FILE);
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

    if (!writeWarehouseToFile(warehouseProducts, warehouseStock, warehouseUnits, warehouseCount)) {
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
    if (orderName == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ofstream ordersFile(ORDERS_FILE, ios::app);
    if (!ordersFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    ordersFile << orderName << " " << quantity << endl;
    ordersFile.close();

    cout << "Order done!" << endl;
}

void saveDishToFile(const char* dish, const char* price) {
    if (dish == nullptr || price == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ofstream menuFile(MENU_FILE, ios::app);
    if (!menuFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    menuFile << dish << " " << price << endl;
    menuFile.close();

    cout << "Dish is added!" << endl;
}

void saveRecipeToFile(const char* recipe) {
    if (recipe == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    ofstream recipesFile(RECIPES_FILE, ios::app);
    if (!recipesFile.is_open()) {
        cout << "Error: Unable to open orders file for writing!" << endl;
        return;
    }
    recipesFile << recipe << endl;
    recipesFile.close();

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

    bool dishIsFound = findWordInFile(orderName, MENU_FILE);
    if (!dishIsFound) {
        cout << "Error: Dish not found in the menu!" << endl;
        return;
    }

    bool recipeIsFound = findWordInFile(orderName, RECIPES_FILE);
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

void writeOffProduct() {
    char productName[MAXSIZE];
    cout << "Product to be removed: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.getline(productName, MAXSIZE);
    clearInputBuffer();

    ifstream warehouseFile(WAREHOUSE_FILE);
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

    writeWarehouseToFile(products, stock, unit, productCount);

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


    ifstream warehouseFile(WAREHOUSE_FILE);
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

    writeWarehouseToFile(productInWarehouse, stockInWarehouse, unitInWarehouse, productCount);

    cout << "Product \"" << product << "\" updated successfully in the warehouse!" << endl;

    deallocateMemory(productInWarehouse, MAXSIZE);
    deallocateMemory(unitInWarehouse, MAXSIZE);
    delete[] stockInWarehouse;
    delete[] line;
}

void seeDailyRevenue() {

    sortPastOrders();

    char** orders = allocateMemory(MAXSIZE);
    int* quantities = new int[MAXSIZE];
    int orderCount = 0;

    processPastOrderLine(orders, quantities, orderCount, SORTED_ORDERS_FILE);

    int totalRevenue = 0;
    int orderPrices[MAXSIZE];

    for (int i = 0; i < orderCount; i++) {
        int price = findPriceInMenu(orders[i]);
        if (price < 0) {
            cout << "Warning: Could not find price for " << orders[i] << ". Skipping." << endl;
            orderPrices[i] = 0;
            continue;
        }
        orderPrices[i] = price * quantities[i];
        totalRevenue += orderPrices[i];
    }

    ofstream revenueFile(SORTED_ORDERS_FILE);
    if (!revenueFile.is_open()) {
        cout << "Error: Unable to open sorted orders file for writing!" << endl;
        deallocateMemory(orders, MAXSIZE);
        delete[] quantities;
        return;
    }

    for (int i = 0; i < orderCount; i++) {
        revenueFile << orders[i] << " " << quantities[i] << " - " << orderPrices[i] << " leva" << endl;
    }

    revenueFile << "Total: " << totalRevenue << " leva" << endl;
    revenueFile.close();

    readAndPrintFile(SORTED_ORDERS_FILE);

    deallocateMemory(orders, MAXSIZE);
    delete[] quantities;
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

    if (findWordInFile(dish, MENU_FILE)) {
        cout << "Error: This dish is already in the menu!" << endl;
        return;
    }
    saveDishToFile(dish, price);

    char recipe[MAXSIZE];
    cout << endl;
    cout << "Write recipe for the cookbook: ";
    cin.getline(recipe, MAXSIZE);
    clearInputBuffer();

    if (findWordInFile(recipe, RECIPES_FILE)) {
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

    if (!findWordInFile(dish, MENU_FILE)) {
        cout << "Dish is not in the menu! Consider adding it!" << endl;
        return;
    }

    removeLineFromFile(MENU_FILE, dish);
    cout << "Dish '" << dish << "' successfully removed from the menu!" << endl;
}

void generateDailyRevenue(char* date) {
    if (date == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    cout << date << endl;
    seeDailyRevenue();

    ofstream revenueOut(REVENUE_FILE, ios::app);
    if (!revenueOut.is_open()) {
        cout << "Error: Unable to open revenue file for writing!" << endl;
        return;
    }

    ifstream sortedOrders(SORTED_ORDERS_FILE);
    if (!sortedOrders.is_open()) {
        cout << "Error: Unable to open sorted orders file!" << endl;
        revenueOut.close();
        return;
    }

    char line[MAXSIZE];
    while (sortedOrders.getline(line, MAXSIZE)) {
        revenueOut << line << endl;
    }
    sortedOrders.close();
    revenueOut << SEPARATOR << endl;

    char nextDate[MAXSIZE];
    generateNextDate(date, nextDate);
    revenueOut << nextDate << endl;
    revenueOut.close();

    ofstream clearFile(ORDERS_FILE, ios::trunc);
    if (!clearFile.is_open()) {
        cout << "Error: Unable to clear past orders file!" << endl;
        return;
    }
    clearFile.close();
}

void seeAllRevenues() {
    char dateToFind[MAXSIZE];
    cout << "From what date do you want to check: ";
    cin.ignore();
    cin.getline(dateToFind, MAXSIZE);
    clearInputBuffer();

    ifstream revenueFile(REVENUE_FILE);
    if (!revenueFile.is_open()) {
        cout << "Error: Unable to open revenues file!" << endl;
        return;
    }

    char line[MAXSIZE];
    bool print = false;
    cout << "\n=== Revenues from " << dateToFind << " onwards ===\n";

    while (revenueFile.getline(line, MAXSIZE)) {
        if (!print) {
            if (compareStrings(line, dateToFind)) {
                print = true;
            }
        }
        if (print) {
            cout << line << endl;
        }
    }
    cout << endl;
    revenueFile.close();
    if (!print) {
        cout << "Error: Date not found in revenues file!" << endl;
    }
}

void printOptionsForManager(char* date) {
    if (date == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    while (true) {
        cout << MANAGER_MENU_OPTIONS;

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
            readAndPrintFile(MENU_FILE);
            break;
        case 2:
            makeOrder();
            break;
        case 3:
            cancelOrder();
            break;
        case 4:
            readAndPrintFile(ORDERS_FILE);
            break;
        case 5:
            sortPastOrders();
            readAndPrintFile(SORTED_ORDERS_FILE);
            break;
        case 6:
            readAndPrintFile(WAREHOUSE_FILE);
            break;
        case 7:
            writeOffProduct();
            break;
        case 8:
            stockProduct();
            break;
        case 9:
            seeDailyRevenue();
            break;
        case 10:
            generateDailyRevenue(date);
            return;
        case 11:
            seeAllRevenues();
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
        cout << WAITER_MENU_OPTIONS;

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
            readAndPrintFile(MENU_FILE);
            break;
        case 2:
            makeOrder();
            break;
        case 3:
            cancelOrder();
            break;
        case 4:
            readAndPrintFile(ORDERS_FILE);
            break;
        case 5:
            sortPastOrders();
            readAndPrintFile(SORTED_ORDERS_FILE);
            break;
        case 6:
            seeDailyRevenue();
            break;
        case 7:
            continue;
        case 8:
            return;
        }
    }
}

void startingMessages(char& role, char* date) {
    if (date == nullptr) {
        cout << "Error!" << endl;
        return;
    }

    findDate(date);
    cout << "Welcome!" << endl;
    cout << "Today is " << date << endl;
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

void startSoftware() {
    char role;
    char date[MAXSIZE];
    startingMessages(role, date);
    if (role == 'A' || role == 'a') {
        printOptionsForManager(date);
    }
    else if (role == 'B' || role == 'b') {
        printOptionsForWaiter();
    }
}

int main() {
    startSoftware();

    return 0;
}