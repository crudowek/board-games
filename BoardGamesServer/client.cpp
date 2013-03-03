#include "client.h"
#include "assert.h"

#include "constants.h"
#include "security.h"
#include "exception.h"
#include "parameter.h"
#include "moduleLoader.h"
#include "server.h"
#include "req.h"

#include <vp/utils.h>
#include <vp/exception.h>

#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <utility>
#include <string>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

template<typename Container >
std::string client::makeList(const Container & v) {
    std::string res = "";
    bool first = true;
    for(typename Container::const_iterator it = v.begin(); it != v.end(); ++it)
    {
        if(!first) {
            res += ";";
        } else {
            first = false;
        }

        res += *it;
    }
    return res;
}

void client::generateSalt(std::string & gen) {
    boost::random::random_device dev;
    boost::random::uniform_int_distribution<> dis(0, c::saltChars.size() - 1);
    gen.resize(c::SALT_SIZE);
    for(size_t i = 0; i < gen.size(); ++i) {
        gen[i] = c::saltChars[dis(dev)];
    }
}

void client::hi(const std::vector<std::string> & params) {
    if(state_ != init) {
        return;
    }
    clientSalt_ = params[1];
    generateSalt(serverSalt_);
    state_ = unlogged;
    std::string response(c::HI_COMMAND);
    response += ' ';
    response += vp::convert(c::PROTOCOL);
    response += ' ';
    response += escaper_.escape(const_cast<const std::string &>(serverSalt_));
    response += ' ';
    response += escaper_.escape(c::PROGRAM);
    response += ' ';
    response += vp::convert(c::VERSION);
    send(response);
}

bool client::checkUser(const std::string & username, const std::string & hash, bool & bfound) {
    userdb::value_type found;
    bfound = false;
    return (userDb_.find(username, found) &&
            (bfound = true) &&
            checkHash( hash,
                    security_.generateHash(
                            found.second,
                            serverSalt_,
                            clientSalt_ )));
}

bool client::checkHash(const std::string & hex, const std::vector<unsigned char> & hash) const {
    if(2 * hash.size() != hex.size())
    return false;

    for(int i = 0; i < hash.size(); ++i) {
        unsigned char digit = convertHex(hex[2 * i]) * 16 + convertHex(hex[2 * i + 1]);
        if(digit != hash[i])
        return false;
    }

    return true;
}

unsigned char client::convertHex(char c) const {
    switch(c) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        return c - '0';
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        return c - 'A' + 10;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        return c - 'a' + 10;
        default:
        return 255;
    }
}

void client::login(const std::vector<std::string> & params) {
    if(state_ != unlogged) {
        if(state_ == init) {
            throw initFirst;
        } else {
            throw alreadyLogged;
        }
    }

    const std::string & username = params[0];
    const std::string & hash = params[1];
    bool found;
    std::string response;
    if(checkUser(username, hash, found))
    {
        if(server_.clients().find(username) != server_.clients().end()) {
            throw accountInUse;
        }

        logger_.log(logger::information, c::CLIENT_LOGGED_IN);
        logId();
        state_ = ok;
        name_ = username;
        changeId_ = true;
        logId();
        response += c::OK_COMMAND;
        response += ' ';
        response += vp::convert(c::LOGGED_CODE);
        response += ' ';
        response += escaper_.escape(c::LOGGED_MESSAGE);
        response += ' ';
        response += escaper_.escape(username);
    } else {
        logger_.log(logger::error, c::LOGIN_FAILURE);
        logId();
        response += c::ERR_COMMAND;
        response += ' ';
        int code;
        const char * msg;
        if(!found) {
            code = c::REGISTER_FIRST_CODE;
            msg = c::REGISTER_FIRST_MESSAGE;
        } else {
            code = c::INVALID_LOGIN_PASSWORD_CODE;
            msg = c::INVALID_PASSWORD_MESSAGE;
        }
        response += vp::convert(code);
        response += ' ';
        response += escaper_.escape(msg);

        std::string data(c::LOGIN_COMMAND);
        for(int i = 0; i < params.size(); ++i) {
            data += ' ' + escaper_.escape(params[i]);
        }

        response += ' ';
        response += escaper_.escape(data);
    }
    send(response);
}

