#ifndef WATCHABLE_H_
#define WATCHABLE_H_

#include <string>
#include <vector>


class Session;
class BaseAction;

class Watchable{
public:
    Watchable(long id, int length, std::vector<std::string>  tags);
    virtual ~Watchable(){}
    virtual Watchable *Clone()=0 ;
    virtual std::string toString() const = 0;
    virtual Watchable* getNextWatchable(Session&) const = 0;
    long getId() const;
    int getLength() const {return length;}
    virtual std::string getName()const =0;
    std::vector<std::string> getTags()const {return tags;}
private:
    const long id;
    int length;
    std::vector<std::string> tags;
};

class Movie : public Watchable{
public:
    Movie(long id, std::string  name, int length, const std::vector<std::string>& tags);
    virtual ~Movie(){}
    virtual Watchable *Clone();
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getName()const {return name;}

private:
    std::string name;
};


class Episode: public Watchable{
public:
    Episode(long id, std::string  seriesName,int length, int season, int episode ,const std::vector<std::string>& tags);
    virtual ~Episode(){}
    virtual Watchable *Clone();
    virtual std::string toString() const;
    virtual Watchable* getNextWatchable(Session&) const;
    std::string getName() const {return seriesName;}
    int getSeason() const {return season;}
    int getEpisode()const { return episode;}
    Watchable &operator=(Watchable *w);//TO DO

private:
    std::string seriesName;
    int season;
    int episode;
    long nextEpisodeId;
};

#endif
