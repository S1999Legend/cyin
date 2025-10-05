#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>

using namespace std;

ofstream adventurerProfiles("adventurerProfiles.txt", ios::app), coinLog("coinLog.txt", ios::app);

class Adventurer {
private:
    int adventurerId;
    string name, password, adventurerClass;
    double goldCoins;

public:
    Adventurer(const string& uname, const string& pwd, const string& cls, double coins)
        : name(uname), password(pwd), adventurerClass(cls), goldCoins(coins) {

        srand(static_cast<unsigned>(time(0)));
        adventurerId = rand() % 9000 + 1000;
    }

    int getId() const { return adventurerId; }
    string getName() const { return name; }
    string getPassword() const { return password; }
    string getClass() const { return adventurerClass; }
    double getCoins() const { return goldCoins; }

    void earn(double coins) {
        goldCoins += coins;
        system("cls");
        cout << "You earned " << coins << " Gold Coins. Total: " << goldCoins << endl;
    }

    void spend(double coins) {
        if (coins > goldCoins) {
            cout << "Not enough coins. Spend failed.\n";
        }
        else {
            goldCoins -= coins;
            cout << "You spent " << coins << " Gold Coins. Remaining: " << goldCoins << endl;
        }
    }

    void setPassword(const string& newPassword) {
        password = newPassword;
    }

    void setName(const string& newName) {
        name = newName;
    }

    void setClass(const string& newClass) {
        adventurerClass = newClass;
    }

    void setCoins(double newCoins) {
        goldCoins = newCoins;
    }

    void displayMessage(const string& message, int delay = 100) {
        for (char character : message) {
            cout << character;
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
    }
};

class AdventureGame {
private:
    vector<Adventurer> adventurers;
    Adventurer* current = NULL;

    Adventurer* findAdventurerById(int id) {
        for (auto& a : adventurers) {
            if (a.getId() == id) {
                return &a;
            }
        }
        return nullptr;
    }

public:
    void displayMessage(const string& msg, int delay = 30) {
        for (char c : msg) {
            cout << c;
            this_thread::sleep_for(chrono::milliseconds(delay));
        }
    }

    void createProfile() {
        string name, password, cls;
        double coins;

        cout << "Choose a name: ";
        cin >> name;

        cout << "Set your password: ";
        cin >> password;

        while (true) {
            cout << "Choose your class (Warrior/Mage): ";
            cin >> cls;

            if ((cls == "Warrior") || (cls == "warrior") || (cls == "Mage") || (cls == "mage")) {
                break;
            } else {
                displayMessage("Invalid class. Try again.\n");
            }
        }

        cout << "Start with how many Gold Coins? ";
        cin >> coins;

        adventurers.emplace_back(name, password, cls, coins);

        system("cls");
        displayMessage("Profile created! Your adventurer ID is: ");
        cout << adventurers.back().getId() << endl;

        if (adventurerProfiles.is_open()) {
            adventurerProfiles << "[ NEW PROFILE ] Name: " << name << ", Class: " << cls
                << ", Coins: " << coins << ", ID: " << adventurers.back().getId() << endl;
        }
    }

    void login() {
        string input, password;
        cout << "Enter your ID or name: ";
        cin >> input;

        cout << "Enter password: ";
        cin >> password;

        bool isNumber = all_of(input.begin(), input.end(), ::isdigit);
        for (auto& a : adventurers) {
            if ((isNumber && a.getId() == stoi(input)) || (!isNumber && a.getName() == input)) {
                if (a.getPassword() == password) {
                    current = &a;
                    system("cls");
                    displayMessage("Welcome back, ");
                    cout << current->getName() << "!\n";

                    if (coinLog.is_open()) {
                        coinLog << "[ LOGIN ] " << current->getName() << " (ID: " << current->getId() << ") logged in.\n";
                    }
                    return;
                }
            }
        }

        displayMessage("Login failed. Wrong credentials.\n");
    }

