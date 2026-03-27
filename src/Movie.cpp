#include "../include/Movie.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//konstruktor
Movie::Movie(const string& t, int d, const string& c, const string& desc)
    : title(t), category(c), duration(d), description(desc) {}

//gettery
string Movie::getTitle() const { return title; }
int Movie::getDuration() const { return duration; }
string Movie::getCategory() const { return category; }
string Movie::getDescription() const { return description; }

void Movie::display() const {//wyświetl dane o filmie
    cout << "\n---------------------------------\n";
    cout << "Tytuł: " << title << "\n";
    cout << "Kategoria: "<< category << "\n";
    cout << "Czas trwania: " << duration << " min\n";
    if (!description.empty())
        cout << "Opis: " << description << "\n";
    cout << "---------------------------------\n";
}

void Movie::displayMovieList(const vector<Movie>& movies) {//wyświetl listę filmów
    cout << "\n======== LISTA FILMÓW ========\n";

    for (size_t i = 0; i < movies.size(); i++) {
        cout << i + 1 << ". " << movies[i].getTitle()
             << " (" << movies[i].getDuration() << " min)\n";
    }

    cout << "===============================\n";
}

vector<Movie> Movie::loadFromFile(const string& path) {//załaduj filmy
    vector<Movie> movies;
    ifstream file(path);
    

    if (!file.is_open()) {
        cerr << "[X] Nie udalo sie otworzyć pliku: " << path << endl;
        return movies;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string title, durationStr, category, desc;

        getline(ss, title, ';');        //czyta ze strumienia ss, zapisuje jako tytuł aż napotka ;
        getline(ss, durationStr, ';');  //czas trwania
        getline(ss, category, ';');     //kategoria
        getline(ss, desc);              //opis

        if (title.empty() || durationStr.empty())   //jeśli brakuje tytułu lub czasu omija linijkę
            continue;

        int duration = stoi(durationStr);   //konwersja czasu ze stringu na integer (czas w minutach)

        movies.emplace_back(title, duration, category, desc); //tworzy obiekt Movie w wektorze filmów
    }

    file.close();
    return movies;
}
