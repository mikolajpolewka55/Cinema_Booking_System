#include <vector>
#include <string>
#include "Movie.h"
#include "Screening.h"
#include "Reservation.h"
#include "Hall.h"

class AdminManager { 
private:
    std::vector<Movie>& movies;
    std::vector<Screening>& screenings;
    std::vector<Reservation>& reservations;

    void addHall(); //dodaj sale
    void removeHall(); //usun sale
    void addScreening(); //dodaj seans
    void removeScreening(); //usun seans
    void removeReservation(); //usun rezerwacje

public:
    AdminManager(std::vector<Movie>& m,
                 std::vector<Screening>& s,
                 std::vector<Reservation>& r);

    void run();
};
