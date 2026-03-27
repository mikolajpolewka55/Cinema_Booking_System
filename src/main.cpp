#include <iostream>
#include <locale.h>
#include <vector>
#include "../include/UserManager.h"
#include "../include/AdminManager.h"
#include "../include/LoginSystem.h"
#include "../include/Movie.h"
#include "../include/Screening.h"
#include "../include/Reservation.h"
#include <filesystem>


using namespace std;

int main() {
    setlocale(LC_ALL, "pl_PL.UTF-8");

    cout << filesystem::current_path() << endl;
    LoginSystem system("database/konta.txt"); 
    system.loadAccounts();

    string login;
    string role;


    //wczytuję dane 
    vector<Movie> movies = Movie::loadFromFile("database/filmy.txt");
    vector<Screening> screenings = Screening::loadFromFile("database/seanse.txt", movies);
    vector<Reservation> allReservations = Reservation::loadFromFile("database/rezerwacje.txt");

    //przypisuję rezerwacje do seansów
    for (Screening& s : screenings)
        s.applyExistingReservations(allReservations);

    if (movies.empty()) {
        cout << "Brak filmów do wyświetlenia." << endl;
        return 0;
    }

    //główna pętla
    while (true) {

        //logowanie
        while (login.empty()) {
            cout << "Podaj login (lub 'exit' aby wyjść): ";
            cin >> login;

            if (login == "exit") return 0;

            role = system.login(login);

            if (role.empty()) {
                cout << "Nie znaleziono użytkownika. Chcesz się zarejestrować? (t/n): ";
                char c; cin >> c;

                if (c == 't' || c == 'T') {
                    system.registerUser(login);
                    role = "user"; //nowy użytkownik zawsze jako user
                } else {
                    login.clear();
                }
            }
        }

        cout << "\n[OK] Witaj " << (role == "admin" ? "administratorze " : "użytkowniku ") << login << "!\n";

        // odpowiedni panel, w zależności od roli
        if (role == "user") {
            UserManager user(login, movies, screenings, allReservations);
            user.run(); // zwraca po wylogowaniu
        } else if (role == "admin") {
            AdminManager admin(movies, screenings, allReservations);
            admin.run(); // zwraca po wylogowaniu
        }

        // po wylogowaniu
        login.clear();
        role.clear();
        cout << "\nWylogowano. Powrót do logowania.\n";
    }

    return 0;
}
