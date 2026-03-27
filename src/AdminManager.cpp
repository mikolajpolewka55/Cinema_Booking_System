#include "../include/AdminManager.h"
#include "../include/UserManager.h" 
#include <iostream>
#include <algorithm>

using namespace std;

//konstruktor

AdminManager::AdminManager(vector<Movie>& m,
                           vector<Screening>& s,
                           vector<Reservation>& r)
    : movies(m), screenings(s), reservations(r) {}


//Główne menu

void AdminManager::run() {
    while (true) {
        cout << "\n===== PANEL ADMINISTRATORA =====\n";
        cout << "1. Dodaj salę\n";
        cout << "2. Usuń salę\n";
        cout << "3. Dodaj seans\n";
        cout << "4. Usuń seans\n";
        cout << "5. Usuń rezerwację użytkownika\n";
        cout << "0. Powrót\n";
        cout << "Wybór: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            UserManager::clearScreen();
            cout << "Niepoprawny format danych!\n";
            continue;
        }

        if (choice == 0) {
            UserManager::clearScreen();
            break;
        }

        switch (choice) {
            case 1: UserManager::clearScreen(); addHall(); break;           //dodaj salę
            case 2: UserManager::clearScreen(); removeHall(); break;        //usuń salę
            case 3: UserManager::clearScreen(); addScreening(); break;      //dodaj seans
            case 4: UserManager::clearScreen(); removeScreening(); break;   //usuń seans
            case 5: UserManager::clearScreen(); removeReservation(); break; //usuń rezerwację
            default: 
                UserManager::clearScreen();
                cout << "Niepoprawny wybór.\n";
        }
    }
}


// Dodawanie i usuwanie sal

void AdminManager::addHall() { //dodawanie sali
    auto halls = Hall::loadFromFile("database/sale.txt");

    int number, rows, cols;
    cout << "=== DODAWANIE SALI ===\n";
    cout << "Numer sali: "; cin >> number;
    cout << "Liczba rzędów: "; cin >> rows;
    cout << "Liczba kolumn: "; cin >> cols;

    for (auto& h : halls) {
        if (h.getNumber() == number) {
            UserManager::clearScreen();
            cout << "[X] Sala o takim numerze już istnieje.\n";
            return;
        }
    }

    halls.emplace_back(number, rows, cols); //zapisywanie danych do wektora 
    Hall::saveToFile(halls, "database/sale.txt"); //i do pliku

    UserManager::clearScreen();
    cout << "[OK] Sala dodana.\n";
}


void AdminManager::removeHall() {//usuwanie sali
    int hallNumber;
    cout << "=== USUWANIE SALI ===\n";
    cout << "Podaj numer sali do usunięcia: ";
    cin >> hallNumber;

    auto halls = Hall::loadFromFile("database/sale.txt");
    size_t oldSize = halls.size();

    halls.erase(remove_if(halls.begin(), halls.end(),
        [&](const Hall& h) {
            return h.getNumber() == hallNumber;
        }), halls.end());

    if (halls.size() == oldSize) { //jeśli nie usunięto sali o podanym numerze, liczba sal się nie zmieni
        UserManager::clearScreen();
        cout << "[X] Nie znaleziono sali o numerze " << hallNumber << ".\n";
        return;
    }

    Hall::saveToFile(halls, "database/sale.txt");

    screenings.erase(remove_if(screenings.begin(), screenings.end(), //usuwanie seansów powiązanych z tą salą
        [&](const Screening& s) {
            return s.getHallNumber() == hallNumber;
        }), screenings.end());

    Screening::saveAll(screenings, "database/seanse.txt");

    reservations.erase(remove_if(reservations.begin(), reservations.end(), //usuwanie rezerwacji powiązanych z tą salą
        [&](const Reservation& r) {
            return r.getHall() == hallNumber;
        }), reservations.end());

    Reservation::saveAllReservations(reservations, "database/rezerwacje.txt");

    UserManager::clearScreen();
    cout << "[OK] Sala, seanse i rezerwacje zostały usunięte.\n";
}


// Dodawanie i usuwanie seansów

