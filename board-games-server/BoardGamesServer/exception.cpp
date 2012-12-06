#include "exception.h"
#include "constants.h"

bg_namespace_BEGIN

exception::exception(int code, const char * msg):vp::exception(msg, code) {
}

alreadyLoggedException::alreadyLoggedException():exception(c::ALREADY_LOGGED_CODE, c::ALREADY_LOGGED_MESSAGE) {
};

alreadyLoggedException alreadyLogged;

initFirstException::initFirstException():exception(c::INIT_FIRST_CODE, c::INIT_FIRST_MESSAGE) {
};

initFirstException initFirst;

loginFirstException::loginFirstException():exception(c::LOGIN_FIRST_CODE, c::LOGIN_FIRST_MESSAGE) {
};

loginFirstException loginFirst;

accountInUseException::accountInUseException():exception(c::ACCOUNT_IN_USE_CODE, c::ACCOUNT_IN_USE_MESSAGE) {
};

accountInUseException accountInUse;

roomAlreadyExistException::roomAlreadyExistException():exception(c::ROOM_ALREADY_EXIST_CODE, c::ROOM_ALREADY_EXIST_MESSAGE) {
};

roomAlreadyExistException roomAlreadyExist;

roomNotExistException::roomNotExistException():exception(c::ROOM_NOT_EXIST_CODE, c::ROOM_NOT_EXIST_MESSAGE) {
};

roomNotExistException roomNotExist;

matchAlreadyExistException::matchAlreadyExistException():exception(c::MATCH_ALREADY_EXIST_CODE, c::MATCH_ALREADY_EXIST_MESSAGE) {
};

matchAlreadyExistException matchAlreadyExist;

matchNotExistException::matchNotExistException():exception(c::MATCH_NOT_EXIST_CODE, c::MATCH_NOT_EXIST_MESSAGE) {
};

matchNotExistException matchNotExist;

clientNotExistException::clientNotExistException():exception(c::CLIENT_NOT_EXIST_CODE, c::CLIENT_NOT_EXIST_MESSAGE) {
};

clientNotExistException clientNotExist;

alreadyInTheRoomException::alreadyInTheRoomException():exception(c::ALREADY_IN_THE_ROOM_CODE, c::ALREADY_IN_THE_ROOM_MESSAGE) {
};

alreadyInTheRoomException alreadyInTheRoom;

alreadyInTheMatchException::alreadyInTheMatchException():exception(c::ALREADY_IN_THE_MATCH_CODE, c::ALREADY_IN_THE_MATCH_MESSAGE) {
};

alreadyInTheMatchException alreadyInTheMatch;

joinTheRoomFirstException::joinTheRoomFirstException():exception(c::JOIN_THE_ROOM_FIRST_CODE, c::JOIN_THE_ROOM_FIRST_MESSAGE) {
};

joinTheRoomFirstException joinTheRoomFirst;

joinTheMatchFirstException::joinTheMatchFirstException():exception(c::JOIN_THE_MATCH_FIRST_CODE, c::JOIN_THE_MATCH_FIRST_MESSAGE) {
};

joinTheMatchFirstException joinTheMatchFirst;

invalidPasswordException::invalidPasswordException():exception(c::INVALID_PASSWORD_CODE, c::INVALID_PASSWORD_MESSAGE) {
}

invalidPasswordException invalidPassword;

gameNotFoundException::gameNotFoundException(): exception(c::GAME_NOT_FOUND_CODE, c::GAME_NOT_FOUND_MESSAGE) {
}

gameNotFoundException gameNotFound;

libraryNotExistException::libraryNotExistException(): exception(c::LIBRARY_NOT_EXIST_CODE, c::LIBRARY_NOT_EXIST_MESSAGE) {
}

libraryNotExistException libraryNotExist;

libraryAlreadyLoadedException::libraryAlreadyLoadedException(): exception(c::LIBRARY_ALREADY_LOADED_CODE, c::LIBRARY_ALREADY_LOADED_MESSAGE) {
}

libraryAlreadyLoadedException libraryAlreadyLoaded;

parameterNotFoundException::parameterNotFoundException(): exception(c::PARAMETER_NOT_FOUND_CODE, c::PARAMETER_NOT_FOUND_MESSAGE) {
}

parameterNotFoundException parameterNotFound;

accessDeniedException::accessDeniedException():exception(c::ACCESS_DENIED_CODE, c::ACCESS_DENIED_MESSAGE);

accessDeniedException accessDenied;

bg_namespace_END
