class TextBox:
    TEXTSIZE = 30

    def __init__(self, text, x=0, y=0):
        self._text, self._x, self._y = text, x, y

    def replaceText(self, text):
        self._text = text

    def setLocation(self, x, y):
        self._x, self._y = x, y

    def draw(self):
        textAlign(LEFT, TOP)
        textSize(TextBox.TEXTSIZE)
        rectMode(CORNER)
        fill(255)
        stroke(0)
        strokeWeight(1)
        rect(self._x, self._y, self.width(), self.height())
        fill(0)
        text(self._text, self._x + textWidth(" ") //
             2, self._y - textDescent() // 2)

    def width(self):
        textSize(TextBox.TEXTSIZE)
        return textWidth(self._text + " ")

    def height(self):
        textSize(TextBox.TEXTSIZE)
        return textAscent() + textDescent()

    def drawLineToOtherBoxBelow(self, otherBox):
        stroke(0)
        textSize(TextBox.TEXTSIZE)
        strokeWeight(1)
        line(self._x + self.width() / 2, self._y + self.height(),
             otherBox._x + otherBox.width() / 2, otherBox._y)

#------- Regular Suffix Trie ----------
class suffixTrie:
    class _Node:
        def __init__(self):
            self.children = {}
        
        def print_console(self,level):
            for val in self.children:
                print str(level)+ " "+ val,
                self.children[val].print_console(level+1)
            print        
    #Hello$
    def __init__(self,S):
        
        self.root=self._Node()
        curNode = self.root
        
        #print("MY STRING: " + S)
        for i in range(len(S)):
            #print("I: " + str(i))
            #Suffix
            for sufChar in range(i,len(S)):
                #print(S[sufChar])
                #print("Current Node: " + str(curNode.children))
                if S[sufChar] not in curNode.children:
                    curNode.children[S[sufChar]] = self._Node()
                    #print(curNode.children)
                    curNode = curNode.children[S[sufChar]]
                else:
                    curNode = curNode.children[S[sufChar]]
            curNode = self.root
        #print(self.root.children)
    def print_console(self):
        self.root.print_console(1)
        
    def _num_leaves(self, node):
        s = 0
        if node.children == {}:
            return 1
        for c in node.children.values():
            s += self._num_leaves(c)
        return s
    
    def draw_trie(self, node, x, y, parent=None):
        if parent == None:
            parent = TextBox("",x,y)
            parent.draw()
        start = x
        for child in node.children.keys():
            num = self._num_leaves(node.children[child])
            offset = 50
            tb = TextBox(child,start, y + 60)
            parent.drawLineToOtherBoxBelow(tb)
            tb.draw()
            self.draw_depth_first(node.children[child], start, y + 50, tb)
            start += offset*num
            
    def draw(self,x,y):
        self.draw_depth_first(self.root,x,y)
        
#----------- Compressed Trie---------------

class compressedSuffixTrie:
    class _Node:
        def __init__(self):
            self.children = {}
            
        def print_console(self,level):
            for val in self.children:
                print str(level)+ " "+ val,
                self.children[val].print_console(level+1)
            print   
    def print_console(self):
        self.root.print_console(1)        
    def __init__(self,S):
        
        self.root = self._Node()
        curNode = self.root
        sLength = len(S)
        foundMatch = False
        for i in range(sLength):
            sufChar = i
            print("--------------------")
            print(S[i:])
            print("suffix: (" + str(i) + "," + str(sLength)+ ")")
            print(curNode.children)
            #foundMatch = False  
            ### Original in PyCharm
            while foundMatch == False and len(curNode.children) != 0 and sufChar < sLength: ######### PROBLEM. SHOULDN'T BE REPEATING WORK.
            #for sufChar in range(i,sLength): ### Mechanisms seems to be working. This loop is making the program repeat work. so fucking it up. 
                print("HELLO")
                foundMatch = False            
                ##Checks if anything conflicts / should be splitted 
                for child in curNode.children.keys():
                    #print(child)
                    if S[sufChar] == S[child[0]] and foundMatch == False:
                        print("YES")
                        foundMatch = True
                        indexSpace,noSplit = self.splitPoint(S,child,sufChar) 
                        
                        if not noSplit:
                            print("I split")
                            newKey = (child[0],child[0]+indexSpace)
                            print(newKey)
                            #Splits it. Make new indices and creates two new nodes underneath. One with the old suffix and one with new.
                            curNode.children[newKey] = self._Node()
                            curNode.children[newKey].children[(child[0]+indexSpace,sLength)] = self._Node() #Old Suffix that was there
                            curNode.children[newKey].children[(sufChar+indexSpace,sLength)] = self._Node() #Suffix 
                            print("Deleting: " + str(child) + " VS " + str(sufChar))
                            del curNode.children[child]
                            
                        sufChar += indexSpace
                        
                        for c in curNode.children[newKey].children.keys():
                            if S[c[0]] == S[sufChar]:
                                print("Updated CurNode: " + str(c))
                                print("NewKEY: " + str(newKey))
                                print("CurNode: " + str(curNode.children))
                                curNode = curNode.children[newKey].children[c]  
                                print("CurNode After: "+ str(curNode.children))
                sufChar+=1                     
                #if not foundMatch:
            curNode.children[(i,sLength)] = self._Node()
            print("--------------------")
            curNode = self.root
                    
    #Tells you how many indexes to move, in order to split.
    def splitPoint(self,S,child,suffix):
        
        childCursor = child[0]
        suffixCursor = suffix
        numCharMatch = 0
        sLength = len(S)
        
        while childCursor < child[1] and suffixCursor < sLength and S[childCursor] == S[suffixCursor]:
            numCharMatch+=1
            childCursor+=1
            suffixCursor+=1
           
        if childCursor == child[1]:
            return numCharMatch,True
        return numCharMatch,False
            
        
def keyPressed():
    global S
    if key==u'\x08':
        S=S[:-1]
    elif key!=65535:
        S+=key
    redraw()
    
def setup():
    global S
    S="minimize"
    size(1200, 1000)
    pixelDensity(displayDensity())
    noLoop()
    
def draw():
    background(200,150,200)
    TextBox(S,10,10).draw()
    #print("STRING: " + str(S))
    #ST=suffixTrie(S)
    CST = compressedSuffixTrie(S)
    CST.print_console()
    #print("ROOT: " + str(ST.root.children))
    #ST.draw(50,100)