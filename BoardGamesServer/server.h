#ifndef BG_SERVER
#define BG_SERVER

#define BOOST_THREAD_USE_LIB

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "crossdependence.h"
#include "client.h"

#include "matchKeeper.h"
#include "roomKeeper.h"
#include "moduleLoader.h"

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/**
 * @brief server class cannot be copy constructed or assigned
 */
class server: boost::noncopyable {
public:
    typedef std::map<std::string, boost::shared_ptr<roomKeeper> > room_container;
    typedef std::map<std::string, boost::shared_ptr<client> > client_container;
private:
    boost::shared_ptr<boost::barrier> barrier_;
    /**
     * @brief map organizing connected clients by id
     */
    client_container clients_;
    /**
     * @brief logger used to log everything
     */
    logger logger_;
    /**
     * @brief standard input buffer
     */
    boost::asio::streambuf inputBuffer_;
    /**
     * @brief standard input parser
     */
    vp::utils::parser<server> inputParser_;
    /**
     * @brief io_service
     *
     */
    boost::asio::io_service io_service_;
    /**
     * @brief tcp acceptor
     */
    boost::asio::ip::tcp::acceptor acceptor_;

    /**
     * @brief pointer running the io service
     */
    boost::shared_ptr<boost::thread> serviceThread_;

    room_container rooms_;

    bg::moduleLoader moduleLoader_;

    //match_container matches_;

    /**
     * @brief continue reading standard input
     */
    volatile bool continue_;

    /**
     * @brief string used to read lines of input we want the thread to hold almost none resources...
     */
    std::string line_;

    void unknownException(boost::shared_ptr<client> cl);

    /**
     * @brief sets the first generated id
     * @param id id
     */
    void firstId(std::string & id);
    /**
     * @brief last assigned generated id
     */
    std::string lastId_;
    /**
     * @brief status of the server
     */
    unsigned char status_;
    /**
     * @brief increment id
     * @param id to be incremented
     * @return 1 - incremented, 0 - could not be incremented
     */
    bool incrementId(std::string & id);
    /**
     * @brief generate id for new client
     * @param id generated id
     * @return true - generated, false - cannot be generated
     */
    bool generateId(std::string & id);
    /**
     * @brief starts accepting incoming tcp connection on designated port
     *
     */
    void startAccept();
    /**
     * @brief starts reading line from a client
     */
    void startRead(boost::shared_ptr<client> cl);

    /**
     * @brief message read
     */
    void read(boost::shared_ptr<client> cl, const boost::system::error_code & error);
    /**
     * @brief creates new client from socket and stores it..
     * @param socket pointer to accepted socket
     * @param error error code
     */
    void accept(boost::shared_ptr<boost::asio::ip::tcp::socket> socket, const boost::system::error_code & error);

    void parse(const std::string & line);
    /**
     * @brief read standard input
     */
    void readInput();
    /**
     * @brief starts reading from std input
     *
     */
    void startReadInput();
    /**
     * @brief stops the server
     * @param params parameters to the command
     */
    void stop(const std::vector<std::string> & params);

    void load(const std::vector<std::string> & params);

    void terminate(client_container::reference pair);

    void printInputError(const char * msg);

    void insertRoom(boost::shared_ptr<roomKeeper> room, const std::string & pname);
public:
    /**
     * @brief default constructor
     */
    server();

    void removeRoom(const std::string & name);

    room_container & rooms();

    client_container & clients();

    const bg::moduleLoader & moduleLoader() const;
    bg::moduleLoader & moduleLoader();

    /**
     * start accepting connection and run the server
     * @return STATUS_OK - OK, STATUS_ERROR - error
     */
    unsigned char run();

    void createRoom(const std::string & rname, const std::string & pname);
    void createRoom(const std::string & rname, const std::string & password, const std::string & pname);
    void sendRoomInfo(const roomKeeper & room);

    /**
     * @brief destructor
     */
    ~server();
};

bg_namespace_END

#endif
