#include "matchKeeper.h"
#include <vp/utils.h>
#include <vp/exception.h>
#include <utility>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include "roomKeeper.h"

bg_namespace_BEGIN

matchKeeper::matchKeeper
(const std::string & name, roomKeeper & room,
        const std::string & game, const std::string & params):
room_(room), keeper(name),
module_(room.server().moduleLoader().info(game).createModule(*this)) {
}

//void matchKeeper::addClient(client & which);
/**
 * @brief removes client from the match
 * @param which client to be removed
 */
//void removeClient(client & which);
/*const std::string & matchKeeper::name() const{
 return keepername();
 }*/

void matchKeeper::join(const std::string & who, bool active) {
    roomKeeper::client_container::const_iterator itc = room_.clients().find(who);
    boost::shared_ptr<matchClient> mcl(new matchClient(*itc->second, *this, active));
    if(active) {
        players_.push_back(mcl);
    } else {
        spectators_.push_back(mcl);
    }

    for(matchKeeper::player_container::iterator it = players_.begin(); it != players_.end(); ++it) {
        static_cast<matchClient &>(**it).client().joined_match(*this, who, active);
    }

    for(matchKeeper::player_container::iterator it = spectators_.begin(); it != spectators_.end(); ++it) {
        static_cast<matchClient &>(**it).client().joined_match(*this, who, active);
    }

}

void matchKeeper::leave(const std::string & who) {
    boost::shared_ptr<matchClient> cl;
    bool found = false;
    size_t i;
    for(i = 0; i < players_.size(); ++i) {
        cl = boost::static_pointer_cast<matchClient>(players_[i]);
        if(cl->client().name() == who) {
            found = true;
            break;
        }
    }

    if(!found) {
        for(i = 0; i < spectators_.size(); ++i) {
            cl = boost::static_pointer_cast<matchClient>(players_[i]);
            if(cl->client().name() == who) {
                break;
            }
        }
    }

    for(matchKeeper::player_container::iterator it = players_.begin(); it != players_.end(); ++it) {
        (boost::static_pointer_cast<matchClient>(*it))->client().left_match(*this, who);
    }

    for(matchKeeper::player_container::iterator it = spectators_.begin(); it != spectators_.end(); ++it) {
        (boost::static_pointer_cast<matchClient>(*it))->client().left_match(*this, who);
    }

    if(found) {
        players_.erase(players_.begin() + i);
    } else {
        spectators_.erase(spectators_.begin() + i);
    }

    if(!spectators_.size() && !players_.size()) {
        room_.removeMatch(this->name());
    }

}

int matchKeeper::parameterVersion() const {
    return parameterVersion_;
}

const matchKeeper::parameter_container & matchKeeper::parameters() const {
    return parameters_;
}

bool matchKeeper::isFirstPlayer(const std::string & who) const {
    return players_[0].name() == who;
}

void matchKeeper::setParam(const std::string & who, const std::string & version, const std::string & params) {
}
void matchKeeper::reqParam(const std::string & who, const std::string & params) {
}
void matchKeeper::ready(const std::string & who, const std::string & version) {
    /*	istringstream in(version);
     unsigned int v;
     char c;
     if(!(in >> v) || in.get(c))
     throw invalidFormat;
     if(v == version_)
     {
     vp::pseudoContainer<client_container, const std::string &> p(clients_,
     secondFunc<std::pair<std::string, boost::shared_ptr<client>, client >
     (boost::bind(&boost::shared_ptr<client>::operator*, _1));
     std::foreach(p.begin(), p.end(), boost::bind(&client::is_ready, _1, version));
     ready_[who] = v;
     }*/
}

void matchKeeper::choose(const std::string & author, const std::string & who) {
}

void matchKeeper::unchoose(const std::string & author, const std::string & who) {
}
//void msg(const std::string & author, const std::string & message);
const roomKeeper & matchKeeper::room() const {
    return room_;
}

matchKeeper::parameter_container & matchKeeper::parameters() {
    return parameters_;
}
const module & matchKeeper::game() const {
    return *module_;
}
/**
 * @brief gets array of parameter names used before a sender and a message to
 * @return pointer to first element of the array, the elements are in the order exactly how they should be used
 */
std::string matchKeeper::chatParameters() const {
    std::string params = escaper_.escape(room_.name());
    params += ' ';
    params += escaper_.escape(keeper::name());
    return params;
}

const std::string & matchKeeper::name() const {
    return static_cast<const keeper * const>(this)->name();
}

matchKeeper::player_container & matchKeeper::players() {
    return players_;
}

const matchKeeper::player_container & matchKeeper::players() const {
    return players_;
}

matchKeeper::player_container & matchKeeper::spectators() {
    return spectators_;
}

const matchKeeper::player_container & matchKeeper::spectators() const {
    return spectators_;
}

std::map<std::string, std::string> & matchKeeper::settings() {
    return settings_;
}

void matchKeeper::round(const std::string & name) {
}

void matchKeeper::end(const std::string & result) {
}

bool matchKeeper::hasClient(const std::string & name) const {
    for(size_t i = 0; i < players_.size(); ++i) {
        if(players_[i]->name() == name) {
            return true;
        }
    }

    for(size_t i = 0; i < spectators_.size(); ++i) {
        if(spectators_[i]->name() == name) {
            return true;
        }
    }

    return false;
}

void matchKeeper::sendToAllClients(const std::string & msg) {
    for(matchKeeper::player_container::iterator it = players_.begin(); it != players_.end(); ++it) {
        (boost::static_pointer_cast<matchClient>(*it))->client().sendMsg(msg);
    }

    for(matchKeeper::player_container::iterator it = spectators_.begin(); it != spectators_.end(); ++it) {
        (boost::static_pointer_cast<matchClient>(*it))->client().sendMsg(msg);
    }
}

std::string matchKeeper::parameterList() const {
    return "";
}

roomKeeper & matchKeeper::room() {
    return room_;
}

bool matchKeeper::state() const {
    return false;
}

/**
 * @brief returns size of the array of parameters
 * @return number of chat parameters
 */
//	size_type chatParametersSize();
/**
 * @brief gets value for the parameter
 * @param name name of the parameter
 * @return value of the parameter
 */
//	const std::string & parameter(const std::string & name);
bg_namespace_END
