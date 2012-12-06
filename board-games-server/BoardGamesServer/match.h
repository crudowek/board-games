#ifndef BG_MATCH
#define BG_MATCH

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

#include <string>
#include <vector>
#include <map>

#include <boost/utility.hpp>
#include <boost/shared_ptr.hpp>

bg_namespace_BEGIN

class matchPlayer;
class matchCounter;
class match;
class matchObjectPlacement;
/** action types
 *
 * PA primary action
 * SA secondary action
 * DRAG drag
 * HOLD hold
 * RELEASE release
 */
enum action_t {
    PA = 1, SA = 2, DRAG = 3, HOLD = 4, RELEASE = 5
};

/**
 * Exception to be thrown by the modules when the settings of the match are unsupported / against the rules
 */
class unsupportedSettingsException {
};

/**
 * Exception to be thrown when there is no object to return reference to
 */
class noObjectAssignedException {
};

/**
 * match Object
 */
class matchObject {
public:
    /**
     * name of the object
     * @return reference to name of the object
     */
    const std::string & name() const;
    /**
     * Allow or disallow actions to be performed on the object
     * @param enable enable
     */
    void enabled(bool enable);
    /**
     * enabled
     * @return enabled
     */
    bool enabled() const;
};

/**
 * Group of object placement to control them easier
 */
class placementGroup {
private:
    /**
     * default constructor
     *
     * should not be used
     */
    placementGroup();
    /**
     * copy constructor
     *
     * should not be used
     * @param original object from which new one will be copy constructed
     */
    placementGroup(const placementGroup & original);
    /**
     * assignment operator
     *
     * should not be used
     * @param r object to be assigned
     * @return reference to the object to which was assigned
     */
    placementGroup & operator=(const placementGroup & r);
public:
    /**
     * constructor
     *
     * @param name name of the group
     */
    placementGroup(const std::string & name);
    /**
     * name of the group
     *
     * @return name of the group
     */
    const std::string & name();
    /**
     * adds object placement to the group
     *
     * @param placement placement to be added
     */
    void addPlacement(matchObjectPlacement & placement);

    /**
     * removes object placement from the group
     *
     */
    void removePlacement(matchObjectPlacement & placement);
    /**
     * enables or disables all of the object placements in the group
     *
     * @param enable enable
     */
    void enabled(bool enable);
    /**
     * gets enabled status of the group
     *
     * @return enabled
     */
    bool enabled() const;
};

/**
 * action that can be performed on the match object
 *
 */
class placementAction {
private:
    /**
     * assignment operator
     *
     * should not be used
     * @param r placementAction to be assigned
     * @return reference to which was assigned
     */
    placementAction & operator=(const placementAction & r);
protected:
    /**
     * copy constructor
     *
     *  @param action original action to be copied
     */
    placementAction(const placementAction & action);
    /**
     * default constructor
     */
    placementAction();
    /**
     * copy constructor
     *
     * @param placement placement to be copied
     */
    placementAction(matchObjectPlacement & placement);

public:
    /**
     * returns type of placement action
     *
     *  @return type of action
     */
    virtual action_t actionType();
    /**
     * return reference to object which is the action performable on
     *
     * @throws noObjectAssignedException
     * @return reference to Object
     */
    matchObject & Object();
    /**
     * return reference to object which is the action performable on
     *
     * @throws noObjectAssignedException
     * @return reference to Object
     */
    const matchObject & Object() const;
    /**
     * destructor
     */
    virtual ~placementAction();

};

/**
 * primary action
 */
class pa: public placementAction {
public:
    /**
     * constructor
     *
     *@param placement related placement
     */
    pa(const matchObject & placement);
    /**
     * default constructor
     *
     */
    pa();
    /**
     * action type
     *
     *@return action type
     */
    action_t actionType() const;
};

/**
 * secondary action
 */
class sa: public placementAction {
public:
    /**
     * constructor
     *
     * @param placement related object
     */
    sa(const matchObjectPlacement & placement);
    /**
     * default constructor
     *
     */
    sa();
    /**
     * action type
     *
     *@return type of action
     */
    action_t actionType() const;
};

/**
 * hold action
 *
 */
class hold: public placementAction {
public:
    /**
     * constructor
     *
     *  @param placement object to be action performed on
     */
    hold(const matchObjectPlacement & placement);
    /**
     * default constructor
     */
    hold();
    /**
     * type of action
     * @return type of action
     */
    action_t actionType() const;
};

/**
 * release action
 *
 */
class release: public placementAction {
public:
    /**
     * constructor
     *
     * @param placement object on which action will be performable
     * @param where at which object it can be released
     */
    release(const matchObjectPlacement & placement, const std::string & where);
    /**
     * constructor
     *
     * @param where at which it can be released
     */
    release(const std::string & where);
    /**
     * type of action
     *
     *  @return type of action
     */
    action_t actionType() const;
    /**
     * at which object it can be released
     *
     * @return name of the object at which it can be released
     */
    const std::string & where() const;
};

/**
 * drag and drop action
 *
 *
 */
