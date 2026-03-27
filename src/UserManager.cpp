#include "../include/UserManager.h"
#include <iostream>
#include <limits>

using namespace std;

void UserManager::clearScreen() {   //funkcja pomocnicza do czyszczenia ekranu
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

//konstruktor
UserManager::UserManager(const string& userLogin,           
                         vector<Movie>& moviesRef,
                         vector<Screening>& screeningsRef,
                         vector<Reservation>& reservationsRef)
    : login(userLogin), movies(moviesRef),
      screenings(screeningsRef), allReservations(reservationsRef) {}

//głowna pętla użytkownika
void UserManager::run() {
    int choice;
    while (true) {
        cout << "\n==== MENU UŻYTKOWNIKA ====\n";
        cout << "1. Przeglądaj filmy\n";
        cout << "2. Kup bilet\n";
        cout << "3. Moje rezerwacje\n";
        cout << "4. Zmień lub wycofaj rezerwację\n";
        cout << "0. Wyloguj\n";
        cout << "Twój wybór: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UserManager::clearScreen();
            cout << "Niepoprawne dane!\n";
            continue;
        }

        switch (choice) {
            case 1: viewMovies(); break;                    //zobacz filmy
            case 2: buyTicket(); break;                     //kup bilet
            case 3: viewMyReservations(); break;            //zobacz swoje rezerwacje
            case 4: modifyOrCancelReservation(); break;     //zmień lub usuń rezerwację
            case 0: UserManager::clearScreen();
                    return;
            default: UserManager::clearScreen();
                    cout << "Niepoprawny wybór!\n";
        }
    }
}

void UserManager::viewMovies() {
    while (true) {
        UserManager::clearScreen();
        Movie::displayMovieList(movies);
        cout << "0. Powrót do menu\n";
        cout << "Wybierz film, aby zobaczyć szczegóły: ";

        int choice;
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Niepoprawne dane!\n";
            continue;
        }

        if (choice == 0) {
            UserManager::clearScreen();
            return;
        }

        if (choice < 1 || choice > movies.size()) {
            continue;
        }

        UserManager::clearScreen();
        movies[choice - 1].display();

        cout << "\n1. Wróć do listy filmów\n";
        cout << "0. Powrót do menu głównego\n";
        cout << "Twój wybór: ";

        int action;
        if (!(cin >> action)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            UserManager::clearScreen();
            cout << "Niepoprawne dane!\n";
            continue;
        }

        if (action == 0) {
        UserManager::clearScreen();
        return;
        }
    }
}

