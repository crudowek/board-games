#include "constants.h"

#include "client.h"

#include <vp/utils.h>

bg_namespace_BEGIN

const char * const c::UNKNOWN_EXCEPTION = "unknown exception has occured";
const char * const c::MAIN_LOG_FOLDER = "main";
const char * const c::INSERT_FAIL = "insert failed when it should not";
const char * const c::ID_DEPLETED = "temporary ids depleted";
const char * const c::STOPPING_SERVER = "Stopping server";
const char * const c::STOP_COMMAND = "stop";
const char * const c::HI_COMMAND = "HI";
const std::string c::saltChars = "abcdefghijklmnopqrstuvwxyz"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"1234567890"
"!@#$%^&*()";
const char * const c::LIST_COMMAND = "LIST";
const char * const c::LISTEND_COMMAND = "LISTEND";
const char * const c::OK_COMMAND = "OK";
const char * const c::ERR_COMMAND = "ERR";
const char * const c::PROGRAM = "Original Board Games Server";
const int c::PROTOCOL = 1;
const int c::VERSION = 1;

const int c::LOGGED_CODE = 200;
const char * const c::LOGGED_MESSAGE = "logged in";
const char * const c::MLIST_COMMAND = "MLIST";
const char * const c::MLISTEND_COMMAND = "MLISTEND";
const char * const c::PARAM_COMMAND = "PARAM";
const char * const c::RLIST_COMMAND = "RLIST";
const char * const c::RLISTEND_COMMAND = "RLISTEND";
const char * const c::MATCH_COMMAND = "MATCH";
const char * const c::ROOM_COMMAND = "ROOM";
const char * const c::REND_COMMAND = "REND";
const char * const c::MEND_COMMAND = "MEND";
const char * const c::RJOIN_COMMAND = "RJOIN";
const char * const c::RLEAVE_COMMAND = "RLEAVE";
const char * const c::RMSG_COMMAND = "RMSG";
const char * const c::MMSG_COMMAND = "MMSG";
const size_t c::ROOM_CHAT_PARAMETERS_SIZE = 2;
const size_t c::MATCH_CHAT_PARAMETERS_SIZE = 3;
const char * const c::MODULE_DIR_NAME = "modules";
const char * const c::MODULE_FUNCTION_NAME = "createGameInfo";
const char * const c::CLIENT_CONNECTED = "Client connected";
const int c::LOGIN_COMMAND_PARAMS = 2;
const int c::HI_COMMAND_PARAMS = 4;
const char * const c::LOGIN_COMMAND = "LOGIN";
const int c::UNKNOWN_COMMAND_CODE = 20;
const char * const c::UNKNOWN_COMMAND_MESSAGE = "Unidentified command";
const char * const c::TERMINATE_MESSAGE = "terminating connection";
const char * const c::INVALID_NUMBER_PARAMS_MESSAGE = "Invalid number of parameters";
const int c::INVALID_NUMBER_PARAMS_CODE = 40;
const char c::BACKSLASH = '\\';
const char c::REPLACEMENT = ',';
const size_t c::SALT_SIZE = 128;
const char c::QUOTE = '"';
const char c::SEPARATOR = ' ';
const unsigned char c::STATUS_ERROR = 1;
const unsigned char c::STATUS_OK = 0;
const char c::UNLOGGED_PREFIX = '&';
const size_t c::MAX_NAME_SIZE = 30;
const unsigned short c::PORT = 35548;
const char * const c::ALREADY_LOGGED_MESSAGE = "Already logged in.";
const int c::ALREADY_LOGGED_CODE = 260;
const char * const c::LOGIN_FIRST_MESSAGE = "Log in first.";
const int c::LOGIN_FIRST_CODE = 60;
const int c::INIT_FIRST_CODE = 50;
const char * const c::INIT_FIRST_MESSAGE = "Hi first.";
const char * const c::ROOM_NOT_EXIST_MESSAGE = "Room does not exist.";
const int c::ROOM_NOT_EXIST_CODE = 340;
const char * const c::MATCH_NOT_EXIST_MESSAGE = "Match does not exist.";
const int c::MATCH_NOT_EXIST_CODE = 345;
const int c::MATCH_ALREADY_EXIST_CODE = 362;
const char * const c::MATCH_ALREADY_EXIST_MESSAGE = "Match already exists.";
const int c::ROOM_ALREADY_EXIST_CODE = 360;
const char * const c::ROOM_ALREADY_EXIST_MESSAGE = "Room already exists.";
const int c::CLIENT_NOT_EXIST_CODE = 320;
const char * const c::CLIENT_NOT_EXIST_MESSAGE = "The message can not be delivered, because the user is not logged on.";
const char * const c::CLIENT_LOGGED_IN = "Client logged in";
const char * const c::REGISTER_FIRST_MESSAGE = "The account does not exist. Register first.";
const int c::REGISTER_FIRST_CODE = 220;
const int c::INVALID_PASSWORD_CODE = 240;
const char * const c::INVALID_PASSWORD_MESSAGE = "Incorrect password.";
const char * const c::LOGIN_FAILURE = "login failure";
const char * const c::WRITE_ERROR = "write error";
const char * const c::QUIT_COMMAND = "QUIT";
const int c::QUIT_COMMAND_PARAMS = 0;
const char * const c::ACCOUNT_IN_USE_MESSAGE = "Account is already in use.";
const int c::ACCOUNT_IN_USE_CODE = 280;
const int c::CLIENT_INPUT_BUFFER_MAX_SIZE = 8*1024;
const char * const c::PMSG_COMMAND = "PMSG";
const int c::PMSG_COMMAND_PARAMS = 2;
const char * const c::GLOBAL_ROOM = "global";
const int c::RMSG_COMMAND_PARAMS = 2;
const int c::RJOIN_COMMAND_PARAMS = vp::utils::parser<client>::NO_PARAM_CHECK;
const int c::RLEAVE_COMMAND_PARAMS = 1;
const char * const c::ALREADY_IN_THE_ROOM_MESSAGE = "Room already joined.";
const char * const c::ALREADY_IN_THE_MATCH_MESSAGE = "Match already joined.";
const int c::ALREADY_IN_THE_ROOM_CODE = 342;
const int c::ALREADY_IN_THE_MATCH_CODE = 347;
const char * const c::MJOIN_COMMAND = "MJOIN";
const char * const c::JOIN_THE_ROOM_FIRST_MESSAGE = "Join the room first.";
const char * const c::JOIN_THE_MATCH_FIRST_MESSAGE = "Join the match first.";
const int c::JOIN_THE_ROOM_FIRST_CODE = 80;
const int c::JOIN_THE_MATCH_FIRST_CODE = 85;
const char * const c::RCREATE_COMMAND = "RCREATE";
const int c::RCREATE_COMMAND_PARAMS = vp::utils::parser<client>::NO_PARAM_CHECK;
const int c::RCREATE_COMMAND_PARAMS_MAX = 2;
const int c::RCREATE_COMMAND_PARAMS_MIN = 1;
const int c::RJOIN_COMMAND_PARAMS_MAX = 2;
const int c::RJOIN_COMMAND_PARAMS_MIN = 1;
const int c::RLIST_COMMAND_PARAMS = 0;
const int c::MLIST_COMMAND_PARAMS = 1;
const int c::LIST_COMMAND_PARAMS = 0;
const int c::GAME_NOT_FOUND_CODE = 310;
const char * const c::GAME_NOT_FOUND_MESSAGE = "Game not supported";
const char * const c::MLEAVE_COMMAND = "MLEAVE";
const int c::MJOIN_COMMAND_PARAMS = 3;
const int c::MLEAVE_COMMAND_PARAMS = 2;
const int c::MMSG_COMMAND_PARAMS = 3;
const int c::MCREATE_COMMAND_PARAMS = 4;
const char * const c::MCREATE_COMMAND = "MCREATE";
const char * const c::LIBRARY_NOT_EXIST_MESSAGE = "Library does not exist or it is invalid.";
const int c::LIBRARY_NOT_EXIST_CODE = 4;
const char * const c::LOAD_COMMAND = "load";
const int c::LOAD_COMMAND_PARAMS = 1;
const char * const c::LIBRARY_ALREADY_LOADED_MESSAGE = "Library already loaded";
const int c::LIBRARY_ALREADY_LOADED_CODE = 3;
const int c::INVALID_LOGIN_PASSWORD_CODE = 240;
const char * const c::INVALID_LOGIN_PASSWORD_MESSAGE = "Incorrect login password.";
const char * const c::GAMEDEF_COMMAND = "GAMEDEF";
const int c::GAMEDEF_COMMAND_PARAMS = 1;
const char * const c::DEFATTR_COMMAND = "DEFFATTR";
const char * const c::DEFREQ_COMMAND = "DEFREQ";
const char * const c::DEFEND_COMMAND = "DEFEND";
const int c::PARAMETER_NOT_FOUND_CODE = 420;
const char * const c::PARAMETER_NOT_FOUND_MESSAGE = "Parameter wasn't found.";
const char * const c::ACCESS_DENIED_MESSAGE = "Access denied.";
const int c::ACCESS_DENIED_CODE = 90;

bg_namespace_END
