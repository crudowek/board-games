import threading
import traceback
import socket

class SThread(threading.Thread):
    def __init__(self, host, port, callback, callbackDone):
        threading.Thread.__init__(self)
        self.__openSocket(host, port)
        self.__file = self.__socket.makefile(encoding='utf-8', newline='\r\n', mode='rb')
        self.__callback = callback
        self.__callbackDone = callbackDone
    
    
    def run(self):
        try:
            line = self.__readLine()        
            while(len(line) != 0):
                if(len(line) >= 2):
                    line = line[:-2]
                self.__callback(line)
                line = self.__readLine()
        except Exception as _:
            traceback.print_exc()
        print("Input from server done")
        self.__callbackDone()

    def __openSocket(self, host, port):
        self.__socket = socket.create_connection((host, port))

    def __readLine(self):
        return str(self.__file.readline(), encoding='utf-8')

    def send(self, msg):
        buf = (msg + '\r\n').encode('utf-8')
        try:
            self.__socket.send(buf)
        except:
            self.disconnect()
            self.__callbackDone()
    
    def disconnect(self):
        print("Disconnecting")
        self.__socket.shutdown(socket.SHUT_RDWR)
        self.__socket.close()