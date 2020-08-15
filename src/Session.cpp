//
// Created by orithaz on 11/19/19.
//
#include "../include/Session.h"
#include "../include/User.h"
#include "../include/Watchable.h"
#include <vector>
#include <iostream>
#include "../include/json.hpp"
#include "fstream"
#include <unordered_map>

using namespace nlohmann;

Session::Session(const std::string &configFilePath) :  userInput(),content(),actionsLog(), userMap() , activeUser(){
    std::ifstream content_file(configFilePath);
    json contentJ;
    content_file>>contentJ;
    int index=1;
    //MOVIES
    for(auto& obj :contentJ["movies"]){
        Watchable *movie=new Movie(index,obj["name"],obj["length"],obj["tags"]);
        content.push_back(movie);
        index++;
    }
    //Episode
    for(auto& obj :contentJ["tv_series"]) {
        int season = 1;
        for (int s : obj["seasons"]) {
            for (int i = 1; i <= s; i++) {
                Watchable *episode = new Episode(index, obj["name"], obj["episode_length"], season, i, obj["tags"]);
                content.push_back(episode);
                index++;
            }
            season++;
        }
    }

    User *user=new LengthRecommenderUser("default");
    setUserInMap(user);
    activeUser=user;

}

Session::Session(const Session &other) : userInput(),content(),actionsLog(), userMap() , activeUser() {

   for(Watchable *w:other.content){
       this->content.push_back(w->Clone());
   }
   for(std::pair<std::string,User*> p : other.userMap){
       User *user=p.second->Clone();
       if(user->getName()==other.activeUser->getName()){
           this->activeUser=user;
       }
       user->clearHistory();
       for(Watchable *w: p.second->get_history()) {
           long id = w->getId();
           user->setHistory(this->content.at(id-1));
       }
       this->userMap.insert(std::make_pair(user->getName(),user));
   }
   for(BaseAction *action : other.actionsLog){
       this->actionsLog.push_back(action->Clone());
   }
}

Session::Session(Session &&other)noexcept :  userInput(),content(),actionsLog(), userMap() , activeUser(){
    for(Watchable *i : other.content){
        this->content.push_back(i);
        i= nullptr;
    }
    other.content.clear();

    for(BaseAction  *i : other.actionsLog){
        this->actionsLog.push_back(i);
        i=nullptr;
    }
    other.actionsLog.clear();

    this->activeUser=other.activeUser;
    other.activeUser= nullptr;
    for(std::pair<std::string, User*> p:other.userMap){
        this->userMap.insert(std::make_pair(p.first,p.second));
        p.second= nullptr;
    }
    other.userMap.clear();
}

Session& Session::operator=(const Session &other) {
    if(this==&other){
        return *this;
    }
    for(Watchable *w : this->content){
        delete w;
    }
    content.clear();
    for(Watchable *w : other.content){
        this->content.push_back(w->Clone());
    }

    for(std::pair <std::string, User*> p :this->userMap){
        delete p.second;
    }
    this->userMap.clear();
    for(std::pair <std::string, User*> p :other.userMap){
        User *user=p.second->Clone();
        if(p.first==other.activeUser->getName()){
            this->activeUser=user;
        }
        user->clearHistory();
        for(Watchable *w: p.second->get_history()) {
            long id = w->getId();
            user->setHistory(this->content.at(id-1));
        }
        this->userMap.insert(std::make_pair(user->getName(),user));
    }

    for(BaseAction *action : this->actionsLog){
        delete action;
    }
    this->actionsLog.clear();
    for(BaseAction *action : other.actionsLog){
        this->actionsLog.push_back(action->Clone());
    }
    return *this;
}

Session& Session::operator=(Session &&other) noexcept{
    if(this==&other){
        return *this;
    }
    for(Watchable *w : this->content){
        delete w;
    }
    content.clear();

    for(Watchable *w : other.content){
        this->content.push_back(w);
    }
    other.content.clear();

    for(std::pair <std::string, User*> p :this->userMap){
        delete p.second;
    }
    this->userMap.clear();
    for(std::pair <std::string, User*> p :other.userMap){
        this->userMap.insert(p);
    }
    this->activeUser=other.activeUser;
    other.userMap.clear();
    other.activeUser= nullptr;

    for(BaseAction *action : this->actionsLog){
        delete action;
    }
    actionsLog.clear();
    for(BaseAction *action : other.actionsLog){
        this->actionsLog.push_back(action);
    }
    other.actionsLog.clear();
    return *this;
}



