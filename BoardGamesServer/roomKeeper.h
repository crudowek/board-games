#ifndef BG_ROOMKEEPER
#define BG_ROOMKEEPER

#include "crossdependence.h"
#include "keeper.h"
#include "client.h"
#include "server.h"
#include "matchKeeper.h"

#include <boost/shared_ptr.hpp>

#include <string>
#include <map>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 * @brief Class that creates thread sending periodically information about changes inside room and
 * And also sends information about clients joining/leaving the room to the other one
 */
class roomKeeper: public keeper {
public:
    typedef std::map<std::string, boost::shared_ptr<matchKeeper> > match_container;
    typedef std::map<std::string, boost::shared_ptr<client> > client_container;
private:
    typedef match_container::mapped_type client_type;
    match_container matches_;
    bg::server & server_;
    std::string name_;
    bool isPrivate_;
    std::string password_;

    client_container clients_;

public:
    typedef size_t size_type;

    roomKeeper(const std::string & name, bg::server & srv);

    roomKeeper(const std::string & name, const std::string & password, bg::server & srv);

    match_container & matches();

    const std::string & name() const;

    void join(const std::string & who);

    void leave(const std::string & who);

    void createMatch(const std::string & mname, const std::string & pname, const std::string & game, const std::string & settings);

    void removeMatch(const std::string & name);

    client_container & clients();
    bg::server & server();

    const client_container & clients() const;

    bool isPrivate() const;

    bool checkPassword(const std::string & pw) const;

    void sendMatchInfo(const matchKeeper & match);

    void sendToAllClients(const std::string & msg);

    /**
     * @brief adds client to the room
     * @param which client to be added
     */
    //	void addClient(client & which);
    /**
     * @brief removes client from the room
     * @param which client to be removed
     */
    //	void removeClient(client & which);
private:
    /**
     * @brief gets room name
     * @return name of the room
     */
    std::string chatParameters() const;

    /**
     * @brief returns size of the array of parameters
     * @return number of chat parameters
     */
    //	size_type chatParametersSize() const;
    /**
     * @brief gets value for the parameter
     * @param name name of the parameter
     * @return value of the parameter
     */
    //const std::string & parameter(const std::string & name) const;
};

bg_namespace_END

#endif
