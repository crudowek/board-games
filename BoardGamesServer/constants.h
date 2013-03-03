#ifndef BG_CONSTANTS
#define BG_CONSTANTS

#include <string>

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

/** @brief struct grouping all constants used by server all together
 */
struct c {
public:
    /**
     * @brief port used by the server
     */
    static const unsigned short PORT;
    /**
     * @brief unknown exception message to be written to error output
     */
    static const char * const UNKNOWN_EXCEPTION;
    /**
     * @brief folder where to be created main logs
     */
    static const char * const MAIN_LOG_FOLDER;

    /**
     * @brief maximum characters in a username
     */
    static const size_t MAX_NAME_SIZE;
    /**
     * @brief prefix used in generatedId before logging in
     */
    static const char UNLOGGED_PREFIX;
    /**
     * @brief server runned ok
     */
    static const unsigned char STATUS_OK;
    /**
     * @brief server ended by error
     */
    static const unsigned char STATUS_ERROR;
    /**
     * @brief insert failed message
     */
    static const char * const INSERT_FAIL;

    /**
     * @brief ID depleted message
     */
    static const char * const ID_DEPLETED;

    /**
     * @brief STOPPING_SERVER log message
     */
    static const char * const STOPPING_SERVER;
    /**
     * @brief separator
     */
    static const char SEPARATOR;
    /**
     * @brief quote
     */
    static const char QUOTE;
    /**
     * @brief command used to stop server from command line
     * currently not used
     */
    static const char * const STOP_COMMAND;

    /**
     * @brief first command from client and from the server
     */
    static const char * const HI_COMMAND;
    /**
     * @brief chars used in server salt
     */
    static const std::string saltChars;

    /**
     * @brief size of server salt
     */
    static const size_t SALT_SIZE;
    /**
     * @brief LIST command
     */
    static const char * const LIST_COMMAND;
    /**
     * @brief LISTEND command
     */
    static const char * const LISTEND_COMMAND;
    /**
     * @brief OK command
     */
    static const char * const OK_COMMAND;

