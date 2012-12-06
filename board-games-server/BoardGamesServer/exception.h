#ifndef BG_EXCEPTION
#define BG_EXCEPTION

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

#include "vp/exception.h"

#define bg_namespace_BEGIN namespace bg{
#define bg_namespace_END };

bg_namespace_BEGIN

class exception:public vp::exception {
protected:
    exception(int code, const char * msg);
};

class alreadyLoggedException:public exception {
public:
    alreadyLoggedException();
};

extern alreadyLoggedException alreadyLogged;

class initFirstException:public exception {
public:
    initFirstException();
};

extern initFirstException initFirst;

class accountInUseException:public exception {
public:
    accountInUseException();
};

extern accountInUseException accountInUse;

class loginFirstException:public exception {
public:
    loginFirstException();
};

extern loginFirstException loginFirst;

class roomAlreadyExistException:public exception {
public:
    roomAlreadyExistException();
};

extern roomAlreadyExistException roomAlreadyExist;

class roomNotExistException:public exception {
public:
    roomNotExistException();
};

extern roomNotExistException roomNotExist;

class matchAlreadyExistException:public exception {
public:
    matchAlreadyExistException();
};

extern matchAlreadyExistException matchAlreadyExist;

class matchNotExistException:public exception {
public:
    matchNotExistException();
};

extern matchNotExistException matchNotExist;

class clientNotExistException:public exception {
public:
    clientNotExistException();
};

extern clientNotExistException clientNotExist;

class alreadyInTheRoomException:public exception {
public:
    alreadyInTheRoomException();
};

extern alreadyInTheRoomException alreadyInTheRoom;

class alreadyInTheMatchException:public exception {
public:
    alreadyInTheMatchException();
};

extern alreadyInTheMatchException alreadyInTheMatch;

class joinTheRoomFirstException : public exception {
public:
    joinTheRoomFirstException();
};

extern joinTheRoomFirstException joinTheRoomFirst;

class joinTheMatchFirstException: public exception {
public:
    joinTheMatchFirstException();
};

extern joinTheMatchFirstException joinTheMatchFirst;

class invalidPasswordException: public exception {
public:
    invalidPasswordException();
};

extern invalidPasswordException invalidPassword;

class gameNotFoundException: public exception {
public:
    gameNotFoundException();
};

extern gameNotFoundException gameNotFound;

class libraryNotExistException: public exception {
public:
    libraryNotExistException();
};

extern libraryNotExistException libraryNotExist;

class libraryAlreadyLoadedException: public exception {
public:
    libraryAlreadyLoadedException();
};

extern libraryAlreadyLoadedException libraryAlreadyLoaded;

class parameterNotFoundException: public exception {
public:
    parameterNotFoundException();
};

extern parameterNotFoundException parameterNotFound;

class accessDeniedException: public exception {
public:
    accessDeniedException();
};

extern accessDeniedException accessDenied;

class invalidLoginPassword: public exception {
};

bg_namespace_END

#endif