class drag: public placementAction {
public:
    /**
     * constructor
     *
     * @param placement object on which the action can be performed
     * @param where name of the object at which the object can be dropped
     *
     */
    drag(const matchObjectPlacement & placement, const std::string & where);
    /**
     * constructor
     *
     * @param where name of the object at which the object can be dropped
     */
    drag(const std::string & where);
    /**
     * type of action
     *
     * @return type of action
     */
    action_t actionType() const;
    /**
     * at which object it can be dropped
     *
     *  @return reference to name of the object it can be dropped at
     */
    const std::string & where() const;
};

/**
 * Object to be placed on the board
 *
 */
class matchObjectPlacement:public matchObject {
private:
    /**
     * copy constructor
     *
     * should not be used
     * @param original object to be copied
     */
    matchObjectPlacement(const matchObjectPlacement & original);
    /**
     * assignment operator
     *
     * should not be used
     * @param original object to be assigned
     * @return reference to object to which is to be assigned
     */
    matchObjectPlacement & operator=(const matchObjectPlacement & original);
    /**
     * default constructor
     *
     * should not be used
     */
    matchObjectPlacement();
public:
    /**
     * remove action performable on the object
     * @param action action to be performed
     */
    void removeAction(placementAction & action);
    /**
     * player at whose board object is placed
     *
     * @return reference to player
     */
    matchPlayer & player();
    /**
     * player at whose board object is placed
     *
     * @return reference to player
     */
    const matchPlayer & player() const;

    const std::string & name() const;

};

/**
 * Object to be placed outside board..
 *
 */
class data: public matchObject {
private:
    /**
     * copy constructor
     *
     * should not be used
     * @param original object to be copied
     */
    data(const data & original);
    /**
     *  assignment operator
     *
     * should not be used
     * @param r object to be assigned
     * @return reference to object to which is to be assigned
     */
    data & operator=(const data & r);
public:
    /**
     * constructor
     *
     *@param name name of the object
     *@param shown name to be shown in GUI
     */
    data(const std::string & name, const std::string & shown);
    /**
     *@brief sets shown text
     *@param s text to be shown
     */
    void shown(const std::string & s);
    /**
     * shown name
     *
     * @return reference to the shown name
     */
    const std::string & shown() const;
    /**
     * sets name of the user which are data related to
     *
     *  @param username reference to the new username
     */
    void username(const std::string & username);
    /**
     * name of the user which are data related to
     *
     *  @return reference to username
     */
    const std::string & username() const;

};

/**
 * button outside the board
 *
 */
class button: public matchObject {
private:
    /**
     * copy constructor
     *
     * should not be used
     * @param original button to be copied
     */
    button(const button & original);
    /** assignment operator
     *
     * should not be used
     * @param r button to be assigned
     * @return button to which is to be assigned
     */
    button & operator=(const button & r);
public:
    /**
     * constructor
     *
     * @param name name of the object
     * @param shown name shown on the GUI
     */
    button(const std::string & name, const std::string & shown);
    /**
     * name shown on the GUI
     *
     * @return name shown on the GUI
     */
    const std::string & shown() const;
};

class view {
    /**
     * @brief adds group to the view
     * @param group reference group to be added
     */
    void addGroup(placementGroup & group);
    /**
     * @brief vector object placements on the players board
     *
     *
     *
     * @return vector of matchObjectPlacements
     */
    std::vector<matchObjectPlacement> & objectPlacements();
    /**
     * @brief vector of object placements on the players board
     *
     *
     *
     * @return vector of matchObjectPlacements
     */
    const std::vector<matchObjectPlacement> & objectPlacements() const;
    /**
     * move, change svg file and size of the object placement
     *
     *  @param name name of the object placement
     *  @param file name of the svg object file
     *  @param x x coordinate on the board
     *  @param y y coordinate on the board
     *  @param width width on the board
     *  @param height height on the board
     *  @return reference to the changed object placement
     */
    matchObjectPlacement & changeObjectPlacement(const std::string & name,
            const std::string & file,
            size_t x, size_t y, size_t width, size_t height);
    /**
     * move, change svg file and size of the object placement
     *
     *  @param which object placement
     *  @param file name of the svg object file
     *  @param x x coordinate on the board
     *  @param y y coordinate on the board
     *  @param width width on the board
     *  @param height height on the board
     */
    void changeObjectPlacement(matchObjectPlacement & which,
            const std::string & file,
            size_t x, size_t y, size_t width, size_t height);
    /**
     * finds object placement by its name
     *
     * @return reference to object placement
     */
    matchObjectPlacement & objectPlacement(const std::string & name);
    /**
     * sets to logical size of the players board
     *
     *@param width new width of the board
     *@param height new height of the board
     */
    void changeBoard(size_t width, size_t height);
    /**
     * finds object placement by its name
     *
     * @return reference to object placement
     */
    const matchObjectPlacement & objectPlacement(const std::string & name) const;
    /**
     * deletes all object placements on the players board
     */
    void clear();
    /**
     *@brief gets data object by name
     *@param name name of the data object
     */
    data & dataObject(const std::string & name);