Session::~Session() {
    for(Watchable *w : content){
        delete w;
    }
    content.clear();
    for(std::pair<std::string, User*> p : userMap){
        delete p.second;
    }
    userMap.clear();

    for(BaseAction * action : actionsLog){
        delete action;
    }
    actionsLog.clear();
    userInput.clear();
}

void Session::start() {
    std::cout<<"SPLFLIX is now on!"<<std::endl;
    bool running=true;
    while (running){
        std::string str;
        getline(std::cin, str);
        setUserInput(make_vector(str));

        if(userInput[0]=="exit") {
            if(userInput.size()==1) {
                BaseAction *exit = new Exit();
                actionsLog.push_back(exit);
                exit->act(*this);
                running = false;
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="createuser"){
            if(userInput.size()==3) {
                BaseAction *creatuser = new CreateUser();
                actionsLog.push_back(creatuser);
                creatuser->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;

            }
        }
        else if(userInput[0]=="changeuser"){
            if(userInput.size()==2) {
                BaseAction *change = new ChangeActiveUser();
                actionsLog.push_back(change);
                change->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="deleteuser"){
            if(userInput.size()==2) {
                BaseAction *deleteuser = new DeleteUser();
                actionsLog.push_back(deleteuser);
                deleteuser->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="dupuser"){
            if(userInput.size()==3) {
                BaseAction *dup = new DuplicateUser();
                actionsLog.push_back(dup);
                dup->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="content"){
            if(userInput.size()==1) {
                BaseAction *Content = new PrintContentList();
                actionsLog.push_back(Content);
                Content->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="watchhist"){
            if(userInput.size()==1) {
                BaseAction *printwacthlist = new PrintWatchHistory();
                actionsLog.push_back(printwacthlist);
                printwacthlist->act(*this);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="watch"){
            if(userInput.size()==2) {
                bool addicted=true;
                while(addicted) {
                    BaseAction *watch = new Watch();
                    actionsLog.push_back(watch);
                    watch->act(*this);
                    Watchable *nextWatch = content[(std::stoi(userInput[1]) - 1)]->getNextWatchable(*this);
                    std::cout << "we recommend watching " << nextWatch->toString() << ", continue watching? [y/n]"<< std::endl;
                    userInput[1]=std::to_string(nextWatch->getId());
                    std::string continueWatch;
                    getline(std::cin, continueWatch);
                    if(continueWatch=="n" || continueWatch=="N"){
                        addicted=false;
                    }
                }
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else if(userInput[0]=="log"){
            if(userInput.size()==1) {
                BaseAction *printLog = new PrintActionsLog();
                printLog->act(*this);
                actionsLog.push_back(printLog);
            }
            else{
                std::cout<<"incorrect input"<<std::endl;
            }
        }
        else{
            std::cout<<"input incorrect"<<std::endl;
        }


    }

}

std::unordered_map<std::string,User*> Session::getMap() {
    return userMap;
}

User* Session::getActiveUser() {
    return activeUser;
}

void Session::setActiveUser(std::string & str) {
    activeUser= userMap.at(str);

}

std::vector<Watchable*> Session::getContent() {
    return content;
}

std::vector<BaseAction*> Session::getActionLog() {
    return actionsLog;
}

std::vector<std::string> Session::make_vector(std::string basicString) {
    std::vector<std::string> output;
    size_t i=0;
    std::string insert;
    while (i<basicString.length()) {
        while (i<basicString.length() && basicString.at(i) != ' ' ) {
            insert+=basicString[i];
            i++;
        }
        i++;
        output.push_back(insert);
        insert.clear();
    }
    return output;
}

void Session::setUserInMap(User *user) {
    userMap.insert(std::make_pair(user->getName(), user));
}

void Session::deleteUser(std::string &name) {
    delete userMap.at(name);
    userMap.erase(name);
}