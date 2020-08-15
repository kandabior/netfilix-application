//
// Created by orithaz on 11/19/19.
//
#include "../include/Action.h"
#include "../include/User.h"
#include "../include/Session.h"
#include "../include/Watchable.h"
#include <vector>





BaseAction::BaseAction(): errorMsg(),status(PENDING){}

ActionStatus BaseAction::getStatus() const { return status; }

std::string BaseAction::print_status() const {
    if(status==ERROR){
        return "ERROR"+errorMsg;
    }
    else if(status==COMPLETED){
        return "COMPLETED";
    }
    else if(status==PENDING){
        return "PENDING";
    }
    else return "null";
}

void BaseAction ::complete() {status=COMPLETED;}

void BaseAction::error(const std::string &_errorMsg) {
    status=ERROR;
    this->errorMsg=_errorMsg;
}

std::string BaseAction::getErrorMsg() const {
    return errorMsg;
}


bool BaseAction::IsFound(const std::string& name,Session &sess) {
    std::unordered_map<std::string,User*> userMap= sess.getMap() ;
    std::unordered_map<std::string,User*>::iterator it;
    it=userMap.find(name);
    return !(it == userMap.end());

}


void BaseAction::insertUser(User *user,Session &sess) {

    sess.setUserInMap(user);

}