void client::listOne(const boost::shared_ptr<gameInfo> mod) {
    std::string response(c::LIST_COMMAND);
    const std::vector<std::string> & pNames = mod->paramNames();
    std::vector<parameter> params;
    for(size_t i = 0; i < pNames.size(); ++i) {
        params.push_back(mod->param(pNames[i]));
    }
    response += ' ';
    response += escaper_.escape(mod->name());
    response += ' ';
    response += vp::convert(mod->width());
    response += ' ';
    response += escaper_.escape(
            makeList(vp::pseudoContainer<std::vector<parameter>, std::string>(params, &parameter::listInfo)));
    send(response);
}

void client::list(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    moduleLoader::container modules = server_.moduleLoader().infos();
    std::for_each(modules.begin(),
            modules.end(),
            boost::bind(&client::listOne, this, _1));
    send(c::LISTEND_COMMAND);
}

void client::param(const std::vector<std::string> & params) {
    if(state_ != ok)
    throw loginFirst;
    parameter par = server_.moduleLoader().info(params[0]).param(params[1]);
    send(c::PARAM_COMMAND + ' ' +
            escaper_.escape(par.name()) + ' ' +
            escaper_.escape(par.description()) + ' ' +
            escaper_.escape(par.values()));

}

void client::rlistOne(const server_room_container::reference pair) {
    roomKeeper & room = *pair.second;
    std::string command = c::RLIST_COMMAND;
    command += ' ';
    command += escaper_.escape(room.name());
    command += ' ';
    command += vp::convert(room.clients().size());
    command += ' ';
    command += vp::convert(room.isPrivate());
    send(command);
}

void client::rlist(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    std::for_each(server_.rooms().begin(), server_.rooms().end(), boost::bind(&client::rlistOne, this, _1));
    send(c::RLISTEND_COMMAND);
}