void AdminManager::addScreening() { //dodawanie seansów
    if (movies.empty()) {
        cout << "Brak filmów w bazie.\n";
        return;
    }

    cout << "=== DODAWANIE SEANSU ===\n";
    Movie::displayMovieList(movies);
    int movieChoice;
    cout << "Wybierz film: ";
    cin >> movieChoice;

    if (movieChoice < 1 || movieChoice > movies.size()) {
        UserManager::clearScreen();
        cout << "Niepoprawny wybór filmu.\n";
        return;
    }

    auto halls = Hall::loadFromFile("database/sale.txt");
    if (halls.empty()) {
        UserManager::clearScreen();
        cout << "[X] Brak dostępnych sal. Najpierw dodaj salę.\n";
        return;
    }

    cout << "\nDostępne sale:\n";
    for (auto& h : halls) {
        cout << "Sala " << h.getNumber() << " | "
             << h.getRows() << "x" << h.getCols() << "\n";
    }

    int hallNumber;
    cout << "Wybierz numer sali: ";
    cin >> hallNumber;

    Hall* selectedHall = nullptr;
    for (auto& h : halls) {
        if (h.getNumber() == hallNumber) {
            selectedHall = &h;
            break;
        }
    }

    if (!selectedHall) {
        UserManager::clearScreen();
        cout << "[X] Sala nie istnieje.\n";
        return;
    }

    string time;
    while (true) {
    cout << "Godzina seansu (HH:MM): ";
    cin >> time;

        if (time.length() == 5 && time[2] == ':') {
            // konwertujemy fragmenty stringa na int (żeby zapobiec godzinie 420:69)
            int hh = (time[0] - '0') * 10 + (time[1] - '0');
            int mm = (time[3] - '0') * 10 + (time[4] - '0');

            // i sprawdzamy czy spełniają założone normy
            if (hh >= 0 && hh <= 23 && mm >= 0 && mm <= 59) break;
        }
    
    cout << "[X] Niepoprawna godzina! Podaj zakres 00:00 - 23:59.\n";
    }

    screenings.emplace_back(
        movies[movieChoice - 1],
        time,
        selectedHall->getNumber(),
        selectedHall->getRows(),
        selectedHall->getCols()
    );

    Screening::saveAll(screenings, "database/seanse.txt");

    UserManager::clearScreen();
    cout << "[OK] Seans dodany.\n";
}


void AdminManager::removeScreening() {//usuwanie seansu
    if (screenings.empty()) {
        cout << "Brak seansów w systemie.\n";
        return;
    }

    cout << "=== USUWANIE SEANSU ===\n";
    for (int i = 0; i < screenings.size(); i++) {
        cout << i + 1 << ". "
             << screenings[i].getMovie().getTitle()
             << " | " << screenings[i].getTime()
             << " | Sala " << screenings[i].getHallNumber()
             << "\n";
    }

    int choice;
    cout << "Wybierz seans do usunięcia (0 aby wrócic): ";
    cin >> choice;

    if (choice == 0) {
        UserManager::clearScreen();
        return;
    }

    if (choice < 1 || choice > screenings.size()) {
        UserManager::clearScreen();
        cout << "Niepoprawny numer seansu.\n";
        return;
    }

    Screening removed = screenings[choice - 1];
    screenings.erase(screenings.begin() + (choice - 1));
    Screening::saveAll(screenings, "database/seanse.txt");

    reservations.erase(remove_if(reservations.begin(), reservations.end(), //usuwanie rezerwacji powiązanych z seansem
        [&](const Reservation& r) {
            return r.getMovieTitle() == removed.getMovie().getTitle()
                && r.getTime() == removed.getTime()
                && r.getHall() == removed.getHallNumber();
        }), reservations.end());

    Reservation::saveAllReservations(reservations, "database/rezerwacje.txt");

    UserManager::clearScreen();
    cout << "[OK] Seans i powiązane rezerwacje usunięte.\n";
}


// Usuwanie rezerwacji

void AdminManager::removeReservation() {
    if (reservations.empty()) {
        cout << "Brak aktywnych rezerwacji w systemie.\n";
        return;
    }

    cout << "=== MODERACJA REZERWACJI ===\n";
    Reservation::displayReservations(reservations);

    int choice;
    cout << "Wybierz rezerwację do usunięcia (0 aby wrócić): ";
    cin >> choice;

    if (choice == 0) {
        UserManager::clearScreen();
        return;
    }

    if (choice < 1 || choice > reservations.size()) {
        UserManager::clearScreen();
        cout << "Niepoprawny wybór.\n";
        return;
    }

    reservations.erase(reservations.begin() + (choice - 1));
    Reservation::saveAllReservations(reservations, "database/rezerwacje.txt");

    UserManager::clearScreen();
    cout << "[OK] Rezerwacja użytkownika została usunięta.\n";
}