void CreateUser::act(Session &sess) {

    std::vector<std::string>userInput=sess.getUserInput();
    User *user;
    if(!IsFound(userInput.at(1),sess)) {
        if (userInput.at((2)) == "gen") {
            user = new GenreRecommenderUser(userInput.at(1));
        } else if (userInput.at((2)) == "len") {
            user = new LengthRecommenderUser(userInput.at(1));
        } else if (userInput.at((2)) == "rer") {
            user = new RerunRecommenderUser(userInput.at(1));
        } else {
            std::cout << "input incorrect" << std::endl;
            error(": 3 letter cod is invalid");
            return;
        }
        insertUser(user, sess);
        complete();
    }
    else {
            error(": unable to creat the user- " + userInput.at(1));
            std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

    }


std::string CreateUser::toString() const {
  return "Create user "+print_status();
}

bool BaseAction::activateUser(std::string name, Session &sess) {
    if(IsFound(name,sess)) {

       sess.setActiveUser(name);
        return true;
    }
    else{
        return false;
    }

}


void ChangeActiveUser::act(Session &sess) {
    std::vector<std::string>userInput=sess.getUserInput();
    if(activateUser(userInput.at(1),sess))
        complete();
    else  {
        error(": user doesn't exist: "+userInput.at(1));
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}

std::string ChangeActiveUser::toString() const {
    return "ChangeActiveUser "+ this->print_status();

}



bool BaseAction::deleteUser(std::string name, Session &sess) {
    if (IsFound(name,sess)) {
        if(name!=sess.getActiveUser()->getName()) {
            sess.deleteUser(name);
            return true;
        }
        else{
            return false;
        }
    }
    else {
        return false;
    }

}

void DeleteUser::act(Session &sess) {
    std::vector<std::string>userInput=sess.getUserInput();
    if(deleteUser(userInput.at(1),sess))
        complete();
    else {
        error(": user can't be deleted- "+userInput.at(1));
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}

std::string DeleteUser::toString() const {
    return "DeleteUser "+ print_status();


}


bool BaseAction::duplicateUser(const std::string& original, const std::string& newName,Session &sess) {
    if(IsFound(original,sess)){
        if(!IsFound(newName,sess)){
            User *originalUser= sess.getMap().find(original)->second;
            User *newUser=originalUser->toDuplicate(newName);
            insertUser(newUser,sess);
            return true;
        }
    }
    return false;
}

void DuplicateUser::act(Session &sess) {
    std::vector<std::string>userInput=sess.getUserInput();
    if(duplicateUser(userInput.at(1),userInput.at(2),sess))
        complete();
    else {
        error(": username doesn't exist, or username is already taken");
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}
std::string DuplicateUser::toString() const {
    return "DuplicatUser "+print_status();


}

std::string printVec(const std::vector<std::string>& vec){
    std::string output="[";
    for(const std::string& s : vec){
        output+=s+",";
    }
    output.at(output.size()-1)=']';
    return output;
}

bool BaseAction::printContentList(Session &sess)  {
    try {
        std::vector<Watchable*> w  = sess.getContent();

        for (size_t i=0; i<sess.getContent().size(); i++) {
            std::cout << w[i]->getId() << ". " << w[i]->toString() << " " << w[i]->getLength() << " minutes "
                      << printVec(w[i]->getTags()) << std::endl;
        }
        return true;
    }
    catch(error_t){
        return false;
    }
}


void PrintContentList::act(Session &sess) {
    if(printContentList(sess)) {
        complete();
    }
    else {
        error(": could not print content");
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}

std::string PrintContentList::toString() const {
    return "PrintContentsList "+ print_status();


}

bool BaseAction::printWatchHistory(Session &sess) {
    try {

        std::vector<Watchable *> w = sess.getActiveUser()->get_history();
        std::cout << "watch history for " << sess.getActiveUser()->getName() << ":" << std::endl;
        for (size_t i = 0; i < w.size(); i++) {
            std::cout << i+1<< ". "<< w[i]->toString() << std::endl;

        }
        return true;
    }
    catch(error_t) {
        return false;
    }
}

void PrintWatchHistory::act(Session &sess) {
    if(printWatchHistory(sess))
        complete();
    else {
        error(": could not print history");
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}

std::string PrintWatchHistory::toString() const {
    return "PrintWatchHistory "+print_status();


}

void BaseAction::addToHistory(long id, Session &sess) {
    std::vector<Watchable*> content=sess.getContent();
    Watchable *w=content[id];
    sess.getActiveUser()->setHistory(w);
}


bool BaseAction::watch(const std::string& contentId, Session &sess) {
    size_t id = std::stol(contentId);

    try {
        if(id>0 && id<=sess.getContent().size()) {
            std::string contentName = sess.getContent().at(id - 1)->toString();
            std::cout << "watching " << contentName << std::endl;
            addToHistory(id - 1, sess);

            sess.getActiveUser()->addTags(sess.getContent().at(id - 1)->getTags());
            return true;
        }
        else{
            return false;
        }
    }
    catch (error_t){
        return false;
    }


}

void Watch::act(Session &sess) {
    std::vector<std::string>userInput=sess.getUserInput();
    if(watch(userInput.at(1),sess))
        complete();
    else {
        error(": cant play the current content");
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}

std::string Watch::toString() const {
    return "Watch "+print_status();

}

bool BaseAction::printActionLog(Session &sess) {
   try {
       for (size_t i = 0; i < sess.getActionLog().size(); i++) {
           std::string _status;
           if (sess.getActionLog()[i]->getStatus() == ERROR) {
               _status = getErrorMsg();
           }
           std::cout << (sess.getActionLog())[i]->toString() << _status << std::endl;
       }
       return true;
   }
   catch (error_t){
       return false;
   }
}

void PrintActionsLog::act(Session &sess) {
    if(printActionLog(sess))
        complete();
    else {
        error(": cant print the action log");
        std::cout <<"Error"<< getErrorMsg() << std::endl;
    }

}
std::string PrintActionsLog::toString() const {
    return "PrintActionLog "+print_status();

}

void Exit::act(Session &sess) {complete();}

std::string Exit::toString() const {
    return "Exit "+print_status();

}

BaseAction* CreateUser::Clone() {
    BaseAction *action =new CreateUser(*this);
    return action;
}

BaseAction* ChangeActiveUser::Clone() {
    BaseAction *action =new ChangeActiveUser(*this);
    return action;
}

BaseAction* DeleteUser::Clone() {
    BaseAction *action =new DeleteUser(*this);
    return action;
}

BaseAction* DuplicateUser::Clone() {
    BaseAction *action =new DuplicateUser(*this);
    return action;
}

BaseAction* PrintContentList::Clone() {
    BaseAction *action =new PrintContentList(*this);
    return action;
}

BaseAction* PrintActionsLog::Clone() {
    BaseAction *action =new PrintActionsLog(*this);
    return action;
}

BaseAction* PrintWatchHistory::Clone() {
    BaseAction *action =new PrintWatchHistory(*this);
    return action;
}

BaseAction* Watch::Clone() {
    BaseAction *action =new Watch(*this);
    return action;
}

BaseAction* Exit::Clone() {
    BaseAction *action =new Exit(*this);
    return action;
}