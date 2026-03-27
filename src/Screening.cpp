#include "../include/Screening.h"
#include "../include/Reservation.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

//konstruktor
Screening::Screening(const Movie& m, const string& t, int hall, int r, int c)
    : movie(m), time(t), hallNumber(hall), rows(r), cols(c)
{
    seats2D.resize(rows, vector<bool>(cols, false));
}

//gettery
Movie Screening::getMovie() const { return movie; }
string Screening::getTime() const { return time; }
int Screening::getHallNumber() const { return hallNumber; }

void Screening::display() const {       //wyświetl dane o filmie
    cout << "=================================\n";
    cout << "Film: " << movie.getTitle() << "\n";
    cout << "Godzina: " << time << " | Sala: " << hallNumber << "\n";
    cout << "Kategoria: " << movie.getCategory() << "\n";
    cout << "Czas trwania: " << movie.getDuration() << " min\n";
    cout << "Wolne miejsca: " << getAvailableSeats() << "/" << rows * cols << "\n";
    cout << "Opis: " << movie.getDescription() << "\n";
    cout << "=================================\n";
}

void Screening::displaySeats() const {      //wyświetl miejsca siedzące
    cout << "Miejsca: (O = wolne, X = zajęte)\n";

    cout << "    ";
    for (int c = 0; c < cols; ++c)
        cout << c + 1 << " ";
    cout << "\n";

    for (int r = 0; r < rows; ++r) {
        cout << "R" << r + 1;
        if (r + 1 < 10) cout << " "; 
        cout << " ";
        for (int c = 0; c < cols; ++c)
            cout << (seats2D[r][c] ? "X" : "O") << " ";
        cout << "\n";
    }
}

bool Screening::reserveSeat(int row, int col) {     //czy to miejsce jest wolne?
    if (row < 1 || row > rows || col < 1 || col > cols)
        return false;
    if (seats2D[row - 1][col - 1])
        return false;
    seats2D[row - 1][col - 1] = true;
    return true;
}

void Screening::applyExistingReservations(const vector<Reservation>& reservations) {    //uwzględnij aktualne rezerwacje
    for (const Reservation& r : reservations) {
        if (r.getMovieTitle() == movie.getTitle() &&
            r.getTime() == time &&
            r.getHall() == hallNumber)
        {
            if (r.getRow() >= 1 && r.getRow() <= rows && r.getCol() >= 1 && r.getCol() <= cols) {
                seats2D[r.getRow() - 1][r.getCol() - 1] = true;
            }
        }
    }
}

int Screening::getAvailableSeats() const {          //liczba wolnych miejsc
    int count = 0;
    for (const auto& r : seats2D)
        for (bool s : r)
            if (!s) count++;
    return count;
}

void Screening::displayScreenings(const vector<Screening>& screenings) {        //wyświetl seanse danego filmu
    cout << "\n======== LISTA SEANSÓW ========\n";
    for (size_t i = 0; i < screenings.size(); i++) {
        cout << i + 1 << ". " << screenings[i].getMovie().getTitle()
             << " | " << screenings[i].getTime()
             << " | Sala: " << screenings[i].getHallNumber()
             << " | Wolne miejsca: " << screenings[i].getAvailableSeats() << "\n";
    }
    cout << "===============================\n";
}

vector<Screening> Screening::loadFromFile(const string& path,               //załaduj seanse danego filmu
                                         const vector<Movie>& movies)
{
    vector<Screening> screenings;
    ifstream file(path);

    if (!file.is_open()) {
        cerr << "[X] Nie udało się otworzyć pliku: " << path << endl;
        return screenings;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string movieTitle, timeStr;
        int hall, rows, cols;

        getline(ss, movieTitle, ';');
        getline(ss, timeStr, ';');
        ss >> hall;     //wersja getline tylko dla liczb
        ss.ignore(1);   //zatrzymuje się ona przed średnikiem, więc żeby następną wartość nie wczytywać od ; omijamy go
        ss >> rows;
        ss.ignore(1);
        ss >> cols;

        auto it = find_if(      //przeszukuje wektor movies w poszukiwaniu obiektu o odpowiadającym tytule
            movies.begin(), movies.end(),
            [&](const Movie& m) { return m.getTitle() == movieTitle; }
        );

        if (it != movies.end()) {   //jeśli znalazł tworzy seans
            screenings.emplace_back(*it, timeStr, hall, rows, cols);    //i zapisuje obiekt Screening w wektorze seansów
        }
    }

    file.close();
    return screenings;
}


void Screening::saveAll(const vector<Screening>& screenings,        //zapisz seanse w pliku
                        const string& path)
{
    ofstream file(path);

    if (!file.is_open()) {
        cerr << "[X] Nie udało się zapisać seansów do pliku: " << path << endl;
        return;
    }

    for (const auto& s : screenings) {
        file << s.movie.getTitle() << ";"
                << s.time << ";"
                << s.hallNumber << ";"
                << s.rows << ";"
                << s.cols << "\n";
    }

    file.close();
}

