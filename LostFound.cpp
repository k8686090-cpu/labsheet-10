#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cctype>
#include <limits>
using namespace std;

const int MAX = 100;

struct Item {
    int id;
    string type, name, description;
    string location, date, contact, status;
};

Item items[MAX];
int countItems = 0;
int nextId = 1;

// Convert text into lowercase
string lowerCase(string s) {
    for (int i = 0; i < s.length(); i++)
        s[i] = tolower((unsigned char)s[i]);
    return s;
}

// Input helper
string getText(string message) {
    string s;
    cout << message;
    getline(cin >> ws, s);
    return s;
}

// Save records in file
void saveRecords() {
    ofstream file("records.txt");

    for (int i = 0; i < countItems; i++) {
        file << items[i].id << "|"
             << items[i].type << "|"
             << items[i].name << "|"
             << items[i].description << "|"
             << items[i].location << "|"
             << items[i].date << "|"
             << items[i].contact << "|"
             << items[i].status << "\n";
    }
    file.close();
}

// Load records from file
void loadRecords() {
    ifstream file("records.txt");
    string line;

    while (getline(file, line) && countItems < MAX) {
        stringstream ss(line);
        string temp;
        Item x;

        if (!getline(ss, temp, '|')) continue;
        x.id = stoi(temp);

        if (!getline(ss, x.type, '|')) continue;
        if (!getline(ss, x.name, '|')) continue;
        if (!getline(ss, x.description, '|')) continue;
        if (!getline(ss, x.location, '|')) continue;
        if (!getline(ss, x.date, '|')) continue;
        if (!getline(ss, x.contact, '|')) continue;
        if (!getline(ss, x.status)) continue;

        items[countItems++] = x;

        if (x.id >= nextId)
            nextId = x.id + 1;
    }
    file.close();
}

// Student Login
void studentLogin() {
    string name, roll;

    cout << "\n--- STUDENT LOGIN ---\n";
    name = getText("Enter Student Name: ");
    roll = getText("Enter Roll Number: ");

    cout << "\nWelcome, " << name << "!\n";
    cout << "Roll Number: " << roll << "\n";
}

// Report Lost or Found Item
void addItem(string type) {
    if (countItems >= MAX) {
        cout << "\nRecord limit reached!\n";
        return;
    }

    Item x;
    x.id = nextId++;
    x.type = type;

    cout << "\n--- REPORT " << type << " ITEM ---\n";

    x.name = getText("Enter Item Name: ");
    x.description = getText("Enter Description: ");
    x.location = getText("Enter Location: ");
    x.date = getText("Enter Date: ");
    x.contact = getText("Enter Contact: ");
    x.status = "Open";

    items[countItems++] = x;
    saveRecords();

    cout << "\nItem reported successfully!";
    cout << "\nYour Item ID is: " << x.id << "\n";
}

// Search Item
void searchItem() {
    string key = lowerCase(getText("\nEnter item name to search: "));
    bool found = false;

    for (int i = 0; i < countItems; i++) {
        if (lowerCase(items[i].name).find(key) != string::npos) {
            cout << "\nID: " << items[i].id
                 << "\nType: " << items[i].type
                 << "\nItem: " << items[i].name
                 << "\nLocation: " << items[i].location
                 << "\nDate: " << items[i].date
                 << "\nContact: " << items[i].contact
                 << "\nStatus: " << items[i].status << "\n";

            found = true;
        }
    }

    if (!found)
        cout << "\nNo matching item found!\n";
}

// Automatically Match Lost and Found Items
void autoMatch() {
    bool found = false;

    cout << "\n--- MATCHED ITEMS ---\n";

    for (int i = 0; i < countItems; i++) {
        if (items[i].type == "Lost" && items[i].status == "Open") {
            for (int j = 0; j < countItems; j++) {
                if (items[j].type == "Found" &&
                    items[j].status == "Open" &&
                    lowerCase(items[i].name) == lowerCase(items[j].name)) {

                    cout << "\nLost Item ID: " << items[i].id;
                    cout << "\nFound Item ID: " << items[j].id;
                    cout << "\nMatched Item: " << items[i].name << "\n";

                    items[i].status = "Matched";
                    items[j].status = "Matched";
                    found = true;
                    break;
                }
            }
        }
    }

    if (!found)
        cout << "No matching records found.\n";

    saveRecords();
}

