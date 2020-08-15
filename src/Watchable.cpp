//
// Created by kandabior on 11/19/19.
//
#include "../include/Watchable.h"

#include <utility>
#include "../include/Session.h"
#include "../include/User.h"



Watchable::Watchable(long id, int length, std::vector<std::string> tags)
            : id(id), length(length), tags(std::move(tags)){

}




long Watchable::getId() const {
    return id;
}

Movie::Movie(long id, std::string name, int length, const std::vector<std::string> &tags)
: Watchable(id,length,tags), name(std::move(name)){

}

std::string Movie::toString() const {
    std::string output;
    output=getName();
    return output;
}
Watchable* Movie::Clone() {
    Watchable *movie=new Movie(*this);
    return movie;
}

Watchable * Movie::getNextWatchable(Session &sess) const {
    return sess.getActiveUser()->getRecommendation(sess);
}

Episode::Episode(long id, std::string seriesName, int length, int season, int episode,
                 const std::vector<std::string> &tags): Watchable(id,length,tags), seriesName(std::move(seriesName)) ,season(season),
                 episode(episode), nextEpisodeId(id){}

std::string Episode::toString() const {
    std::string output;
    std::string _season=std::to_string(getSeason());
    std::string _episode=std::to_string(getEpisode());
    output= getName() + " S" + _season + "E" + _episode;
    return output;
}

Watchable* Episode::getNextWatchable(Session & sess) const {
    size_t idTmp=nextEpisodeId;
    if (idTmp<sess.getContent().size() && (sess.getContent())[nextEpisodeId]->getName() == this->getName()) {
        return (sess.getContent())[nextEpisodeId];
    }
    else {
       return sess.getActiveUser()->getRecommendation(sess);

    }
}

Watchable* Episode::Clone() {
    Watchable *_episode=new Episode(*this);
    return _episode;
}



