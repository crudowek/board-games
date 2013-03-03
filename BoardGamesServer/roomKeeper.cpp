#include "roomKeeper.h"

#include <utility>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

roomKeeper::roomKeeper(const std::string & name, bg::server & srv):name_(name), server_(srv), isPrivate_(false), keeper() {
}

roomKeeper::roomKeeper(const std::string & name, const std::string & password, bg::server & srv):name_(name), password_(password), isPrivate_(true),server_(srv), keeper() {
}

roomKeeper::match_container & roomKeeper::matches() {
    return matches_;
}

const std::string & roomKeeper::name() const {
    return name_;
}

bool roomKeeper::isPrivate() const {
    return isPrivate_;
}

bool roomKeeper::checkPassword(const std::string & password) const {
    return password == password_;
}

void roomKeeper::join(const std::string & who) {
    roomKeeper::client_container::const_iterator itc = server_.clients().find(who);
    clients_.insert(*itc);
    for(roomKeeper::client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->joined_room(*this, who);
    }
}

void roomKeeper::leave(const std::string & who) {
    roomKeeper::client_container::iterator itc = clients_.find(who);
    for(roomKeeper::client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->left_room(*this, who);
    }

    clients_.erase(itc);

    if(clients_.empty()) {
        server_.removeRoom(name());
    }
}

void roomKeeper::createMatch(const std::string & mname, const std::string & pname, const std::string & game,const std::string & settings) {
    roomKeeper::match_container::mapped_type match(new matchKeeper(mname, *this, game, settings));
    matches_.insert(std::make_pair(mname, match));
    match->join(pname, true);
    client & cl = *(server_.clients().find(pname)->second);
    //cl.joined_match(*match, settings, true);
    for(roomKeeper::client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->created_match(*match);
    }
}

void roomKeeper::removeMatch(const std::string & name) {
    match_container::iterator it = matches_.find(name);
    match_container::mapped_type match = it->second;
    matches_.erase(it);
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->removed_match(*match);
    }
}

std::string roomKeeper::chatParameters() const {
    return escaper_.escape(std::string(name_));
}

bg::server & roomKeeper::server() {
    return server_;
}

roomKeeper::client_container & roomKeeper::clients() {
    return clients_;
}

const roomKeeper::client_container & roomKeeper::clients() const {
    return clients_;
}

void roomKeeper::sendMatchInfo(const matchKeeper & match) {
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->match_info(match);
    }
}

void roomKeeper::sendToAllClients(const std::string & msg) {
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->sendMsg(msg);
    }
}

/*void roomKeeper::msg(const std::string & author, const std::string & message){

 }*/

bg_namespace_END