void UserManager::buyTicket() {
    UserManager::clearScreen();
    Movie::displayMovieList(movies);
    int choice;
    cout << "Wybierz film: ";

    if (!(cin >> choice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        UserManager::clearScreen();
        cout << "Niepoprawne dane!\n";
        return;
    }

    if (choice < 1 || choice > movies.size()) {
        UserManager::clearScreen();
        return;
    }

    Movie& selectedMovie = movies[choice - 1];
    vector<Screening*> movieScreenings;

    for (auto& s : screenings)
        if (s.getMovie().getTitle() == selectedMovie.getTitle())
            movieScreenings.push_back(&s);

    if (movieScreenings.empty()) {
        cout << "Brak seansów dla tego filmu.\n";
        return;
    }

    UserManager::clearScreen();
    cout << "\nDostępne seanse:\n";
    for (int i = 0; i < movieScreenings.size(); i++) {
        cout << i + 1 << ". Godzina: " << movieScreenings[i]->getTime()
             << " | Sala: " << movieScreenings[i]->getHallNumber()
             << " | Wolne miejsca: " << movieScreenings[i]->getAvailableSeats() << "\n";
    }

    int sChoice;
    cout << "Wybierz seans: ";

    if (!(cin >> sChoice)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        UserManager::clearScreen();
        cout << "Niepoprawne dane!\n";
        return;
    }

    if (sChoice < 1 || sChoice > movieScreenings.size()) {
        UserManager::clearScreen();
        return;
    }
    UserManager::clearScreen();
    Screening* sel = movieScreenings[sChoice - 1];
    sel->displaySeats();

    int r, c;
    cout << "Rząd: ";
    if (!(cin >> r)) return;
    cout << "Kolumna: ";
    if (!(cin >> c)) return;

    if (!sel->reserveSeat(r, c)) {
        UserManager::clearScreen();
        cout << "[X] To miejsce jest zajęte!\n";
        return;
    }

    allReservations.emplace_back(login, sel->getMovie().getTitle(),
                                 sel->getTime(), sel->getHallNumber(), r, c);
    Reservation::saveAllReservations(allReservations, "database/rezerwacje.txt");
    UserManager::clearScreen();
    cout << "[OK] Miejsce zarezerwowane!\n";
}

void UserManager::viewMyReservations() {
    UserManager::clearScreen();
    auto myTickets = Reservation::loadUserReservations(login, "database/rezerwacje.txt");
    Reservation::displayReservations(myTickets);
}

void UserManager::modifyOrCancelReservation() {
    auto myTickets = Reservation::loadUserReservations(login, "database/rezerwacje.txt");
    if (myTickets.empty()) {
        cout << "Brak rezerwacji do zmiany/usunięcia.\n";
        return;
    }

    UserManager::clearScreen();
    Reservation::displayReservations(myTickets);
    cout << "Wybierz bilet: ";

    int nr;
    if (!(cin >> nr)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        UserManager::clearScreen();
        cout<<"Niepoprawne dane!"<<endl; 
        return;
    }

    if (nr < 1 || nr > myTickets.size()) {
        UserManager::clearScreen();
        cout<<"Niepoprawne dane!"<<endl; 
        return;   
    }
    

    Reservation chosen = myTickets[nr - 1];

    cout << "1. Wycofaj rezerwację\n2. Zmień miejsce\nTwój wybór: ";

    int act;
    if (!(cin >> act)) {
        UserManager::clearScreen();
        return;
    }

    if (act == 1) {
        // usuwamy starą rezerwację z wektora allReservations
        vector<Reservation> updated;
        for (auto& r : allReservations) {
            if (!(r.getLogin() == chosen.getLogin() &&
                  r.getMovieTitle() == chosen.getMovieTitle() &&
                  r.getTime() == chosen.getTime() &&
                  r.getHall() == chosen.getHall() &&
                  r.getRow() == chosen.getRow() &&
                  r.getCol() == chosen.getCol())) {
                updated.push_back(r);
            }
        }
        allReservations = updated;
        Reservation::saveAllReservations(allReservations, "database/rezerwacje.txt");
        UserManager::clearScreen();
        cout << "Rezerwacja wycofana!\n";
    }

    if (act == 2) { 
    // przeładowujemy seanse z pliku 
    screenings = Screening::loadFromFile("database/seanse.txt", movies); 

    // usuwamy starą rezerwację z wektora allReservations
    vector<Reservation> updated;
    for (auto& r : allReservations) {
        if (!(login == r.getLogin() && 
              r.getMovieTitle() == chosen.getMovieTitle() &&
              r.getTime() == chosen.getTime() && 
              r.getHall() == chosen.getHall() &&
              r.getRow() == chosen.getRow() && 
              r.getCol() == chosen.getCol())) {
            updated.push_back(r); 
        }
    }
    allReservations = updated;

    //nakładamy AKTUALNE rezerwacje na świeżo wczytane (puste) seanse
    for (Screening& s : screenings) {
        s.applyExistingReservations(allReservations); 
    }

    //znajdujemy nasz konkretny seans w przeładowanym wektorze
    Screening* scr = nullptr; 
    for (auto& s : screenings) {
        if (s.getMovie().getTitle() == chosen.getMovieTitle() && 
            s.getTime() == chosen.getTime() && 
            s.getHallNumber() == chosen.getHall()) {
            scr = &s;
        }
    }

    if (!scr) { 
        cout << "Błąd — nie znaleziono seansu po przeładowaniu!\n";
        return; 
    }

    scr->displaySeats(); 

    int newRow, newCol;
    cout << "Nowy rząd: "; cin >> newRow;
    cout << "Nowa kolumna: "; cin >> newCol;

    if (!scr->reserveSeat(newRow, newCol)) {
        cout << "[X] To miejsce jest zajęte!\n";
        return;
    } 

    allReservations.emplace_back(login, scr->getMovie().getTitle(), 
                                 scr->getTime(), scr->getHallNumber(), newRow, newCol); 
    
    Reservation::saveAllReservations(allReservations, "database/rezerwacje.txt");

    cout << "[OK] Bilet zmieniony!\n"; 
}
                     
}