    void earnCoins() {
        double coins;
        cout << "How many coins did you earn? ";
        cin >> coins;
        current->earn(coins);

        if (coinLog.is_open()) {
            coinLog << "[ COINS EARNED ] " << current->getName() << " earned " << coins << " coins.\n";
        }
    }

    void spendCoins() {
        double coins;
        cout << "How many coins do you want to spend? ";
        cin >> coins;

        if (coins > current->getCoins()) {
            displayMessage("You don’t have that much.\n");
        } else {
            current->spend(coins);
            if (coinLog.is_open()) {
                coinLog << "[ COINS SPENT ] " << current->getName() << " spent " << coins << " coins.\n";
            }
        }
    }

    void viewProfile() {
        cout << "Adventurer ID: " << current->getId() << endl;
        cout << "Name: " << current->getName() << endl;
        cout << "Class: " << current->getClass() << endl;
        cout << "Gold Coins: " << current->getCoins() << endl;
    }

    void changeProfile() {
        int option;
        string input;

        cout << "\nChange Profile:\n1. Name\n2. Password\n3. Class\nChoose: ";
        cin >> option;

        if (option == 1) {
            cout << "New name: ";
            cin >> input;
            current->setName(input);
        } else if (option == 2) {
            cout << "New password: ";
            cin >> input;
            current->setPassword(input);
        } else if (option == 3) {
            cout << "New class: ";
            cin >> input;
            current->setClass(input);
        } else {
            cout << "Invalid option.\n";
        }

        system("cls");
        cout << "Profile updated.\n";
    }

    void sendCoins() {
        int targetId;
        double coins;

        cout << "Enter adventurer ID to send coins to: ";
        cin >> targetId;

        Adventurer* target = findAdventurerById(targetId);

        if (target == nullptr) {
            cout << "That adventurer doesn’t exist.\n";
            return;
        }

        cout << "Coins to send: ";
        cin >> coins;

        if (coins > current->getCoins()) {
            cout << "Not enough coins.\n";
            return;
        }

        current->spend(coins);
        target->earn(coins);

        if (coinLog.is_open()) {
            coinLog << "[ TRANSFER ] " << current->getName() << " (ID " << current->getId()
                << ") sent " << coins << " coins to " << target->getName() << " (ID " << targetId << ")\n";
        }

        system("cls");
        cout << "Coins sent successfully.\n";
    }

    void logout() {
        displayMessage("Goodbye, ");
        cout << current->getName() << "!\n";

        if (coinLog.is_open()) {
            coinLog << "[ LOGOUT ] " << current->getName() << " logged out.\n";
        }

        current = nullptr;
    }

    void menu() {
        char choice;
        do {
            displayMessage("\n1. Earn Coins\n2. Spend Coins\n3. Send Coins\n4. Profile\n5. Logout\n");
            cout << "Choose: ";
            cin >> choice;

            switch (choice) {
            case '1':
                earnCoins();
                break;
            case '2':
                spendCoins();
                break;
            case '3':
                sendCoins();
                break;
            case '4':
                viewProfile();
                cout << "\nDo you want to update your profile? (1 = Yes / 0 = No): ";
                int ch;
                cin >> ch;
                if (ch == 1) changeProfile();
                break;
            case '5':
                logout();
                return;
            default:
                cout << "Invalid choice.\n";
            }

        } while (true);
    }

    Adventurer* getCurrent() { return current; }
};

int main() {
    AdventureGame game;
    char choice;

    cout << "\n\t=============================\n";
    cout << "\t Welcome to Coin Quest RPG\n";
    cout << "\t   Collect. Spend. Rule.\n";
    cout << "\t=============================\n";

    do {
        cout << "\n1. Login\n2. Create Profile\n3. Exit\nChoose: ";
        cin >> choice;

        switch (choice) {
        case '1':
            game.login();
            if (game.getCurrent() != nullptr) {
                game.menu();
            }
            break;
        case '2':
            game.createProfile();
            break;
        case '3':
            cout << "See ya next time!\n";
            break;
        default:
            cout << "Invalid option.\n";
        }

    } while (choice != '3');

    return 0;
}
