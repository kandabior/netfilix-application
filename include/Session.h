#ifndef SESSION_H_
#define SESSION_H_

#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include "Action.h"

class User;
class Watchable;

class Session{
public:
    explicit Session(const std::string &configFilePath);
    Session(const Session &other);//copy constractor
    Session(Session &&sess) noexcept;// move constractor
    Session &operator=(const Session &sess); //copy assigment operator=
    Session &operator=( Session &&sess) noexcept; //move assigment operator=
    ~Session();
    void start();

    std::unordered_map<std::string,User*> getMap ();
    User* getActiveUser();
    void setActiveUser(std::string&);
    std::vector<Watchable*> getContent() ;
    std::vector<BaseAction*> getActionLog();
    void setUserInput(std::vector<std::string> _userInput){ this->userInput=std::move(_userInput);}
    std::vector<std::string> getUserInput() {return userInput;}
    void setUserInMap(User *);
    void deleteUser(std::string &name);

private:
    std::vector<std::string> userInput;
    std::vector<Watchable*> content;
    std::vector<BaseAction*> actionsLog;
    std::unordered_map<std::string,User*> userMap;
    User* activeUser;

    static std::vector<std::string> make_vector(std::string basicString);
};
#endif
