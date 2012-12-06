import threading

from .escaper import Escaper
from .security import Security
from .sthread import SThread
from .settings import debug

class Room:
    def __init__(self, name, players, private):
        self.name = name
        self.players = players
        self.private = (private != '0')

class Game:
    def __init__(self, name, width, height, params):
        self.name = name
        self.width = width
        self.height = height
        self.params = params

class Match:
    def __init__(self, room, match, players, spectators, game, state, parver, parameters):
        self.room = room
        self.match = match
        self.players = players
        self.spectators = spectators
        self.game = game
        self.state = state
        self.parver = parver
        self.parameters = parameters

class Server:
    def __init__(self, host, port, gui):
        self.__lock = threading.RLock()
        self.__gui = gui
        self.__escaper = Escaper()
        self.__security = Security()
        self.__sthread = SThread(host, port, self.__parse, self.__serverDisconnected)
        self.__sthread.start()
        self.__sendHi()
        self.__unknownCommandWarning = False
        self.__listEnded = True
        self.__rlistEnded = True
        self.__mlistEnded = True
        self.__login = ''
    
    def __serverDisconnected(self):
        self.__login = ''
        self.__gui.serverDisconnected()
    
    def __send(self, msg):
        self.__lock.acquire()
        if debug > 0:
            print(msg)
        self.__sthread.send(msg)
        self.__lock.release()

    def disconnect(self):
        self.__lock.acquire()
        self.__login = ''
        self.__sthread.disconnect()
        self.__lock.release()

    def __sendHi(self):
        self.__send('HI 1 ' + self.__escaper.escape(self.__security.clientString()) + ' "pyClient" 1')

    def login(self, user, pw):
        if self.__login != '':
            return
        print('login: ' + user + ' pw: ' + pw)
        self.__login = user
        self.__send('LOGIN ' + self.__escaper.escape(user) + ' ' + self.__escaper.escape(self.__security.generateHash(pw)))
    
    def createRoom(self, name, password):
        print('create room ' + name + ' ' + password)
        if(len(password) > 0):
            pw = ' ' + self.__escaper.escape(password)
        else:
            pw = ''
        
        self.__send('RCREATE ' + self.__escaper.escape(name) + pw)
    
    def sendJoinRoom(self, room, pw):
        cmd = 'RJOIN '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(pw)
        self.__send(cmd)
    
    def sendLeaveMatch(self, room, match):
        cmd = 'MLEAVE '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(match)
        self.__send(cmd)
    
    def leaveRoom(self, room):
        cmd = 'RLEAVE '
        cmd += self.__escaper.escape(room)
        self.__send(cmd)
    
    def sendRoomMessage(self, room, msg):
        cmd = 'RMSG '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(msg)
        self.__send(cmd)
    
    def sendJoinMatch(self, room, match, active):
        cmd = 'MJOIN '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(match)
        cmd += ' '
        if active:
            act = 1
        else:
            act = 0
        cmd += str(act)
        self.__send(cmd)
    
    def sendMatchMessage(self, room, match, msg):
        cmd = 'MMSG '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(match)
        cmd += ' '
        cmd += self.__escaper.escape(msg)
        self.__send(cmd)
    
    def sendPrivateMessage(self, whom, message):
        cmd = 'PMSG '
        cmd += self.__escaper.escape(whom)
        cmd += ' '
        cmd += self.__escaper.escape(message)
        self.__send(cmd)
    
    def createMatch(self, room, match, game, parameters):
        cmd = 'MCREATE '
        cmd += self.__escaper.escape(room)
        cmd += ' '
        cmd += self.__escaper.escape(match)
        cmd += ' '
        cmd += self.__escaper.escape(game)
        cmd += ' '
        cmd += self.__escaper.escape(parameters)
        self.__send(cmd)
    
    def loggedName(self):
        return self.__login

    def __parse(self, msg):
        if debug > 0:
            print(msg)
        
        mlist = msg.split(' ')
        args = []
        first = True
        for s in mlist:
            if first:
                first = False
                command = '_Server__bc_' + s
            else:
                print(s)
                args.append(self.__escaper.strip(s))
                print(args[len(args) - 1])
    
        
        try:
            if debug > 0:
                print(command)
                print(*args)    
            
            func = getattr(self, command);
            func(*args)
        except (RuntimeError, AttributeError, TypeError, NameError) as e:
            print(e)
            if not self.__unknownCommandWarning:
                self.__gui.error('Server using unknown commands', 'Download latest client version or contact administrator of the server')
                self.__unknownCommandWarning = True
    
    
    def __bc_HI(self, pver, serverString, server, sver):
        self.__security.serverString(serverString)
        self.__gui.login()
        
    def __bc_OK(self, code, msg, data):
        if code == '200':
            self.__send('LIST')
            self.__send('RLIST')
            self.__gui.loggedIn()
        
    
    def __bc_ERR(self, code, msg, data):
        print(msg);
        print(data);
        if code == '20' or code == '40' or code == '50':
            if not self.__unknownCommand:
                self.__unknownCommand = True
                self.__gui.error('Server does not recognize some of the commands. Please contact an administrator')
        elif code == '60':
            self.__login = ''
            self.__gui.login()        
        elif code == '220' or code == '240' or code == '280':
            self.__login = ''
            self.__gui.loginError(msg)
        else:
            self.__gui.error('Error', msg)
            
    def __bc_LIST(self, game, size, params):
        if self.__listEnded:
            self.__newlist = []
            self.__listEnded = False
        
        ss = size.split(',')
        
        valid = True
        try:
            if len(ss) != 2:
                valid = False
            else:
                width = int(ss[0])
                height = int(ss[1])
                if width < 0 or height < 0:
                    valid = False
        except ValueError as _:
            valid = False
        
        if not valid:
            width = 0
            height = 0
        
            
        
        
        self.__newlist.append(Game(game, width, height, params))
    
    def __bc_LISTEND(self):
        self.__listEnded = True
        self.__gui.drawList(self.__newlist)
    
    def __bc_RLIST(self, name, players, passworded):
        if self.__rlistEnded:
            self.__rlistEnded = False
            self.__newrlist = []
        
        self.__newrlist.append(Room(name, players, passworded))
    
    def __bc_RLISTEND(self):
        self.__rlistEnded = True
        self.__gui.drawRlist(self.__newrlist)
    
    def __bc_RJOIN(self, room, name):
        if name == self.__login:
            self.__gui.joinRoom(room)
            cmd = 'MLIST '
            cmd += self.__escaper.escape(room)
            self.__send(cmd)
        else:
            self.__gui.joinedRoom(room, name)
    
    def __bc_ROOM(self, room, players, private):
        self.__gui.roomInfo(Room(room, players, private))
    
    def __bc_RLEAVE(self, room, client):
        if client == self.__login:
            self.__gui.leaveRoom(room)
        else:
            self.__gui.leftRoom(room, client)
    
    def __bc_REND(self, room):
        self.__gui.roomEnded(room)
    
    def __bc_RMSG(self, room, client, message):
        self.__gui.roomMessage(room, client, message)
    
    def __bc_PMSG(self, client, message):
        self.__gui.privateMessage(client, message)
    
    def __bc_MLIST(self, room, match, players, spectators, game, state, parver, parameters):
        self.__gui.matchInfo(Match(room, match, players, spectators, game, state, parver, parameters))
    
    def __bc_MLISTEND(self, room):
        pass
    
    def __bc_MATCH(self, room, match, players, spectators, game, state, parver, params):
        self.__gui.matchInfo(Match(room, match, players, spectators, game, state, parver, params))
        
    def __bc_MJOIN(self, room, match, who, active):
        if who == self.__login:
            self.__gui.joinMatch(room, match, active)
        else:
            self.__gui.joinedMatch(room, match, who, active)
    
    def __bc_MLEAVE(self, room, match, who):
        if who == self.__login:
            self.__gui.leaveMatch(room, match)
        else:
            self.__gui.leftMatch(room, match, who)
    
    def __bc_MMSG(self, room, match, who, msg):
        self.__gui.matchMessage(room, match, who, msg)
    
    def __bc_MEND(self, room, match):
        self.__gui.matchEnd(room, match)
