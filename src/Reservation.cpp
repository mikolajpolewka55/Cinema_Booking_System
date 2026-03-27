#include "../include/Reservation.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<Reservation> Reservation::loadFromFile(const std::string& path) {
    std::vector<Reservation> result;
    std::ifstream file(path);

    if (!file.is_open()) return result;

    std::string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string login, title, time;
        int hall, row, col;

        getline(ss, login, ';');    //zczytuje ze strumienia ss i zapisuje jako login aż napotka ;
        getline(ss, title, ';');    //zapisuje jako tytuł
        getline(ss, time, ';');     //zapisuje jako czas
        ss >> hall >> row >> col;

        result.emplace_back(login, title, time, hall, row, col);    //tworzy obiekt Reservation w wektorze result
    }
    return result;
}

std::vector<Reservation> Reservation::loadUserReservations( //ładuje rezerwacje użytkownika
        const std::string& login,
        const std::string& path)
{
    std::vector<Reservation> all = loadFromFile(path);
    std::vector<Reservation> filtered;

    for (auto& r : all)
        if (r.getLogin() == login)
            filtered.push_back(r);

    return filtered;
}

//gettery
const std::string& Reservation::getLogin() const {
    return login;
}

const std::string& Reservation::getMovieTitle() const {
    return movieTitle;
}

const std::string& Reservation::getTime() const {
    return time;
}

int Reservation::getHall() const {
    return hall;
}

int Reservation::getRow() const {
    return row;
}

int Reservation::getCol() const {
    return col;
}


void Reservation::displayReservations(const std::vector<Reservation>& list) {       //wyświetl rezerwacje
    if (list.empty()) {
        std::cout << "\nBrak rezerwacji.\n";
        return;
    }

    std::cout << "\n===== TWOJE BILETY =====\n";
    for (int i = 0; i < list.size(); i++) {
        const auto& r = list[i];
        std::cout << i + 1 << ". Film: " << r.getMovieTitle()
                  << " | Godzina: " << r.getTime()
                  << " | Sala: " << r.getHall()
                  << " | Rząd: " << r.getRow()
                  << " | Miejsce: " << r.getCol() << "\n";
    }
    std::cout << "=========================\n";
}

void Reservation::saveAllReservations(    //zapisz wszystkie rezerwacje w pliku
        const std::vector<Reservation>& all,
        const std::string& path)
{
    std::ofstream file(path, std::ios::trunc);
    if (!file.is_open()) return;

    for (const auto& r : all) {
        file << r.getLogin() << ";"
             << r.getMovieTitle() << ";"
             << r.getTime() << ";"
             << r.getHall() << " "
             << r.getRow() << " "
             << r.getCol() << "\n";
    }
}
