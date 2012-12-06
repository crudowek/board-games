import random
import string
import hashlib

class Security:
    def __init__(self):
        self.__clientString = self.__generateRandomString(32, string.ascii_letters + string.digits)
    
    
    def __generateRandomString(self, size, chars):
        return ''.join(random.choice(chars) for _ in range(size))

    def serverString(self, s):
        self.__serverString = s

    def clientString(self):
        return self.__clientString

    def generateHash(self, pw):
        sha = hashlib.sha1()    
        sha.update(pw.encode('utf-8'))
        sha.update(self.__serverString.encode('utf-8'))
        sha.update(self.__clientString.encode('utf-8'))
        return sha.hexdigest()