#include <vector>
#include <string>

class Hall {
private:
    int number;
    int rows;
    int cols;

public:
    Hall(int n, int r, int c);

    // gettery
    int getNumber() const;
    int getRows() const;
    int getCols() const;

    static std::vector<Hall> loadFromFile(const std::string& path);
    static void saveToFile(const std::vector<Hall>& halls, const std::string& path);
};
