//
// Created by orithaz on 11/19/19.
//
#include "../include/User.h"
#include "../include/Watchable.h"
#include "../include/Session.h"
#include "../include/Tag.h"
#include <algorithm>
#include <cmath>
#include <utility>




User::User(std::string name) :tagVec(),history(),name(std::move(name)){}

std::string User::getName() const { return name;}

std::vector<Watchable*> User::get_history() const {
    return history;
}

void User::setHistory(Watchable *w) {
    history.push_back(w);
}

void User::setAllHistory(std::vector<Watchable *>_history) {
    this->history=std::move(_history);
}

void User::setAlltags(std::vector<Tag> tag) {
    this->tagVec=std::move(tag);
}

User::~User() = default;

LengthRecommenderUser::LengthRecommenderUser(const std::string &name) :User(name){}

Watchable* LengthRecommenderUser::getRecommendation(Session &s) {
    double sum=0,index=0;
    for(Watchable *w:history){
        sum=sum+w->getLength();
        index++;
    }
    double ave=sum/index;
    Watchable *curr = nullptr;
    double min=std::numeric_limits<int>::max();
    for(Watchable *w:s.getContent()){
        if(std::find(history.begin(),history.end(),w)==history.end()){
            if(std::abs(w->getLength()-ave) < min){
                min=std::abs(w->getLength()-ave);
                curr=w;
            }
        }
    }
    return curr;
}

User* LengthRecommenderUser::toDuplicate(std::string newName) {
    std::vector<Watchable *> historyNew;
    for(Watchable* w : history){
        historyNew.push_back(w);
    }
    std::vector <Tag> vecTagNew;
    for(Tag &t: tagVec){
        vecTagNew.push_back(t);
    }
    User *newUser= new LengthRecommenderUser(newName);
    newUser->setAllHistory(historyNew);
    newUser->setAlltags(vecTagNew);
    return newUser;
}

RerunRecommenderUser::RerunRecommenderUser(const std::string &name) :User(name), indexOfHistory(0) {}

Watchable* RerunRecommenderUser::getRecommendation(Session &s) {
    if(history.empty()){
        return nullptr;
    }
    Watchable* Rer= history.at((indexOfHistory+1)%history.size());
    indexOfHistory++;
    return Rer;

}

User* RerunRecommenderUser::toDuplicate(std::string newName) {
    std::vector<Watchable *> historyNew;
    for(Watchable* w : history){
        historyNew.push_back(w);
    }
    std::vector <Tag> vecTagNew;
    for(Tag &t: tagVec){
        vecTagNew.push_back(t);
    }
    User *newUser= new RerunRecommenderUser(newName);
    newUser->setAllHistory(historyNew);
    newUser->setAlltags(vecTagNew);
    return newUser;
}

GenreRecommenderUser::GenreRecommenderUser(const std::string &name) :User(name) {}

void GenreRecommenderUser::addTags(std::vector<std::string> tags) {
    for(const std::string& t : tags){
        bool enter= true;
        for(Tag &tag :tagVec) {
            if (tag.getName() == t) {
                tag.adder();
                enter = false;
            }

        }
        if(enter) {
            tagVec.emplace_back(t);
        }
    }
}


Watchable* GenreRecommenderUser::getRecommendation(Session &sess) {
    std::sort(tagVec.begin(), tagVec.end());
    for(Tag t: tagVec) {
        std::string maxTag=t.getName();
        for (Watchable *w: sess.getContent()) {
            if (std::find(history.begin(), history.end(), w) == history.end()) {
                for (const std::string& s: w->getTags()) {
                    if (s == maxTag)
                        return w;
                }
            }

        }
    }
    return nullptr;
}

User* GenreRecommenderUser::toDuplicate(std::string newName) {
    std::vector<Watchable *> historyNew;
    for(Watchable* w : history){
        historyNew.push_back(w);
    }
    std::vector <Tag> vecTagNew;
    for(Tag t: tagVec){

        vecTagNew.emplace_back(t.getName(),t.getCount());
    }
    User *newUser= new GenreRecommenderUser(newName);
    newUser->setAllHistory(historyNew);
    newUser->setAlltags(vecTagNew);
    return newUser;
}


User* LengthRecommenderUser::Clone() {
    User* user= new LengthRecommenderUser(*this);
    return user;
}

User* GenreRecommenderUser::Clone() {
    User* user= new GenreRecommenderUser(*this);
    return user;
}

User* RerunRecommenderUser::Clone() {
    User* user= new RerunRecommenderUser(*this);
    return user;
}

void User::clearHistory() {
    history.clear();
}