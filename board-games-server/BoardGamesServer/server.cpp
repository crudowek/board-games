#include "server.h"

#include "logger.h"
#include "constants.h"

#include <string> 
#include <utility>

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

#include <vp/utils.h>
#include <vp/exception.h>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

void server::firstId(std::string & id) {
    id.resize(c::MAX_NAME_SIZE, 1);
    id[0] = c::UNLOGGED_PREFIX;
}

bool server::incrementId(std::string & id) {
    bool ret = false;
    for (std::string::size_type i = 1; i < c::MAX_NAME_SIZE; ++i) {
        if(id[i] < 127) {
            ++id[i];
            ret = true;
            break;
        } else {
            id[i] = -128;
        }
    }
    // if cannot increment revert
    if(!ret) {
        for(std::string::size_type i = 1; i < c::MAX_NAME_SIZE; ++i) {
            id[i] = 127;
        }
    }
    return ret;
}

bool server::generateId(std::string & id) {
    id = lastId_;
    bool ret = incrementId(id);
    lastId_ = id;
    return ret;
}

void server::startAccept() {
    boost::shared_ptr<boost::asio::ip::tcp::socket> socket(new boost::asio::ip::tcp::socket(io_service_));
    acceptor_.async_accept(*socket, boost::bind(&server::accept, this, socket, boost::asio::placeholders::error));
}

void server::startRead(boost::shared_ptr<client> cl) {
    boost::asio::async_read_until(cl->socket(), cl->buffer(), "\r\n",
            boost::bind(&server::read, this, cl, boost::asio::placeholders::error));
}

void server::insertRoom(boost::shared_ptr<roomKeeper> room, const std::string & pname) {
    rooms_.insert(std::make_pair(room->name(), room));
    room->join(pname);
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->created_room(*room);
    }
}

void server::createRoom(const std::string & rname, const std::string & pname) {
    boost::shared_ptr<roomKeeper> room(new roomKeeper(rname, *this));
    insertRoom(room, pname);
}

void server::createRoom(const std::string & rname, const std::string & pw, const std::string & pname) {
    boost::shared_ptr<roomKeeper> room(new roomKeeper(rname, pw, *this));
    insertRoom(room, pname);
}

void server::sendRoomInfo(const roomKeeper & room) {
    if(room.clients().begin() == room.clients().end() && room.name() != c::GLOBAL_ROOM)
    return;
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->room_info(room);
    }
}

void server::removeRoom(const std::string & name) {
    if(name == c::GLOBAL_ROOM)
    return;

    room_container::iterator itr = rooms_.find(name);
    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->removed_room(*itr->second);
    }
    rooms_.erase(itr);
}

void server::read(boost::shared_ptr<client> cl, const boost::system::error_code & error) {
    if(error) {
        cl->terminate();
        logger_.addDetail(boost::system::system_error(error).what());
        clients_.erase(cl->name());
        return;
    }

    try {
        switch(cl->parse()) {
            case client::CHANGE_ID:
            {
                clients_.erase(cl->id());
                clients_.insert(std::make_pair(cl->name(), cl));
                break;
            }
            case client::TERMINATE:
            clients_.erase(cl->usedId());
            return;
            default:
            break;
        }
    }
    catch(...) {
        unknownException(cl);
        return;
    }

    startRead(cl);
}

void server::unknownException(boost::shared_ptr<client> cl) {
    try {
        cl->terminate();
        clients_.erase(cl->usedId());
        logger_.addDetail(c::UNKNOWN_EXCEPTION);
    } catch(...) {
    }
}

void server::accept(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, const boost::system::error_code & error) {
    if(error) {
        logger_.log(logger::error, error.message());
        return;
    }

    std::string id;
    if(!generateId(id)) {
        logger_.log(logger::error, c::ID_DEPLETED);
        status_ = c::STATUS_ERROR;
        return;
    }
    boost::shared_ptr<client> cl(new client(socket, id, *this, logger_));
    client_container::iterator it = clients_.insert(std::make_pair(cl->id(), cl)).first;
    if(it == clients_.end()) {
        logger_.log(logger::bug, c::INSERT_FAIL);
        return;
    }
    startRead(it->second);

    startAccept();
}

void server::printInputError(const char * msg) {
    logger_.log(logger::error, msg);
}

void server::parse(const std::string & line) {
    try {
        inputParser_.parse(line);
    } catch(vp::exception & e) {
        printInputError(e.what());
    }

    barrier_->wait();
}

void server::readInput() {
    while(continue_ && std::getline(std::cin, line_)) {
        barrier_ = boost::shared_ptr<boost::barrier>(new boost::barrier(2));
        io_service_.post(boost::bind(&server::parse, this, line_));
        barrier_->wait();
    };

}

void server::startReadInput() {
    readInput();
    serviceThread_->join();
}

void server::stop(const std::vector<std::string> & params) {
    continue_ = false;
    logger_.log(logger::information, c::STOPPING_SERVER);
    for(std::vector<std::string>::const_iterator it = params.begin(); it != params.end(); ++it) {
        logger_.log(logger::information, *it);
    }

    for(client_container::iterator it = clients_.begin(); it != clients_.end(); ++it) {
        it->second->terminate();
    }

    clients_.clear();

    acceptor_.close();

}

void server::load(const std::vector<std::string> & params) {
    moduleLoader_.loadLibrary(params[0]);
    logger_.log(logger::information, c::LOAD_COMMAND);
    logger_.addDetail(params[0]);
}

void server::terminate(client_container::reference pair) {
    pair.second->terminate();
}

server::server():acceptor_(io_service_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), c::PORT)),
logger_(c::MAIN_LOG_FOLDER),
inputParser_(0, c::SEPARATOR, c::QUOTE) {
    status_ = c::STATUS_OK;
    firstId(lastId_);
    boost::shared_ptr<roomKeeper> room(new roomKeeper(c::GLOBAL_ROOM, *this));
    rooms_.insert(room_container::value_type(room->name(), room));
}

server::room_container & server::rooms() {
    return rooms_;
}

server::client_container & server::clients() {
    return clients_;
}

unsigned char server::run() {
    std::vector<std::string> stopParams;
    continue_ = true;
    inputParser_.pointer(this);
    inputParser_.addCommand(c::STOP_COMMAND, &server::stop);
    inputParser_.addCommand(c::LOAD_COMMAND, &server::load);
    const char * err = 0;
    try {
        startAccept();
        //starting running the service thread

        serviceThread_ = boost::shared_ptr<boost::thread>
        (new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service_)));
        //io_service_.run();
        startReadInput();
    } catch(boost::system::system_error & exception) {
        err = exception.what();
    } catch(std::exception & e) {
        err = e.what();
    } catch(...) {
        err = c::UNKNOWN_EXCEPTION;
    }
    if(err) {
        io_service_.post(boost::bind(&server::stop, this, boost::ref(stopParams)));
        io_service_.post(boost::bind(static_cast<void (logger::*)(logger::message_type, const char *) >(&logger::log), &logger_, logger::error, err));
        serviceThread_->join();
        return c::STATUS_ERROR;
    }
    return status_;
}

const bg::moduleLoader & server::moduleLoader() const
{
    return moduleLoader_;
}

bg::moduleLoader & server::moduleLoader()
{
    return moduleLoader_;
}

server::~server() {
}

bg_namespace_END
