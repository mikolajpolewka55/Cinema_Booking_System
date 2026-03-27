#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "Movie.h"
#include "Screening.h"
#include "Reservation.h"
#include <vector>
#include <string>

class UserManager {
private:
    std::string login;
    std::vector<Movie>& movies;
    std::vector<Screening>& screenings;
    std::vector<Reservation>& allReservations;

    void viewMovies(); //zobacz filmy
    void buyTicket(); //kup bilet
    void viewMyReservations(); //zobacz swoje rezerwacje
    void modifyOrCancelReservation(); //zmień lub usuń rezerwację

public:
    static void clearScreen();
    UserManager(const std::string& userLogin,
                std::vector<Movie>& moviesRef,
                std::vector<Screening>& screeningsRef,
                std::vector<Reservation>& reservationsRef);

    void run(); // główna pętla użytkownika
};
#endif