void client::rcreate(const std::vector<std::string> & params) {
    if(params.size() < c::RCREATE_COMMAND_PARAMS_MIN || params.size() > c::RCREATE_COMMAND_PARAMS_MAX) {
        throw vp::invalidNumberParams;
    }

    if(state_ != ok) {
        throw loginFirst;
    }

    if(server_.rooms().find(params[0]) != server_.rooms().end()) {
        throw roomAlreadyExist;
    }

    logger_.log(logger::information, c::RCREATE_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    if(params.size() == c::RCREATE_COMMAND_PARAMS_MAX)
    logger_.addDetail(params[1]);

    if(params.size() < c::RCREATE_COMMAND_PARAMS_MAX) {
        server_.createRoom(params[0], name());
    } else {
        server_.createRoom(params[0], params[1], name());
    }

    boost::shared_ptr<roomKeeper> room = server_.rooms().find(params[0])->second;
    rooms_.insert(std::make_pair(room->name(), boost::weak_ptr<roomKeeper>(room)));
}

void client::rjoin(const std::vector<std::string> & params) {
    if(params.size() < c::RJOIN_COMMAND_PARAMS_MIN || params.size() > c::RJOIN_COMMAND_PARAMS_MAX) {
        throw vp::invalidNumberParams;
    }

    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper & room = *it->second;

    if(room.clients().find(name()) != room.clients().end()) {
        throw alreadyInTheRoom;
    }

    if(room.isPrivate() &&
            (
                    params.size() < c::RJOIN_COMMAND_PARAMS_MAX ||
                    !room.checkPassword(params[1])
            )
    ) {
        throw invalidPassword;
    }

    logger_.log(logger::information, c::RJOIN_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    for(roomKeeper::client_container::iterator it = room.clients().begin();
            it != room.clients().end();
            ++it) {
        joined_room(room, it->second->name());
    }
    room.join(name());
    server_.sendRoomInfo(*it->second);
    rooms_.insert(room_container::value_type(params[0], it->second));
}

void client::rleave(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    room_container::iterator it = rooms_.find(params[0]);
    if(it == rooms_.end()) {
        throw joinTheRoomFirst;
    }

    for(match_container::iterator it = matches_.begin(); it != matches_.end(); ++it) {
        boost::shared_ptr<matchKeeper> match = boost::shared_ptr<matchKeeper>(it->second);
        if(match->room().name() == params[0]) {
            match->leave(name());
            match->room().sendMatchInfo(*match);
            matches_.erase(it);
        }

    }

    logger_.log(logger::information, c::RLEAVE_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    boost::shared_ptr<roomKeeper> room(it->second);
    room->leave(name());
    server_.sendRoomInfo(*room);
    rooms_.erase(it);
}

void client::mlistOne(const std::string & room, const room_match_container::reference p) {
    matchKeeper & m = *(p.second);
    std::string response = c::MLIST_COMMAND;
    response += ' ';
    response += escaper_.escape(m.room().name());
    response += ' ';
    response += escaper_.escape(m.name());
    response += ' ';
    response += escaper_.escape(makeList(vp::pseudoContainer<match::player_container, const std::string &>(m.players(), &bg::matchPlayer::name)));
    response += ' ';
    response += escaper_.escape(makeList(vp::pseudoContainer<match::player_container, const std::string &>(m.spectators(), &bg::matchPlayer::name)));
    response += ' ';
    response += escaper_.escape(m.game().info().name());
    response += ' ';
    response += vp::convert(m.state());
    response += ' ';
    response += vp::convert(m.parameterVersion());
    response += ' ';
    response += escaper_.escape(m.parameterList());
    send(response);
}

void client::mlist(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::const_iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    if(it->second->clients().find(name()) == it->second->clients().end()) {
        throw joinTheRoomFirst;
    }

    std::for_each(it->second->matches().begin(), it->second->matches().end(), boost::bind(&client::mlistOne, this, params[0], _1));
    std::string end = c::MLISTEND_COMMAND;
    end += ' ';
    end += escaper_.escape(params[0]);
    send(end);
}

void client::mjoin(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    if(rooms_.find(params[0]) == rooms_.end()) {
        throw joinTheRoomFirst;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    if(itm->second->hasClient(name())) {
        throw alreadyInTheMatch;
    }

    logger_.log(logger::information, c::MJOIN_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);
    logger_.addDetail(params[2]);

    itm->second->join(name(), params[2] == "1");
    matches_.insert(std::make_pair(itm->second->name(), boost::weak_ptr<matchKeeper>(itm->second)));
    it->second->sendMatchInfo(*itm->second);
}

void client::mleave(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }
    boost::shared_ptr<matchKeeper> match = itm->second;

    if(!match->hasClient(name())) {
        throw joinTheMatchFirst;
    }

    logger_.log(logger::information, c::MLEAVE_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);

    match->leave(name());
    matches_.erase(match->name());
    if(match->players().size() || match->spectators().size())
    it->second->sendMatchInfo(*match);
}

void client::mcreate(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    if(it->second->matches().find(params[1]) != it->second->matches().end()) {
        throw matchAlreadyExist;
    }

    it->second->createMatch(params[1], name(), params[2], params[3]);

    logger_.log(logger::information, c::MCREATE_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);
    logger_.addDetail(params[2]);
    logger_.addDetail(params[3]);

    boost::shared_ptr<matchKeeper> match = it->second->matches().find(params[1])->second;
    matches_.insert(std::make_pair(match->name(), boost::weak_ptr<matchKeeper>(match)));
    //it->second->sendMatchInfo(*match);
}

void client::mset(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    if(!itm->second->isFirstPlayer(name_)) {
        throw accessDenied;
    }

    itm->second->setParam(name(), params[2], params[3]);
}

void client::mreq(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    itm->second->reqParam(name(), params[2]);
}

void client::mready(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    itm->second->ready(name(), params[2]);
}

void client::mchoose(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }
    itm->second->choose(name(), params[2]);
}

void client::munchoose(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    itm->second->unchoose(name(), params[2]);
}

void client::rmsg(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    if(it->second->clients().find(name()) == it->second->clients().end()) {
        throw joinTheRoomFirst;
    }

    logger_.log(logger::information, c::RMSG_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);
    it->second->msg(name(), params[1]);
}

void client::mmsg(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::room_container::iterator it = server_.rooms().find(params[0]);
    if(it == server_.rooms().end()) {
        throw roomNotExist;
    }

    roomKeeper::match_container::iterator itm = it->second->matches().find(params[1]);
    if(itm == it->second->matches().end()) {
        throw matchNotExist;
    }

    if(!itm->second->hasClient(name())) {
        throw joinTheMatchFirst;
    }

    logger_.log(logger::information, c::MMSG_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);
    logger_.addDetail(params[2]);

    itm->second->msg(name(), params[2]);
}

void client::pmsg(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    server::client_container::iterator it =
    params[0][0] != c::UNLOGGED_PREFIX ?
    server_.clients().find(params[0]):
    server_.clients().end();
    if(it == server_.clients().end()) {
        throw clientNotExist;
    }

    logger_.log(logger::information, c::PMSG_COMMAND);
    logId();
    logger_.addDetail(params[0]);
    logger_.addDetail(params[1]);
    it->second->msg(name(), params[1]);
}

void client::gamedef(const std::vector<std::string> & params) {
    if(state_ != ok) {
        throw loginFirst;
    }

    gameInfo & game = server_.moduleLoader().info(params[0]);

    logger_.log(logger::information, c::GAMEDEF_COMMAND);
    logId();
    logger_.addDetail(params[0]);

    std::vector<std::string> paramNames = game.paramNames();
    for(size_t i = 0; i < paramNames.size(); ++i) {
        const parameter & p = game.param(paramNames[i]);
        std::string command = c::DEFATTR_COMMAND;
        command += ' ';
        command += escaper_.escape(p.name());
        command += ' ';
        command += escaper_.escape(p.defaultValue());
        send(command);
    }

    const std::vector<req> & requirements = game.reqs();
    for(size_t i = 0; i < requirements.size(); ++i) {
        std::string command = c::DEFREQ_COMMAND;
        command += ' ';
        command += escaper_.escape(requirements[i].file);
        command += ' ';
        command += escaper_.escape(requirements[i].url);
        send(command);
    }

    send(c::DEFEND_COMMAND);

}

void client::created_match(const matchKeeper & match) {
    match_info(match);
}

void client::removed_match(const matchKeeper & match) {
    std::string command = c::MEND_COMMAND;
    command += ' ';
    command += escaper_.escape(match.room().name());
    command += ' ';
    command += escaper_.escape(match.keeper::name());
    send(command);
}

void client::created_room(const roomKeeper & room) {
    room_info(room);
}

void client::room_info(const roomKeeper & room) {
    if(state_ != ok)
    return;
    std::string response = c::ROOM_COMMAND;
    response += ' ';
    response += escaper_.escape(room.name());
    response += ' ';
    /*response += makeList(
     vp::pseudoContainer<roomKeeper::client_container, const std::string &>(
     room.clients(),
     vp::secondFunc<roomKeeper::client_container::value_type ,const std::string &>(boost::bind(&client::name, this))));*/
    response += vp::convert(room.clients().size());
    response += ' ';
    response += vp::convert(room.isPrivate());
    send(response);
}

void client::match_info(const matchKeeper & match) {
    if(state_ != ok)
    return;
    std::string response = c::MATCH_COMMAND;
    response += ' ';
    response += escaper_.escape(match.room().name());
    response += ' ';
    response += escaper_.escape(match.name());
    response += ' ';
    response += escaper_.escape(makeList(vp::pseudoContainer<match::player_container, const std::string &>(match.players(), &bg::matchPlayer::name)));
    response += ' ';
    response += escaper_.escape(makeList(vp::pseudoContainer<match::player_container, const std::string &>(match.spectators(), &bg::matchPlayer::name)));
    response += ' ';
    response += escaper_.escape(match.game().info().name());
    response += ' ';
    response += vp::convert(match.state());
    response += ' ';
    response += vp::convert(match.parameterVersion());
    response += ' ';
    response += escaper_.escape(match.parameterList());
    send(response);
}

void client::removed_room(const roomKeeper & room) {
    std::string response = c::REND_COMMAND;
    response += ' ';
    response += escaper_.escape(room.name());
    send(response);
}

void client::joined_room(const roomKeeper & room, const std::string & client) {
    std::string response = c::RJOIN_COMMAND;
    response += ' ';
    response += escaper_.escape(room.name());
    response += ' ';
    response += escaper_.escape(client);
    send(response);
}

void client::left_room(const roomKeeper & room, const std::string & client) {
    std::string command = c::RLEAVE_COMMAND;
    command += ' ';
    command += escaper_.escape(room.name());
    command += ' ';
    command += escaper_.escape(client);
    send(command);
}

void client::left_match(const matchKeeper & match, const std::string & client) {
    std::string command = c::MLEAVE_COMMAND;
    command += ' ';
    command += escaper_.escape(match.room().name());
    command += ' ';
    command += escaper_.escape(match.name());
    command += ' ';
    command += escaper_.escape(client);
    send(command);
}

void client::joined_match(const matchKeeper & match, const std::string & client, bool active) {
    std::string response = c::MJOIN_COMMAND;
    response += ' ';
    response += escaper_.escape(match.room().name());
    response += ' ';
    response += escaper_.escape(match.name());
    response += ' ';
    response += escaper_.escape(client);
    response += ' ';
    response += vp::convert(active);
    send(response);
}

boost::asio::streambuf & client::buffer() {
    return buffer_;
}

boost::asio::ip::tcp::socket & client::socket() {
    return *socket_;
}

client::client(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string & id, server & ser, logger & log):
logger_(log),
/*streambuf_(),
 buffer_(&streambuf_),*/
buffer_(c::CLIENT_INPUT_BUFFER_MAX_SIZE),
socket_(socket),
id_(id),
parser_(0, c::SEPARATOR, c::QUOTE),
server_(ser),
userDb_("userDb.txt"),
escaper_(c::QUOTE, c::BACKSLASH, c::SEPARATOR, c::REPLACEMENT),
pending_(false),
state_(init),
terminated_(false),
changeId_(false) {
    logger_.log(logger::information, c::CLIENT_CONNECTED);
    logId();
    parser_.pointer(this);
    parser_.addCommand(c::HI_COMMAND, &client::hi, c::HI_COMMAND_PARAMS);
    parser_.addCommand(c::LOGIN_COMMAND, &client::login, c::LOGIN_COMMAND_PARAMS);
    parser_.addCommand(c::QUIT_COMMAND, &client::quit, c::QUIT_COMMAND_PARAMS);
    parser_.addCommand(c::PMSG_COMMAND, &client::pmsg, c::PMSG_COMMAND_PARAMS);
    parser_.addCommand(c::RMSG_COMMAND, &client::rmsg, c::RMSG_COMMAND_PARAMS);
    parser_.addCommand(c::RJOIN_COMMAND, &client::rjoin, c::RJOIN_COMMAND_PARAMS);
    parser_.addCommand(c::RLEAVE_COMMAND, &client::rleave, c::RLEAVE_COMMAND_PARAMS);
    parser_.addCommand(c::RCREATE_COMMAND, &client::rcreate, c::RCREATE_COMMAND_PARAMS);
    parser_.addCommand(c::RLIST_COMMAND, &client::rlist, c::RLIST_COMMAND_PARAMS);
    parser_.addCommand(c::MLIST_COMMAND, &client::mlist, c::MLIST_COMMAND_PARAMS);
    parser_.addCommand(c::LIST_COMMAND, &client::list, c::LIST_COMMAND_PARAMS);
    parser_.addCommand(c::MMSG_COMMAND, &client::mmsg, c::MMSG_COMMAND_PARAMS);
    parser_.addCommand(c::MCREATE_COMMAND, &client::mcreate, c::MCREATE_COMMAND_PARAMS);
    parser_.addCommand(c::MLEAVE_COMMAND, &client::mleave, c::MLEAVE_COMMAND_PARAMS);
    parser_.addCommand(c::MJOIN_COMMAND, &client::mjoin, c::MJOIN_COMMAND_PARAMS);
}

client::parse_t client::parse() {
    std::istream stream(&buffer_);
    std::string line;
    std::string tmp;
    do {
        std::getline(stream, tmp);
        line += tmp;
    }while(line.size() < 1 || line[line.size() - 1] != '\r');

    line.erase(line.size() - 1);
    try {
        parser_.parse(line);
    } catch(vp::exception & ex) {
        sendError(ex.code(), ex.what(), line);
        error(line, ex);
    }

    if(terminated_)
    return TERMINATE;

    if(changeId_) {
        return CHANGE_ID;
    }

    return NOTHING;
}

void client::sendError(int code, const std::string & message, const std::string & data) {
    std::string response(c::ERR_COMMAND);
    response += ' ';
    response += vp::convert(code);
    response += ' ';
    response += escaper_.escape(message);
    response += ' ';
    response += escaper_.escape(data);
    send(response);
}

const std::string & client::id() const {
    return id_;
}

void client::sendMsg(const std::string & msg) {
    send(msg);
}

void client::quit(const std::vector<std::string> & params) {
    terminate();
}

void client::terminate() {
    if(terminated_)
    return;
    for(match_container::iterator it = matches_.begin(); it != matches_.end(); ++it) {
        boost::shared_ptr<matchKeeper> match = boost::shared_ptr<matchKeeper>(it->second);
        match->leave(name());
        match->room().sendMatchInfo(*match);

    }

    matches_.clear();

    for(room_container::iterator it = rooms_.begin(); it != rooms_.end(); ++it) {
        boost::shared_ptr<roomKeeper> room(it->second);
        room->leave(name());
        server_.sendRoomInfo(*room);
    }

    rooms_.clear();

    logger_.log(logger::information, c::TERMINATE_MESSAGE);
    logId();
    socket_->close();
    terminated_ = true;

}

const std::string & client::name() const {
    return name_;
}

const std::string & client::usedId() const {
    if(state_ == ok) {
        return name();
    } else {
        return id();
    }
}

void client::send(const std::string & what) {
    std::string * msg(new std::string());
    msg->reserve(what.size() + 2);
    *msg += what;
    *msg += '\r';
    *msg += '\n';
    queue_.push(msg);
    checkPending();
}

void client::checkPending() {
    if(!pending_ && queue_.size())
    {
        pending_ = true;
        boost::asio::async_write(*socket_,
                boost::asio::buffer(*queue_.front()),
                boost::bind(&client::writeDone, shared_from_this(), boost::asio::placeholders::error));
    }
}

void client::writeDone(const boost::system::error_code & error) {
    delete queue_.front();
    queue_.pop();
    if(error) {
        logger_.log(logger::error, c::WRITE_ERROR);
        logId();
        logger_.addDetail(error.message());
        terminate();
        return;
    }

    pending_ = false;
    checkPending();
}

void client::msg(const std::string & author, const std::string & message) {
    std::string response = c::PMSG_COMMAND;
    response += ' ';
    response += escaper_.escape(author);
    response += ' ';
    response += escaper_.escape(message);
    send(response);
}

void client::logId() {
    if(state_ == ok) {
        logger_.addDetail(name());
    }
    else {
        logger_.addDetail(socket_->remote_endpoint().address().to_string());
        logger_.addDetail(vp::convert(socket_->remote_endpoint().port()));
    }
}

void client::error(const std::string & line, const vp::exception & ex) {
    logger_.log(logger::error, ex.what());
    logId();
    logger_.addDetail(line);
}

/**
 * @brief destructor
 * need terminate to be sent first and all of the ready handlers called
 */
client::~client() {
    terminate();

    while(!queue_.empty()) {
        delete queue_.front();
        queue_.pop();
    }
}
bg_namespace_END
