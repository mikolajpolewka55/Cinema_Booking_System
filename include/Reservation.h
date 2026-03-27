#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>
#include <vector>

class Reservation {
private:
    std::string login;
    std::string movieTitle;
    std::string time;
    int hall;
    int row;
    int col;

public:
    Reservation(const std::string& l, const std::string& m,
                const std::string& t, int h, int r, int c)
        : login(l), movieTitle(m), time(t), hall(h), row(r), col(c) {}

    //gettery
    const std::string& getLogin() const;
    const std::string& getMovieTitle() const;
    const std::string& getTime() const;
    int getHall() const;
    int getRow() const;
    int getCol() const;

    static std::vector<Reservation> loadFromFile(const std::string& path); //załaduj rezerwacje
    static std::vector<Reservation> loadUserReservations(const std::string& login, const std::string& path); //załaduj rezerwacje użytkownika
    static void displayReservations(const std::vector<Reservation>& list); //wyświetl rezerwacje (wszystkie)
    static void saveAllReservations(const std::vector<Reservation>& all, const std::string& path); //zapisz rezerwacje w pliku 
};

#endif