    /**
     * @brief ERROR command
     */
    static const char * const ERR_COMMAND;
    static const int PROTOCOL;
    static const char * const PROGRAM;
    static const int VERSION;
    static const int LOGGED_CODE;
    static const char * const LOGGED_MESSAGE;
    static const char * const MLIST_COMMAND;
    static const char * const MLISTEND_COMMAND;
    static const char * const PARAM_COMMAND;
    static const char * const RLIST_COMMAND;
    static const char * const RLISTEND_COMMAND;
    static const char * const MATCH_COMMAND;
    static const char * const ROOM_COMMAND;
    static const char * const REND_COMMAND;
    static const char * const MEND_COMMAND;
    static const char * const RJOIN_COMMAND;
    static const char * const RLEAVE_COMMAND;
    static const char BACKSLASH;
    static const char REPLACEMENT;
    static const char * const RMSG_COMMAND;
    static const char * const MMSG_COMMAND;
    static const size_t ROOM_CHAT_PARAMETERS_SIZE;
    static const size_t MATCH_CHAT_PARAMETERS_SIZE;
    static const char * const MODULE_DIR_NAME;
    static const char * const MODULE_FUNCTION_NAME;
    static const char * const CLIENT_CONNECTED;
    static const int LOGIN_COMMAND_PARAMS;
    static const int HI_COMMAND_PARAMS;
    static const char * const LOGIN_COMMAND;
    static const int UNKNOWN_COMMAND_CODE;
    static const char * const TERMINATE_MESSAGE;
    static const int INVALID_NUMBER_PARAMS_CODE;
    static const char * const UNKNOWN_COMMAND_MESSAGE;
    static const char * const INVALID_NUMBER_PARAMS_MESSAGE;
    static const char * const ALREADY_LOGGED_MESSAGE;
    static const int ALREADY_LOGGED_CODE;
    static const char * const LOGIN_FIRST_MESSAGE;
    static const int LOGIN_FIRST_CODE;
    static const int INIT_FIRST_CODE;
    static const char * const INIT_FIRST_MESSAGE;
    static const char * const ROOM_NOT_EXIST_MESSAGE;
    static const int ROOM_NOT_EXIST_CODE;
    static const char * const MATCH_NOT_EXIST_MESSAGE;
    static const int MATCH_NOT_EXIST_CODE;
    static const int MATCH_ALREADY_EXIST_CODE;
    static const char * const MATCH_ALREADY_EXIST_MESSAGE;
    static const int ROOM_ALREADY_EXIST_CODE;
    static const char * const ROOM_ALREADY_EXIST_MESSAGE;
    static const int CLIENT_NOT_EXIST_CODE;
    static const char * const CLIENT_NOT_EXIST_MESSAGE;
    static const char * const CLIENT_LOGGED_IN;
    static const char * const REGISTER_FIRST_MESSAGE;
    static const int REGISTER_FIRST_CODE;
    static const char * const INVALID_PASSWORD_MESSAGE;
    static const int INVALID_PASSWORD_CODE;
    static const char * const LOGIN_FAILURE;
    static const char * const WRITE_ERROR;
    static const char * const QUIT_COMMAND;
    static const int QUIT_COMMAND_PARAMS;
    static const char * const ACCOUNT_IN_USE_MESSAGE;
    static const int ACCOUNT_IN_USE_CODE;
    static const int CLIENT_INPUT_BUFFER_MAX_SIZE;
    static const char * const PMSG_COMMAND;
    static const int PMSG_COMMAND_PARAMS;
    static const char * const GLOBAL_ROOM;
    static const int RMSG_COMMAND_PARAMS;
    static const int RJOIN_COMMAND_PARAMS;
    static const int RLEAVE_COMMAND_PARAMS;
    static const char * const ALREADY_IN_THE_ROOM_MESSAGE;
    static const char * const ALREADY_IN_THE_MATCH_MESSAGE;
    static const int ALREADY_IN_THE_ROOM_CODE;
    static const int ALREADY_IN_THE_MATCH_CODE;
    static const char * const MJOIN_COMMAND;
    static const char * const JOIN_THE_ROOM_FIRST_MESSAGE;
    static const char * const JOIN_THE_MATCH_FIRST_MESSAGE;
    static const int JOIN_THE_ROOM_FIRST_CODE;
    static const int JOIN_THE_MATCH_FIRST_CODE;
    static const char * const RCREATE_COMMAND;
    static const int RCREATE_COMMAND_PARAMS;
    static const int RCREATE_COMMAND_PARAMS_MAX;
    static const int RCREATE_COMMAND_PARAMS_MIN;
    static const int RJOIN_COMMAND_PARAMS_MAX;
    static const int RJOIN_COMMAND_PARAMS_MIN;
    static const int RLIST_COMMAND_PARAMS;
    static const int MLIST_COMMAND_PARAMS;
    static const int LIST_COMMAND_PARAMS;
    static const int GAME_NOT_FOUND_CODE;
    static const char * const GAME_NOT_FOUND_MESSAGE;
    static const char * const MLEAVE_COMMAND;
    static const int MCREATE_COMMAND_PARAMS;
    static const int MJOIN_COMMAND_PARAMS;
    static const int MMSG_COMMAND_PARAMS;
    static const int MLEAVE_COMMAND_PARAMS;
    static const char * const MCREATE_COMMAND;
    static const char * const LIBRARY_NOT_EXIST_MESSAGE;
    static const int LIBRARY_NOT_EXIST_CODE;
    static const char * const LOAD_COMMAND;
    static const int LOAD_COMMAND_PARAMS;
    static const char * const LIBRARY_ALREADY_LOADED_MESSAGE;
    static const int LIBRARY_ALREADY_LOADED_CODE;
    static const int INVALID_LOGIN_PASSWORD_CODE;
    static const char * const INVALID_LOGIN_PASSWORD_MESSAGE;
    static const char * const GAMEDEF_COMMAND;
    static const int GAMEDEF_COMMAND_PARAMS;
    static const char * const DEFATTR_COMMAND;
    static const char * const DEFREQ_COMMAND;
    static const char * const DEFEND_COMMAND;
    static const int PARAMETER_NOT_FOUND_CODE;
    static const char * const PARAMETER_NOT_FOUND_MESSAGE;
    static const char * const ACCESS_DENIED_MESSAGE;
    static const int ACCESS_DENIED_CODE;

};

bg_namespace_END

#endif
