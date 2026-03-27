#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <vector>
using namespace std;

class Movie {
private:
    string title,category,description;
    int duration;

public:
    Movie(const string& t, int d,  const string& c, const string& desc = "");

    std::string getTitle() const;
    int getDuration() const;
    std::string getCategory() const;
    std::string getDescription() const;

    void display() const; //wyswietl dane filmu

    static void displayMovieList(const std::vector<Movie>& movies); //wyświetl filmy

    static std::vector<Movie> loadFromFile(const std::string& path); //pobierz z pliku
};

#endif
