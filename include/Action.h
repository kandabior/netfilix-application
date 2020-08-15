#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include <vector>

class Session;
class User;
class Watchable;

enum ActionStatus{
	PENDING, COMPLETED, ERROR
};


class BaseAction{
public:
	BaseAction();
	virtual ~BaseAction()= default;
	ActionStatus getStatus() const;
	virtual void act(Session& sess)=0;
	virtual std::string toString() const=0;
	std::string print_status() const ;
	virtual BaseAction* Clone()=0;

protected:

    static void insertUser(User *user,Session &sess);
    static bool activateUser(std::string name,Session &sess);
    static bool deleteUser(std::string name,Session &sess);
    static bool duplicateUser(const std::string& original,const std::string& newUser,Session &sess);
    static bool printContentList(Session &sess)  ;
    static bool printWatchHistory(Session &sess);
    static bool watch(const std::string& contentId,Session &sess);
    static void addToHistory(long id, Session &sess);
    bool printActionLog(Session &sess);
    static bool IsFound(const std::string& name,Session &sess);
	void complete();
	void error(const std::string& _errorMsg);
	std::string getErrorMsg() const;

private:
    std::string errorMsg;
	ActionStatus status;
};

class CreateUser  : public BaseAction {
public:

	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};

class ChangeActiveUser : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};

class DeleteUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};


class DuplicateUser : public BaseAction {
public:
	virtual void act(Session & sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};

class PrintContentList : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};

class PrintWatchHistory : public BaseAction {
public:
	virtual void act (Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};


class Watch : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};


class PrintActionsLog : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};

class Exit : public BaseAction {
public:
	virtual void act(Session& sess);
	virtual std::string toString() const;
    virtual BaseAction* Clone();
};
#endif
