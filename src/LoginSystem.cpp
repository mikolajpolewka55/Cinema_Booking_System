#include "../include/LoginSystem.h"
#include <iostream>
#include <fstream>

using namespace std;

LoginSystem::LoginSystem(const std::string& filePath)
    : path(filePath), currentUser(""), currentRole("") {}

// Wczytywanie kont z pliku
void LoginSystem::loadAccounts() {
    accounts.clear();
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "[X] Nie udało się otworzyć pliku kont: " << path << endl;
        return;
    }

    string login, password, role;

    while (file >> login >> password) {
        // Sprawdź, czy po haśle znajduje się rola.
        if (file.peek() == ' ' || file.peek() == '\t') {
            file >> role;
        } else {
            role = "user";  // wszystkie stare konta bez roli → user
        }

        accounts.push_back({login, password, role});
    }
}

// Logowanie
string LoginSystem::login(const std::string& loginInput) {
    string password;
    cout << "Podaj hasło: ";
    cin >> password;

    for (auto& acc : accounts) {
        if (loginInput == acc.login && password == acc.password) {
            currentUser = acc.login;
            currentRole = acc.role;
            return acc.role;   // zwracamy user/admin
        }
    }

    return "";  // niepoprawne dane
}

// Rejestracja
void LoginSystem::registerUser(const std::string& loginInput) {
    string password;
    cout << "Ustaw hasło: ";
    cin >> password;

    accounts.push_back({loginInput, password, "user"}); //dodajemy nowe konto do bazy kont

    ofstream file(path, ios::app);
    if (!file.is_open()) {
        cerr << "[X] Błąd zapisu do pliku kont!" << endl;
        return;
    }

    file << loginInput << " " << password << " user\n";

    cout << "[OK] Zarejestrowano użytkownika: " << loginInput << endl;
}
