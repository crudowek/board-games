import queue, functools

from tkinter import Tk, Toplevel, Entry, Label, Button
from tkinter import Listbox, Menu, Frame, Scrollbar, messagebox
from tkinter import W, E, N, S
from tkinter import DISABLED, NORMAL

from .server import Server

class GUI:
    def __init__(self):
        self.__connected = False
        self.__tk = Tk()
        self.__tk.title("Board Games")
        self.__createMenu()
        self.__queue = queue.Queue()
        self.__tk.protocol("WM_DELETE_WINDOW", self.__close)
        self.__createTopLogin()
        self.__createNewRoomLevel()
        self.__newRoomLevel.destroy()
        self.__topLogin.destroy()
        self.__createFrames()
        self.__tk.geometry("800x600")
        self.__tk.after(500, self.__checkQueue)
    
    def __createNewMatchLevel(self):
        self.__newMatchLevel = Toplevel(self.__tk)
        self.__newMatchLevel.title('New match')
        lab = Label(self.__newMatchLevel, text='Name:')
        lab.grid(row=0, column=0, sticky=W)
        self.__newMatchName = Entry(self.__newMatchLevel, width=20)
        self.__newMatchName.grid(row=0, column=1, sticky=W)
        self.__newMatchCreateButton = Button(self.__newMatchLevel, text='Create', command=self.__newMatchCreate)
        self.__newMatchCreateButton.grid(row=2, column=1, sticky=E)
        self.__newMatchLevel.protocol("WM_DELETE_WINDOW", self.__newMatchClose)
        self.__newMatchGame = Listbox(self.__newMatchLevel)
        for x in self.__gamesSupported:
            self.__newMatchGame.insert(0, x.name)
        self.__newMatchGame.grid(row=1, column=1, sticky=W)
        lab = Label(self.__newMatchLevel, text='Game:')
        lab.grid(row=1, column=0, sticky=W)
    
    def __createNewRoomLevel(self):
        self.__newRoomLevel = Toplevel(self.__tk)
        self.__newRoomLevel.title('New room')
        lab = Label(self.__newRoomLevel, text='Name:')
        lab.grid(row=0, column=0, sticky=W)
        self.__newRoomName = Entry(self.__newRoomLevel, width=20)
        self.__newRoomName.grid(row=0, column=1, sticky=W)
        lab = Label(self.__newRoomLevel, text='Password:')
        lab.grid(row=1, column=0, sticky=W)
        self.__newRoomPassword = Entry(self.__newRoomLevel, width=20, show='*')
        self.__newRoomPassword.grid(row=1, column=1, sticky=W)
        self.__newRoomCreateButton = Button(self.__newRoomLevel, text='Create', command=self.__newRoomCreate)
        self.__newRoomCreateButton.grid(row=2, column=1, sticky=E)
        self.__newRoomLevel.protocol("WM_DELETE_WINDOW", self.__newRoomClose)
    
    
    def __createTopLogin(self):
        self.__topLogin = Toplevel(self.__tk)
        self.__topLogin.title("insert user name and password")
        self.__topLogin.protocol("WM_DELETE_WINDOW", self.__disconnect)
        label = Label(self.__topLogin, text="user name:")
        label.grid(row=0, column=0, sticky=W)
        self.__userBox = Entry(self.__topLogin, width=20)
        self.__userBox.grid(row=0, column=1)
        label2 = Label(self.__topLogin, text="password:")
        label2.grid(row=1, column=0, sticky=W)
        self.__pwBox = Entry(self.__topLogin, show='*', width=20)
        self.__pwBox.grid(row=1, column=1)
        self.__loginButton = Button(self.__topLogin, text="Login", command=self.__login)
        self.__loginButton.grid(row=2, column=1, sticky=E)        
    
    def __close(self):
        print("closing")
        self.__disconnect()
        self.__tk.destroy()
        
    def __checkQueue(self):
        try:
            while True:
                item = self.__queue.get(False)
                item()
        except queue.Empty:
            pass
        
        self.__tk.after(500, self.__checkQueue)

    def __createMenu(self):
        self.__bar = Menu(self.__tk)
        self.__fileMenu = Menu(self.__bar, tearoff=0)
        self.__fileMenu.add_command(label="Connect", command=self.__connect)
        self.__fileMenu.add_command(label="Disconnect", command=self.__disconnect)
        self.__fileMenu.add_command(label="Quit", command=self.__close)
        self.__bar.add_cascade(label="File", menu=self.__fileMenu)
        self.__tk.config(menu=self.__bar)
    
    def __createGameListFrame(self):
        self.__gameListFrame = Frame(self.__tk)
        self.__gameListFrame.grid(row=10, column=30, sticky=W + E + N + S)
        self.__gameListItems = 0
    
    def __createMatchWindow(self, room, match):
        self.__matchesWindows[room][match] = Toplevel(self.__tk)
        scroll = Scrollbar(self.__matchesWindows[room][match])
        scroll.grid(row=10, column=40)
        self.__matchesWindows[room][match].title(match)
        cmd = functools.partial(self.__closeMatch, room, match)
        self.__matchesWindows[room][match].protocol("WM_DELETE_WINDOW", cmd)
        self.__matchesWindowsChat[room][match] = Listbox(self.__matchesWindows[room][match], yscrollcommand=scroll.set)
        
        self.__matchesWindowsPlayers[room][match] = Frame(self.__matchesWindows[room][match])
        self.__matchesWindowsSpectators[room][match] = Frame(self.__matchesWindows[room][match])
        self.__matchesWindowsChatBox[room][match] = Entry(self.__matchesWindows[room][match])
        cmd = functools.partial(self.__sendMatchMessage, room, match)
        butt = Button(self.__matchesWindows[room][match], text="Send", command=cmd)
        self.__matchesWindowsChat[room][match].grid(row=10, column=30, rowspan=11, columnspan=10)
        self.__matchesWindows[room][match].columnconfigure(30, weight=1)
        self.__matchesWindows[room][match].rowconfigure(20, weight=1)
        scroll.config(command=self.__matchesWindowsChat[room][match].yview)
        self.__matchesWindowsPlayers[room][match].grid(row=20, column=10)
        self.__matchesWindowsSpectators[room][match].grid(row=20, column=20)
        self.__matchesWindowsChatBox[room][match].grid(row=30, column=30)
        self.__matchesPlayersButtons[room][match] = dict()
        self.__matchesSpectatorsButtons[room][match] = dict()
        butt.grid(row=30, column=35, columnspan=6)
        self.__matchesPlayersCount[room][match] = 0
        self.__matchesSpectatorsCount[room][match] = 0
        
        
        
    
    def __createRoomListFrame(self):
        self.__roomListFrame = Frame(self.__tk)
        self.__roomListFrame.grid(row=30, column=30, sticky=W + E + N + S)
        self.__roomListItems = 0
        self.__roomButtons = dict()
    
    def __createJoinedRoomsFrame(self):
        self.__joinedRoomsFrame = Frame(self.__tk)
        self.__joinedRoomsFrame.grid(row=40, column=30, sticky=W + E + N + S)
        self.__joinedRoomsItems = 0
        self.__joinedRoomsButtons = dict()
    
    def __createRoomPlayersFrame(self):
        self.__roomPlayersFrame = Frame(self.__tk)
        self.__roomPlayersFrame.grid(row=60, column=10, columnspan=11, sticky=N + W + E + S)
        self.__roomPlayersFrame.columnconfigure(0, weight=1)
        self.__roomPlayersCount = 0
        self.__roomClientsButtons = dict()
    
    def __gridRoomChatFrame(self):
        self.__roomChatFrame.grid(row=50, column=30, rowspan=11, sticky=W + N + S + E)
        self.__scroll.config(command=self.__roomChatFrame.yview)        
    
    def __createMatchesFrame(self):
        self.__matchesButtons = dict()
        self.__matchesButtonsItems = 0
        self.__matchesFrame = Frame(self.__tk)
        self.__matchesFrame.grid(row=45, column=30, sticky=N + S + W + E)                        
    
    def __createFrames(self):
        self.__createGameListFrame()
        lab = Label(self.__tk, text="Supported games:")
        lab.grid(row=10, column=10, columnspan=11, sticky=E)
        lab = Label(self.__tk, text="Room join password:")
        lab.grid(row=20, column=10, columnspan=11, sticky=E)
        self.__joinRoomPassword = Entry(self.__tk, show='*')
        self.__joinRoomPassword.grid(row=20, column=30, sticky=W + E)
        self.__createRoomListFrame()
        self.__roomNewButton = Button(self.__tk, text="New", command=self.__newRoom)
        self.__roomNewButton['state'] = DISABLED
        self.__roomNewButton.grid(row=30, column=10, sticky=W)
        lab = Label(self.__tk, text="Rooms:")
        lab.grid(row=30, column=20, sticky=E)
        lab = Label(self.__tk, text="Joined Rooms:")
        lab.grid(row=40, column=10, columnspan=11, sticky=E)
        self.__createJoinedRoomsFrame()
        self.__playersInsideRooms = dict()
        self.__isActiveRoom = False
        self.__activeRoom = ''
        self.__createRoomPlayersFrame()
        self.__tk.rowconfigure(60, weight=1)
        lab = Label(self.__tk, text="Active room:")
        lab.grid(row=50, column=10, sticky=E)
        self.__activeRoomLabel = Label(self.__tk, text='')
        self.__activeRoomLabel.grid(row=50, column=20, sticky=E)
        self.__roomsChatFrames = dict()
        self.__scroll = Scrollbar(self.__tk)
        self.__scroll.grid(row=50, column=40, rowspan=11, sticky=W + N + S)
        self.__roomChatFrame = Listbox(self.__tk, yscrollcommand=self.__scroll.set)
        self.__gridRoomChatFrame()
        self.__tk.columnconfigure(30, weight=1)
        but = Button(self.__tk, text='Leave', command=self.__sendLeaveRoom)
        but.grid(row=70, column=10, columnspan=11, sticky=W + E + N + S)
        self.__roomChatMsg = Entry(self.__tk)
        self.__roomChatMsg.grid(row=70, column=30, sticky=W + N + S + E)
        but = Button(self.__tk, text='Send ', command=self.__sendRoomMessage)
        but.grid(row=70, column=40, sticky=W + E + N + S)
        self.__privateChatWindows = dict()
        self.__privateChatBox = dict()
        self.__roomsChatMessages = dict()
        self.__privateChatFrame = dict()
        self.__createMatchesFrame()
        lab = Label(self.__tk, text='Matches in room:')
        lab.grid(row=45, column=20, sticky=E)
        self.__matchNewButton = Button(self.__tk, text='New', command=self.__newMatch)
        self.__matchNewButton ['state'] = DISABLED
        self.__matchNewButton .grid(row=45, column=10, sticky=W)
        self.__matchesInRooms = dict()
        self.__gamesSupported = []
        self.__createNewMatchLevel()
        self.__newMatchLevel.destroy()
        self.__matchesWindows = dict()
        self.__matchesWindowsChat = dict()
        self.__matchesWindowsPlayers = dict()
        self.__matchesWindowsSpectators = dict()
        self.__matchesWindowsChatBox = dict()
        self.__matchesPlayersButtons = dict()
        self.__matchesSpectatorsButtons = dict()
        self.__matchesPlayersCount = dict()
        self.__matchesSpectatorsCount = dict()
    
    def __newMatch(self):
        self.__createNewMatchLevel()
        self.__matchNewButton ['state'] = DISABLED
    
    def __sendLeaveRoom(self):
        if self.__isActiveRoom:
            self.__server.leaveRoom(self.__activeRoom)
    
    def __newRoomClose(self):
        self.__newRoomLevel.destroy()
        self.__roomNewButton['state'] = NORMAL
    
    def __newMatchClose(self):
        self.__newMatchLevel.destroy()
        self.__matchNewButton['state'] = NORMAL
    
    
    def __newMatchCreate(self):
        print('test')
        sel = self.__newMatchGame.curselection()
        if len(sel) == 0:
            print('no game selected')
            return
        item = self.__newMatchGame.get(0)
        print(item)
        self.__server.createMatch(self.__activeRoom, self.__newMatchName.get(), item, '')
    
    def __newRoomCreate(self):
        self.__server.createRoom(self.__newRoomName.get(), self.__newRoomPassword.get())
    
    def __sendRoomMessage(self):
        if self.__isActiveRoom:
            self.__server.sendRoomMessage(self.__activeRoom, self.__roomChatMsg.get())
    
    def __newRoom(self):        
        self.__roomNewButton['state'] = DISABLED
        self.__createNewRoomLevel()
        
        
        
    def __clearList(self):
        self.__gameListFrame.destroy()
        self.__createGameListFrame()

    
    def __clearRlist(self):
        self.__roomListFrame.destroy()
        self.__createRoomListFrame()
    
    def __drawRlistItem(self, item):
        t = item.name + '( ' + item.players + ')'
        cmd = functools.partial(self.__sendJoinRoom, item.name)
        if item.private:
            butt = Button(self.__roomListFrame, text=t, background='red', command=cmd)
        else:
            butt = Button(self.__roomListFrame, text=t, command=cmd)
        
        self.__roomButtons[item.name] = butt
        butt.grid(row=0, column=self.__roomListItems, sticky=W)
        self.__roomListItems += 1
        
    
    def __drawListItem(self, item):
        lab = Label(self.__gameListFrame, text=item.name)
        lab.grid(row=0, column=self.__gameListItems)
        self.__gameListItems += 1
    
    def __sendJoinMatch(self, room, match):
        self.__server.sendJoinMatch(room, match, True)
    
    def __drawMatchButton(self, room, match):
        cmd = functools.partial(self.__sendJoinMatch, room, match)
        self.__matchesButtons[match] = Button(self.__matchesFrame, text=match, command=cmd)
        self.__matchesButtons[match].grid(row=0, column=self.__matchesButtonsItems)
        self.__matchesButtonsItems += 1;
    
    def __changeActiveRoom(self, room):
        if not self.__isActiveRoom:
            self.__matchNewButton['state'] = NORMAL
        self.__isActiveRoom = True
        self.__activeRoom = room
        self.__activeRoomLabel.config(text=room)
        self.__roomPlayersFrame.destroy()
        self.__createRoomPlayersFrame()
        for i in range(0, len(self.__playersInsideRooms[room])):
            self.__drawRoomClient(self.__playersInsideRooms[room][i])
        self.__roomChatFrame.grid_forget()
        self.__roomChatFrame = self.__roomsChatFrames[room]
        self.__gridRoomChatFrame()
        self.__matchesFrame.destroy()
        self.__createMatchesFrame()
        for x in self.__matchesInRooms[room]:
            self.__drawMatchButton(room, x)
    
    def __closeMatch(self, room, match):
        self.__server.sendLeaveMatch(room, match)
    
    def __leaveMatch(self, room, match):
        self.__matchesWindows[room][match].destroy()
        del self.__matchesWindows[room][match]
        del self.__matchesWindowsPlayers[room][match]
        del self.__matchesWindowsSpectators[room][match]
        del self.__matchesPlayersCount[room][match]
        del self.__matchesSpectatorsCount[room][match]
        del self.__matchesPlayersButtons[room][match]
        del self.__matchesSpectatorsButtons[room][match]
        del self.__matchesWindowsChat[room][match]

    
    def __leftMatch(self, room, match, who):
        if not match in self.__matchesWindows[room]:
            return
        
        if who in self.__matchesPlayersButtons[room][match]:
            self.__matchesPlayersButtons[room][match][who].destroy()
            del self.__matchesPlayersButtons[room][match][who]
        else:
            self.__matchesSpectatorsButtons[room][match][who].destroy()
            del self.__matchesSpectatorsButtons[room][match][who]
        
    
    def __joinRoom(self, room):
        if not room in self.__playersInsideRooms:
            self.__playersInsideRooms[room] = []
        if not room in self.__roomsChatFrames:
            self.__roomsChatFrames[room] = Listbox(self.__tk, yscrollcommand=self.__scroll.set)
            self.__roomsChatMessages[room] = 0
        cmd = functools.partial(self.__changeActiveRoom, room)
        self.__joinedRoomsButtons[room] = Button(self.__joinedRoomsFrame, text=room, command=cmd)
        self.__joinedRoomsButtons[room].grid(row=0, column=self.__joinedRoomsItems)
        self.__joinedRoomsItems += 1
        self.__matchesInRooms[room] = []
        self.__matchesWindows[room] = dict()
        self.__matchesWindowsChat[room] = dict()
        self.__matchesWindowsPlayers[room] = dict()
        self.__matchesWindowsSpectators[room] = dict()
        self.__matchesWindowsChatBox[room] = dict()
        self.__matchesPlayersButtons[room] = dict()
        self.__matchesSpectatorsButtons[room] = dict()
        self.__matchesPlayersCount[room] = dict()
        self.__matchesSpectatorsCount[room] = dict()
    
    def __joinedRoom(self, room, who):
        if not room in self.__playersInsideRooms:
            self.__playersInsideRooms[room] = []
        self.__playersInsideRooms[room].append(who)
        if self.__isActiveRoom and self.__activeRoom == room:
            self.__drawRoomClient(who)
    
    def receivedMessage(self, s):
        print(s)
    
    def joinRoom(self, room):
        self.__queue.put(functools.partial(self.__joinRoom, str(room)))
    
    def joinedRoom(self, room, who):
        self.__queue.put(functools.partial(self.__joinedRoom, str(room), str(who)))

    def __connect(self):
        if self.__connected:
            self.__disconnect()
        
        self.__top = Toplevel(self.__tk)
        self.__top.title("insert address and port")
        label = Label(self.__top, text="host:")
        label.grid(row=0, column=0)
        self.__hostBox = Entry(self.__top, width=20)
        self.__hostBox.grid(row=0, column=1)
        label2 = Label(self.__top, text="port:")
        label2.grid(row=0, column=2)
        self.__portBox = Entry(self.__top, width=10)
        self.__portBox.grid(row=0, column=3)
        button = Button(self.__top, text="Connect", command=self.__connectTo)
        button.grid(row=1, column=3)
    
    def __drawList(self, l):
        self.__clearList()
        self.__gamesSupported = l
        for i in range(0, len(l)):
            self.__drawListItem(l[i])
    
    def __drawRlist(self, l):
        self.__clearRlist()
        for i in range(0, len(l)):
            self.__drawRlistItem(l[i])

    def __disconnect(self):
        if self.__connected:
            self.__connected = False            
            try:
                self.__server.disconnect()
            except Exception as e:
                print(e)
                self.__error("Connection error", "Error sending data")
            
            self.__topLogin.destroy()
            self.__newRoomLevel.destroy()
            self.__roomListFrame.destroy()
            self.__createRoomListFrame()
            self.__gameListFrame.destroy()
            self.__createGameListFrame()
            self.__joinedRoomsFrame.destroy()
            self.__createJoinedRoomsFrame()
            self.__roomNewButton['state'] = DISABLED
            self.__playersInsideRooms = dict()
            self.__isActiveRoom = False
            self.__roomPlayersFrame.destroy()
            self.__createRoomPlayersFrame()
            self.__activeRoomLabel.config(text='')
            for key in iter(self.__privateChatWindows):
                self.__privateChatWindows[key].destroy()
            self.__privateChatWindows = dict()
            self.__privateChatBox = dict()
            self.__privateChatFrame = dict()
            for key in iter(self.__roomsChatFrames):
                self.__roomsChatFrames[key].destroy()
            self.__roomsChatFrames = dict()
            self.__roomChatFrame.destroy()
            self.__roomChatFrame = Listbox(self.__tk, yscrollcommand=self.__scroll.set)
            self.__gridRoomChatFrame()
            self.__roomsChatMessages = dict()
            self.__matchesFrame.destroy()
            self.__createMatchesFrame()
            self.__matchesInRooms = dict()
            self.__matchNewButton['state'] = DISABLED
            self.__newMatchLevel.destroy()
            self.__gamesSupported = []
            for key in iter(self.__matchesWindows):
                for key2 in iter(self.__matchesWindows[key]):
                    self.__matchesWindows[key][key2].destroy()
            self.__matchesWindows = dict()
            self.__matchesWindowsChat = dict()
            self.__matchesWindowsPlayers = dict()
            self.__matchesWindowsSpectators = dict()
            self.__matchesWindowsChatBox = dict()
            self.__matchesPlayersButtons = dict()
            self.__matchesSpectatorsButtons = dict()
            self.__matchesPlayersCount = dict()
            self.__matchesSpectatorsCount = dict()
            
    
    def __connectTo(self):
        try:
            self.__server = Server(self.__hostBox.get(), int(self.__portBox.get()), self)
        except Exception as e:
            print(e)
            self.__error("Connection error", "Cannot connect to server. Please check address and port fields")
        else:
            self.__connected = True
            self.__top.destroy()

    def __error(self, title, msg):
        messagebox.showerror(title, msg)
    
    def drawList(self, l):
        self.__queue.put(functools.partial(self.__drawList, list(l)))
    
    def drawRlist(self, l):
        self.__queue.put(functools.partial(self.__drawRlist, list(l)))

    def error(self, title, msg):
        self.__queue.put(functools.partial(self.__error, title, msg))
    
    def loginError(self, msg):
        self.__queue.put(functools.partial(messagebox.showerror, "Login error", msg))
        self.__queue.put(self.__enableLogin)

    def login(self):
        self.__queue.put(self.__createLoginFrame)
    
    def __matchInfo(self, match):
        if self.__matchesInRooms[match.room].count(match.match) == 1:
            return
        self.__matchesInRooms[match.room].append(match.match)
        if self.__isActiveRoom and self.__activeRoom == match.room:
            self.__drawMatchButton(match.room, match.match)
            
    
    def matchInfo(self, match):
        cmd = functools.partial(self.__matchInfo, match)
        self.__queue.put(cmd)

    
    def loggedIn(self):
        self.__queue.put(self.__loggedIn)
    
    def __sendJoinRoom(self, room):
        pw = self.__joinRoomPassword.get()
        self.__server.sendJoinRoom(room, pw)
    
    def __roomInfo(self, room):
        t = room.name + '( ' + room.players + ')'
        cmd = functools.partial(self.__sendJoinRoom, room.name)
        if room.name in self.__roomButtons:
            self.__roomButtons[room.name].config(text=t)                
        else:
            if room.private:
                self.__roomButtons[room.name] = Button(self.__roomListFrame, text=t, background='red', command=cmd)
            else:
                self.__roomButtons[room.name] = Button(self.__roomListFrame, text=t, command=cmd)
            self.__roomButtons[room.name].grid(row=0, column=self.__roomListItems)
            self.__roomListItems += 1
    
    def __createPrivateChatWindow(self, who):
        if who in self.__privateChatWindows:
            return
        
        cmd = functools.partial(self.__closePrivateChat, who)
        self.__privateChatWindows[who] = Toplevel(self.__tk)
        self.__privateChatWindows[who].title(who)
        self.__privateChatWindows[who].protocol("WM_DELETE_WINDOW", cmd)
        scroll = Scrollbar(self.__privateChatWindows[who])
        self.__privateChatFrame[who] = Listbox(self.__privateChatWindows[who], yscrollcommand=scroll.set)
        self.__privateChatFrame[who].grid(row=0, column=0, columnspan=2, sticky=W + E + N + S)
        scroll.config(command=self.__privateChatFrame[who].yview)
        scroll.grid(row=0, column=2, sticky=W + E + N + S)
        self.__privateChatBox[who] = Entry(self.__privateChatWindows[who])
        self.__privateChatBox[who].grid(row=1, column=0, sticky=W + E + N + S)
        cmd = functools.partial(self.__sendPrivateMessage, who)
        button = Button(self.__privateChatWindows[who], text='Send', command=cmd)
        button.grid(row=1, column=1, columnspan=2)
        self.__privateChatWindows[who].columnconfigure(0, weight=1)
    
    def __closePrivateChat(self, who):
        self.__privateChatWindows[who].destroy()
        del self.__privateChatWindows[who]
        del self.__privateChatBox[who]
        del self.__privateChatFrame[who]
    
    def __drawRoomClient(self, who):
        cmd = functools.partial(self.__createPrivateChatWindow, who)
        self.__roomClientsButtons[who] = Button(self.__roomPlayersFrame, text=who, command=cmd)
        self.__roomClientsButtons[who].grid(row=self.__roomPlayersCount, column=0, sticky=N + W + E)
        self.__roomPlayersCount += 1
    
    def __sendPrivateMessage(self, whom):
        msg = self.__privateChatBox[whom].get()
        self.__privateChatFrame[whom].insert(0, self.__server.loggedName() + ': ' + msg)
        self.__server.sendPrivateMessage(whom, msg)
    
    def roomInfo(self, room):
        self.__queue.put(functools.partial(self.__roomInfo, room))
    
    def __loggedIn(self):
        self.__topLogin.destroy()
        self.__roomNewButton['state'] = NORMAL
    
    def __leaveRoom(self, room):
        if self.__isActiveRoom and room == self.__activeRoom:
            self.__isActiveRoom = False
            self.__activeRoom = ''
            self.__roomPlayersFrame.destroy()
            self.__createRoomPlayersFrame()
            self.__activeRoomLabel.config(text='')
            self.__roomPlayersCount = 0
            self.__roomChatFrame.destroy()
            self.__roomChatFrame = Listbox(self.__tk, yscrollcommand=self.__scroll.set)
            self.__gridRoomChatFrame()
            self.__matchesButtonsItems = 0
            self.__matchesButtons = dict()
            self.__matchesFrame.destroy()
            self.__matchesFrame = Frame(self.__tk)
            self.__matchesFrame.grid(row=5, column=2, sticky=N + S + W + E)
            self.__matchNewButton['state'] = DISABLED
            self.__newMatchLevel.destroy()

            
        self.__roomsChatFrames[room].destroy()
        del self.__roomsChatFrames[room]
        del self.__playersInsideRooms[room]
        del self.__roomsChatMessages[room]
        self.__joinedRoomsButtons[room].destroy()
        del self.__joinedRoomsButtons[room]
        del self.__matchesInRooms[room]
        for key in iter(self.__matchesWindows[room]):
            self.__matchesWindows[room][key].destroy()
        del self.__matchesWindows[room]
        del self.__matchesWindowsChat[room]
        del self.__matchesWindowsPlayers[room]
        del self.__matchesWindowsSpectators[room]
        del self.__matchesWindowsChatBox[room]
        del self.__matchesPlayersButtons[room]
        del self.__matchesSpectatorsButtons[room]
        del self.__matchesPlayersCount[room]
        del self.__matchesSpectatorsCount[room]
    
    def __leftRoom(self, room, client):
        if room in self.__playersInsideRooms:
            cl = self.__playersInsideRooms[room]
            try:
                cl.remove(room)
            except:
                pass
            
            if self.__isActiveRoom and self.__activeRoom == room:
                self.__roomClientsButtons[client].destroy()
                del self.__roomClientsButtons[client]
            
    
    def leaveRoom(self, room):
        self.__queue.put(functools.partial(self.__leaveRoom, room))
    
    def leftRoom(self, room, client):
        self.__queue.put(functools.partial(self.__leftRoom, room, client))
    
    def __roomEnded(self, room):
        self.__roomButtons[room].destroy()
        del self.__roomButtons[room]
    
    def roomEnded(self, room):
        self.__queue.put(functools.partial(self.__roomEnded, room))
    
    def __privateMessage(self, who, message):
        self.__createPrivateChatWindow(who)
        self.__privateChatFrame[who].insert(0, who + ': ' + message)
    
    def privateMessage(self, who, message):
        self.__queue.put(functools.partial(self.__privateMessage, who, message))
        
            
        
    
    def __createLoginFrame(self):
        self.__topLogin.destroy()
        self.__createTopLogin()

    def __login(self):
        self.__loginButton['state'] = DISABLED
        self.__server.login(self.__userBox.get(), self.__pwBox.get())
    
    def __enableLogin(self):
        self.__loginButton['state'] = NORMAL
    

    def mainloop(self):
        self.__tk.mainloop()
        
    def serverDisconnected(self):
        self.__queue.put(self.__disconnect)
    
    def __roomMessage(self, room, client, message):
        self.__roomsChatMessages[room] += 1
        self.__roomsChatFrames[room].insert(0, client + ': ' + message)    
    
    def roomMessage(self, room, client, message):
        self.__queue.put(functools.partial(self.__roomMessage, room, client, message))
    
    def __matchMessage(self, room, match, client, message):
        self.__matchesWindowsChat[room][match].insert(0, client + ': ' + message)
        
    def matchMessage(self, room, match, client, message):
        self.__queue.put(functools.partial(self.__matchMessage, room, match, client, message))

    def __joinMatch(self, room, match, active):
        if not match in self.__matchesWindows[room]:
            self.__createMatchWindow(room, match)
    
    def __joinedMatch(self, room, match, who, active):
        if not match in self.__matchesWindows[room]:
            self.__createMatchWindow(room, match)
        
        if active:
            frame = self.__matchesWindowsPlayers[room][match]
            buttons = self.__matchesPlayersButtons[room][match]
            pos = self.__matchesPlayersCount[room][match]
        else:
            frame = self.__matchesWindowsSpectators[room][match]
            buttons = self.__matchesSpectatorsButtons[room][match]
            pos = self.__matchesSpectatorsCount[room][match]
        cmd = functools.partial(self.__createPrivateChatWindow, who)
        butt = Button(frame, text=who, command=cmd)
        buttons[who] = butt
        butt.grid(row=pos, column=0)
        pos += 1
        if active:
            self.__matchesPlayersCount[room][match] = pos
        else:
            self.__matchesSpectatorsCount[room][match] = pos
        
    def joinMatch(self, room, match, active):
        self.__queue.put(functools.partial(self.__joinMatch, room, match, active))
    
    def joinedMatch(self, room, match, who, active):
        self.__queue.put(functools.partial(self.__joinedMatch, room, match, who, active))
    
    def leaveMatch(self, room, match):
        self.__queue.put(functools.partial(self.__leaveMatch, room, match))
    
    def leftMatch(self, room, match, who):
        self.__queue.put(functools.partial(self.__leftMatch, room, match, who))
    
    
    def __sendMatchMessage(self, room, match):
        text = self.__matchesWindowsChatBox[room][match].get()
        self.__server.sendMatchMessage(room, match, text)
    
    def __matchEnd(self, room, match):
            if self.__isActiveRoom and self.__activeRoom == room:
                self.__matchesButtons[match].destroy()
                del self.__matchesButtons[match]
            
            self.__matchesInRooms[room].remove(match)
    
    def matchEnd(self, room, match):
        self.__queue.put(functools.partial(self.__matchEnd, room, match))