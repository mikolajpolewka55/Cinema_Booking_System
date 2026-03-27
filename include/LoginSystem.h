#ifndef LOGINSYSTEM_H
#define LOGINSYSTEM_H

#include <string>
#include <vector>

struct Account {
    std::string login;
    std::string password;
    std::string role;   
};

class LoginSystem {
private:
    std::string path;
    std::vector<Account> accounts;
    std::string currentUser;
    std::string currentRole;

public:
    LoginSystem(const std::string& filePath); //logowanie

    void loadAccounts(); //załaduj konta
    std::string login(const std::string& loginInput);
    void registerUser(const std::string& loginInput);

    std::string getCurrentUser() const { return currentUser; }
    std::string getCurrentRole() const { return currentRole; }
};

#endif