    /**
     *@brief gets data object by name
     *@param name name of the data object
     */
    const data & dataObject(const std::string & name) const;

    /**
     * gets reference to counter
     *
     *@param name name of the counter
     *@return reference to counter
     */
    matchCounter & counter(const std::string & name);
    /**
     * gets reference to counter
     *
     *@param name name of the counter
     *@return reference to counter
     */
    const matchCounter & counter(const std::string & name) const;
};

/**
 * player in the match
 *
 */
class matchPlayer {
protected:
    /**
     * default constructor
     *
     * should not be used
     */
    matchPlayer();
private:
    /**
     *
     * copy constructor
     *
     * should not be used
     *
     * @param original player to be copied
     */
    matchPlayer(const matchPlayer & original);
    /**
     * assignment operator
     *
     * should not be used
     *
     * @param r player to be assigned
     * @return player to which is to be assigned
     */
    matchPlayer & operator=(const matchPlayer & r);

public:
    /**
     * username of the player
     *
     * @return username of the player
     */
    virtual const std::string & name() const = 0;
    /**
     * match played by the player
     *
     * @return reference to match played by the player
     */
    virtual bg::match & match() = 0;
    /**
     * match played by the player
     *
     * @return reference to match played by the player
     */
    virtual const bg::match & match() const = 0;
    /**
     * @brief gets player's view
     * @return reference to view
     */
    virtual bg::view & view() = 0;
    /**
     * @brief gets player's view
     * @return reference to view
     */
    virtual const bg::view & view() const = 0;

    /**
     * @brief sets player's view
     * Removes all object placements from the board and sets new ones
     * @param v view to be set
     */
    virtual void view(bg::view & v) = 0;
    /**
     * is player a spectator
     *
     * @return is player a spectator
     */
    virtual bool spectator() const = 0;
    /**
     * is player still connected
     *
     * @return is player still connected
     *
     */
    virtual bool connected() const = 0;
    virtual ~matchPlayer() = 0;
};

/**
 * time counter
 */
class matchCounter:boost::noncopyable {
    /**
     * name of the counter
     *
     * @return reference to name of the counter
     */
    const std::string & name() const;
    /**
     *
     * match to which the counter is related
     * @return match to which the counter is related
     */
    bg::match & match();
    /**
     * starts the counting
     */
    void start();
    /**
     *
     * stops the counting
     */
    void stop();
    /**
     * sets the value on the counter
     *
     * @param amount new amount of time left
     */
    void set(size_t amount);
    /**
     *
     * how much is left on the counter
     *
     * @return how much is left on the counter
     */
    size_t left();

};

/**
 * logger
 *
 */
class moduleLogger {
private:
    /**
     * assignment operator
     *
     * should not be used
     * @param r logger to be assigned
     * @return reference to logger to which it is being assigned
     */
    moduleLogger & operator=(const moduleLogger & r);
    /**
     * copy constructor
     *
     * should not be used
     *
     * @param original logger to copied
     */
    moduleLogger(const moduleLogger & original);
public:
    /**
     * constructor
     *
     * opens the file
     *
     * @param filename name of the file to be logged into
     *
     */
    moduleLogger(const std::string & filename);
    /**
     * log
     *
     * @param what is to be logged
     */
    void log(const std::string & what);
    /**
     * destructor
     *
     * closes the file
     */
    ~moduleLogger();
};

/**
 * recorder
 */
class recorder:boost::noncopyable {
private:
    /**
     * assignment operator
     *
     * should not be used
     * @param r recorder to be assigned
     * @return reference to recordr to which it is being assigned
     */
    recorder & operator=(const recorder & r);
    /**
     * copy constructor
     *
     * should not be used
     *
     * @param original recorder to copied
     */
    recorder(const recorder & original);
public:
    /**
     * constructor
     *
     * opens the file
     *
     * @param filename name of the file to be logged into
     *
     */
    recorder(const std::string & filename);
    /**
     * log
     *
     * @param what is to be logged
     */
    void record(const std::string & what);
    /**
     * destructor
     *
     * closes the file
     */
    ~recorder();
};

/**
 *
 * match
 */
class match:boost::noncopyable {
public:
    typedef std::vector<boost::shared_ptr<matchPlayer> > player_container;
    /**
     * name of the match
     *
     * @return reference to name of the match
     */
    virtual const std::string & name() const = 0;
    virtual const player_container & spectators() const = 0;
    virtual player_container & spectators() = 0;
    /**
     * vector of players
     *
     *
     * @return vector of players
     */
    virtual player_container & players() = 0;
    /**
     * vector of players
     *
     *
     * @return vector of players
     */
    virtual const player_container & players() const = 0;
    /**
     * map of the parameters for the match
     * @return reference map of the parameters mapped by name
     */
    virtual std::map<std::string, std::string> & settings() = 0;
    /**
     * ends round of the play
     *
     *@param name name of the round
     */
    virtual void round(const std::string & name) = 0;
    /**
     * ends the match
     *
     * @param result result to be sent to clients
     */
    virtual void end(const std::string & result) = 0;

    virtual ~match() = 0;

};

bg_namespace_END

#endif