// View All Records
void viewRecords() {
    if (countItems == 0) {
        cout << "\nNo records available!\n";
        return;
    }

    cout << "\n--- ALL ITEM RECORDS ---\n";

    for (int i = 0; i < countItems; i++) {
        cout << "\nID: " << items[i].id
             << "\nType: " << items[i].type
             << "\nName: " << items[i].name
             << "\nDescription: " << items[i].description
             << "\nLocation: " << items[i].location
             << "\nDate: " << items[i].date
             << "\nContact: " << items[i].contact
             << "\nStatus: " << items[i].status
             << "\n----------------------\n";
    }
}

// Delete Record
void deleteRecord() {
    int id;
    cout << "\nEnter Item ID to delete: ";
    cin >> id;

    for (int i = 0; i < countItems; i++) {
        if (items[i].id == id) {
            for (int j = i; j < countItems - 1; j++)
                items[j] = items[j + 1];

            countItems--;
            saveRecords();

            cout << "\nRecord deleted successfully!\n";
            return;
        }
    }

    cout << "\nRecord not found!\n";
}

// Item Statistics
void statistics() {
    int lost = 0, found = 0, matched = 0;

    for (int i = 0; i < countItems; i++) {
        if (items[i].type == "Lost")
            lost++;
        else
            found++;

        if (items[i].status == "Matched")
            matched++;
    }

    cout << "\n--- ITEM STATISTICS ---\n";
    cout << "Total Records: " << countItems << "\n";
    cout << "Lost Items: " << lost << "\n";
    cout << "Found Items: " << found << "\n";
    cout << "Matched Records: " << matched << "\n";
}

// Sort Records by Item Name
void sortRecords() {
    for (int i = 0; i < countItems - 1; i++) {
        for (int j = 0; j < countItems - i - 1; j++) {
            if (lowerCase(items[j].name) >
                lowerCase(items[j + 1].name)) {

                Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    saveRecords();
    cout << "\nRecords sorted successfully!\n";
}

// Export Report
void exportReport() {
    ofstream file("report.txt");

    file << "--- SMART CAMPUS LOST & FOUND REPORT ---\n";

    for (int i = 0; i < countItems; i++) {
        file << "\nID: " << items[i].id
             << "\nType: " << items[i].type
             << "\nName: " << items[i].name
             << "\nLocation: " << items[i].location
             << "\nDate: " << items[i].date
             << "\nStatus: " << items[i].status
             << "\n----------------------\n";
    }

    file.close();
    cout << "\nReport exported to report.txt successfully!\n";
}

// Main Function
int main() {
    loadRecords();

    int choice;

    cout << "====================================\n";
    cout << " SMART CAMPUS LOST & FOUND SYSTEM\n";
    cout << "====================================\n";

    studentLogin();

    do {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Report Lost Item\n";
        cout << "2. Report Found Item\n";
        cout << "3. Search Item\n";
        cout << "4. Auto Match Lost & Found\n";
        cout << "5. View All Records\n";
        cout << "6. Delete Record\n";
        cout << "7. Item Statistics\n";
        cout << "8. Sort Records\n";
        cout << "9. Export Report\n";
        cout << "10. Exit\n";

        cout << "\nEnter Your Choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                addItem("Lost");
                break;

            case 2:
                addItem("Found");
                break;

            case 3:
                searchItem();
                break;

            case 4:
                autoMatch();
                break;

            case 5:
                viewRecords();
                break;

            case 6:
                deleteRecord();
                break;

            case 7:
                statistics();
                break;

            case 8:
                sortRecords();
                break;

            case 9:
                exportReport();
                break;

            case 10:
                cout << "\nThank you for using the system!\n";
                break;

            default:
                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 10);

    return 0;
}
