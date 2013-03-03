#ifndef BG_CLIENT
#define BG_CLIENT

#include <memory>
#include <istream>
#include <string>
#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/utility.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vp/utils.h>

#include "crossdependence.h"
#include "logger.h"
//#include "roomKeeper.h"
#include "userDb.h"
#include "module.h"
#include "security.h"
#include "server.h"
#include "match.h"
#include "roomKeeper.h"
#include "matchKeeper.h"

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 * @brief Class maintaining connection to the client
 */
class client:boost::noncopyable, public boost::enable_shared_from_this<client>
{
public:
    /**
     * @brief states of the client
     */
    enum state_t {
        /**
         * @brief state before initialization
         */
        init,
        /**
         * @brief state after logging
         */
        ok,
        /**
         * @brief state before logging
         */
        unlogged
    };
    /**
     * @brief parse return options
     */
    enum parse_t {
        CHANGE_ID,
        TERMINATE,
        NOTHING
    };
private:
    typedef std::map<std::string, boost::weak_ptr<roomKeeper> > room_container;
    typedef std::map<std::string, boost::weak_ptr<matchKeeper> > match_container;
    typedef std::map<std::string, boost::shared_ptr<roomKeeper> > server_room_container;
    typedef std::map<std::string, boost::shared_ptr<matchKeeper> > room_match_container;
    /**
     * @brief state of the client
     */
    state_t state_;

    bool changeId_;

    std::queue<std::string *> queue_;

    /**
     * @brief parser used to parse messages from client
     */
    vp::utils::parser<client> parser_;

    /**
     * @brief reference to the server
     */
    server & server_;

    /**
     * @brief logger used to logging all client related stuff
     *
     */
    logger & logger_;
    /**
     * @brief pointer to buffer for read operations
     */
    //boost::shared_ptr<boost::asio::basic_streambuf<std::allocator<char> > > buffer_;
    /**
     * @brief pointer to socket
     */
    boost::shared_ptr<boost::asio::ip::tcp::socket> socket_;
    /**
     * @brief string id of the client
     */
    std::string id_;
    /**
     * @brief server generated salt before logging on
     */
    std::string serverSalt_;

    std::string name_;

    bool pending_;
    bool terminated_;

    boost::asio::streambuf buffer_;

    match_container matches_;
    room_container rooms_;
    userdb userDb_;
    security security_;
    /**
     * @brief client generated salt before logging on
     */
    std::string clientSalt_;
    vp::utils::escaper escaper_;

    template<typename Container>
    std::string makeList(const Container & v);
    /**
     * @brief generates random string
     * @param gen generated string
     */
    void generateSalt(std::string & gen);
    /**
     * @brief parses init message of the client
     * @param params parameters of the command
     */
    void hi(const std::vector<std::string> & params);

    bool checkUser(const std::string & username, const std::string & hash, bool & found);

    bool checkHash(const std::string & hex, const std::vector<unsigned char> & hash) const;

    unsigned char convertHex(char c) const;

    void login(const std::vector<std::string> & params);

    void listOne(const boost::shared_ptr<gameInfo> mod);

    void list(const std::vector<std::string> & params);

    void param(const std::vector<std::string> & params);

    void rlistOne(const server_room_container::reference pair);

    void rlist(const std::vector<std::string> & params);

    void rcreate(const std::vector<std::string> & params);

    void rjoin(const std::vector<std::string> & params);

    void rleave(const std::vector<std::string> & params);

    void mlistOne(const std::string & room, const room_match_container::reference p);

    void mlist(const std::vector<std::string> & params);

    void mjoin(const std::vector<std::string> & params);

    void mleave(const std::vector<std::string> & params);

    void mcreate(const std::vector<std::string> & params);

    void mset(const std::vector<std::string> & params);

    void mreq(const std::vector<std::string> & params);

    void mready(const std::vector<std::string> & params);

    void mchoose(const std::vector<std::string> & params);

    void munchoose(const std::vector<std::string> & params);

    void rmsg(const std::vector<std::string> & params);

    void mmsg(const std::vector<std::string> & params);

    void pmsg(const std::vector<std::string> & params);

    void gamedef(const std::vector<std::string> & params);

    void sendError(int code, const std::string & message, const std::string & data);

    void logId();

    void send(const std::string & what);

    void msg(const std::string & author, const std::string & message);

    void error(const std::string & line, const vp::exception & ex);

    void checkPending();

    void writeDone(const boost::system::error_code & error);

    void quit(const std::vector<std::string> & params);

public:

    void sendMsg(const std::string & msg);

    void created_room(const roomKeeper & room);

    void room_info(const roomKeeper & room);
    void match_info(const matchKeeper & match);

    void removed_room(const roomKeeper & room);

    void joined_room(const roomKeeper & room, const std::string & client);

    void left_room(const roomKeeper & room, const std::string & client);

    void created_match(const matchKeeper & match);

    void joined_match(const matchKeeper & match, const std::string & client, bool active);

    void removed_match(const matchKeeper & match);

    void left_match(const matchKeeper & match, const std::string & client);
    /**
     * @brief gets the buffer
     * @return reference to the buffer
     */
    boost::asio::streambuf & buffer();
    /**
     * @brief gets the socket
     * @return reference to the socket
     */
    boost::asio::ip::tcp::socket & socket();
    /**
     * @brief constructs new client from socket and logger
     * @param socket pointer to socket to the client
     * @param log logger
     * @param ser server
     */
    client(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string & id, server & ser, logger & log);
    /**
     * @brief parses when reading into buffer is done
     * @return what to be done
     */
    parse_t parse();
    /**
     * @brief id getter
     * @return reference to id
     */
    const std::string & id() const;

    const std::string & oldId() const;

    /**
     * @brief handle error during rading from client
     * @param error error
     */
    void terminate();

    const std::string & name() const;

    const std::string & usedId() const;

    /**
     * @brief destructor
     */
    ~client();
};

bg_namespace_END

#endif
