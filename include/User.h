#ifndef USER_H_
#define USER_H_

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include "Tag.h"
class Watchable;
class Session;

class User{
public:
    User(std::string  name);
    virtual ~User();
    virtual User *Clone()=0;
    virtual Watchable* getRecommendation(Session& s) = 0;
    std::string getName() const;
    std::vector<Watchable*> get_history() const;
    virtual User* toDuplicate(std::string newName)=0;
    void setHistory(Watchable* w);
    void setAllHistory(std::vector<Watchable*> _history);
    void setAlltags(std::vector<Tag> tag);
    virtual void addTags(std::vector<std::string> tags){}
    void clearHistory();
protected:
    std::vector<Tag> tagVec;
    std::vector<Watchable*> history;
private:
    const std::string name;

};


class LengthRecommenderUser : public User {
public:
    LengthRecommenderUser(const std::string& name);
    virtual ~LengthRecommenderUser(){}
    virtual User *Clone();
    virtual Watchable* getRecommendation(Session& s);
    virtual User* toDuplicate(std::string newName);

private:
};

class RerunRecommenderUser : public User {
public:
    RerunRecommenderUser(const std::string& name);
    virtual ~RerunRecommenderUser(){}
    virtual User *Clone();
    virtual Watchable* getRecommendation(Session& s);
    virtual User* toDuplicate(std::string newName);
private:
    int indexOfHistory;
};

class GenreRecommenderUser : public User {
public:
    virtual void addTags(std::vector<std::string> tags);
    virtual ~GenreRecommenderUser(){}
    virtual User *Clone();
    GenreRecommenderUser(const std::string& name);
    virtual Watchable* getRecommendation(Session& s);
    virtual User* toDuplicate(std::string newName);
private:
};



#endif
