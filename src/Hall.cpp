#include "../include/Hall.h"
#include <fstream>

//konstruktor
Hall::Hall(int n, int r, int c) : number(n), rows(r), cols(c) {}


//gettery
int Hall::getNumber() const {
    return number;
}

int Hall::getRows() const {
    return rows;
}

int Hall::getCols() const {
    return cols;
}

std::vector<Hall> Hall::loadFromFile(const std::string& path) { //ładuje rozkład sal z pliku
    std::vector<Hall> halls;
    std::ifstream f(path);
    int n, r, c;
    while (f >> n >> r >> c)
        halls.emplace_back(n, r, c);
    return halls;
}

void Hall::saveToFile(const std::vector<Hall>& halls, const std::string& path) { //zapisuje sale w pliku
    std::ofstream f(path);
    for (auto& h : halls)
        f << h.number << " " << h.rows << " " << h.cols << "\n";
}
