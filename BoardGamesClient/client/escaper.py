class Escaper:
    def escape(self, s):
        res = ['"']
        for x in s:
            if x == '\\':
                res[len(res):] = '\\\\'
            elif x == ' ':
                res[len(res):] = '\\,'
            elif x == '"':
                res[len(res):] = '\\"'
            elif x == '\n':
                res[len(res):] = '\\n'
            else:
                res[len(res):] = x


        res[len(res):] = '"'
        return ''.join(res)
        
    def strip(self, s):
        if len(s) < 2 or s[0] != '"':
            return s

        res = []
        backslash = False
        for x in range(1, len(s) - 1):
            c = s[x]
            if backslash:
                backslash = False
                if c == '\\':
                    res[len(res):] = '\\'
                elif c == 'n':
                    res[len(res):] = '\n'
                elif c == ',':
                    res[len(res):] = ' '
                else:
                    res[len(res):] = c
            
            else:
                if c == '\\':
                    backslash = True
                else:
                    res[len(res):] = c
        
        
        return ''.join(res)