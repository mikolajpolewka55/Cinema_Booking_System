#ifndef SCREENING_H
#define SCREENING_H

#include "Reservation.h"
#include "Movie.h"
#include <string>
#include <vector>

class Screening {
private:
    Movie movie;
    std::string time;
    int hallNumber;
    int rows;
    int cols;
    std::vector<std::vector<bool>> seats2D;

public:
    Screening(const Movie& m, const std::string& t, int hall, int rows = 5, int cols = 10);

    Movie getMovie() const;
    std::string getTime() const;
    int getHallNumber() const;

    void display() const; //wyświetla dane seansu
    void displaySeats() const; //wyswietla miejsca siedzące

    bool reserveSeat(int row, int col); //czy to miejsce jest wolne?
    void applyExistingReservations(const std::vector<Reservation>& reservations);//nanieś rezerwacje na miejsca
    int getAvailableSeats() const; //liczba wolnych miejsc 

    static std::vector<Screening> loadFromFile(const std::string& path, const std::vector<Movie>& movies);
    static void displayScreenings(const std::vector<Screening>& screenings); //wyświetl seanse
    static void saveAll(const std::vector<Screening>& screenings, //zapisz seanse w pliku (admin)
                    const std::string& path);
};

#endif
