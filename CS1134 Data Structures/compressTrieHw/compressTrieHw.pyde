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
            myTextBox = TextBox(child,start, y + 60)
            parent.drawLineToOtherBoxBelow(myTextBox)
            myTextBox.draw()
            self.draw_trie(node.children[child], start, y + 50, myTextBox)
            start += offset*num
            
    def draw(self,x,y):
        self.draw_trie(self.root,x,y)
        
#----------- Compressed Trie---------------

class compressedSuffixTrie:
    
    #HOLDS THE DICTIONARY OF CHILDRENS AND Key,value
    class _Node:
        def __init__(self, initalIndex = 0 , indexLen = 0):
            self.children = {}
            self.kv = (initalIndex,indexLen) #May never be used. However, the keys for the dictionary are formatted as such.
            #the InitalIndex is the index where to start on the string. indexLen = indicates the len of the substring from "initialIndex"
        def print_console(self,level):
            for val in self.children:
                self.children[val].print_console(level+1)
            print


    def print_console(self):
        self.root.print_console(1) 

    def draw(self,x,y):
        self.draw_compress_trie(self.myString,self.root,x,y)
    
    def draw_compress_trie(self,S,node,x,y,parent = None,level = 1):
        if parent == None:
            level = 1
            parent = TextBox("",x,y)
            parent.draw()
        start = x
        print("NODE: " + str(node.children))
        for child in node.children.keys():
            offset = 50
            num = self.num_leaves(node.children[child])
            mySpace = self.mySpace(node.children[child],offset)
            print(str(child) + " " + str(num))
            print("myStart: " + str(start) + " for: " + str(child))
            myTextBox = TextBox(S[child[0]:child[0]+child[1]],start, y + 60)
            parent.drawLineToOtherBoxBelow(myTextBox)
            myTextBox.draw()
            self.draw_compress_trie(S,node.children[child], start, y + 50, myTextBox,level+1)
            print("spacing at level: " + str(level))
            start += (offset*num + myTextBox.width())
    
    def num_leaves(self,node):
        s = 0
        if node.children == {}:
            return 1
        for c in node.children.values():
            s += self.num_leaves(c)
        return s
    
    def mySpace(self,node,offset):
        s = 0
        if node.children == {}:
            return node.kv[0] + node.kv[1]
        for c in node.children.values():
            s += self.mySpace(c,offset)
        return s
    
    def __init__(self,S):
        self.myString = S
        self.root = self._Node()
        curNode = self.root
        sLength = len(S)
        
        #Initalize with the longest suffix first. 
        self.root.children[(0,sLength)] = self._Node(0,sLength)
        
        #Goes through the suffixes                    
        for i in range(1,sLength):
            
            curNode = self.root #brings the curNode back to the root. 
            sufChar = i
            sufLength = sLength - i #sufLength is the length of the suffix. This is used to get the substring, using a given index, not including the length. (index, length)
            #for example, (2,5) is index 2 and length of 5. so it is actually. [2:7] not including the character at 7
            
            insert = False #Used to determine when to move on to the next suffix. Once you are able to insert, your suffix, you can move on. 
            print("Doing: " + S[i:] + "   ("+str(sufChar) + "," + str(sufLength)+")")
            print("curNode Before Operation: " + str(curNode.children))
            while insert == False:
                indexSpace = 0
                Split = False
                foundMatch = False
                c = (0,0)
                
                #Looks for a child that matches 
                for child in curNode.children.keys():
                    if child[0] < sLength and sufChar < sLength and S[sufChar] == S[child[0]]:
                        foundMatch = True
                        print("foundMatch")
                        print(child)
                        print(sufChar)
                        c = child
                        indexSpace,Split = self.splitPoint(S,child,sufChar)
                        print("indexSpace: " + str(indexSpace))
                        print("Split: " + str(Split))
                #If you split, and insert, you are done.
                if Split and foundMatch:
                    print("splitting")
                    #Creates new node that got splitted
                    curNode.children[(c[0],indexSpace)] = self._Node(c[0],indexSpace)
                    
                    #Creates the children on the new node. (This is part of the old node)
                    curNode.children[(c[0],indexSpace)].children[(c[0]+indexSpace,c[1]-indexSpace)] = self._Node(c[0]+indexSpace,c[1]-indexSpace)
                    #(Part of the new suffix)
                    curNode.children[(c[0],indexSpace)].children[(sufChar+indexSpace,sufLength-indexSpace)] = self._Node(sufChar+indexSpace,sufLength-indexSpace)
                    
                    #Carrys over children of old node
                    #for mySons in curNode.children[(c[0],c[1])]
                    
                    print "RIGHT BEFORE DELETE: " + str(curNode.children[(c[0],c[1])].children)
                    
                    for mySons in curNode.children[(c[0],c[1])].children.keys():
                        curNode.children[(c[0],indexSpace)].children[(c[0]+indexSpace,c[1]-indexSpace)].children[mySons] = self._Node(*mySons)
                    
                    del curNode.children[(c[0],c[1])]
                    print("RESULT OF SPLIT: " + str(curNode.children))
                    insert = True
                    
                #if you found a match, but can't split. find respective children on the curNode.
                elif foundMatch == True and not Split:
                    if sufChar + indexSpace > sLength:
                        sufChar -= sLength
                    sufChar+=indexSpace
                    sufLength-=indexSpace
                    curNode = curNode.children[(c[0],c[1])]
                    print(c)
                
                #Just inserts if can't find anything to split
                else:
                    insert=True
                    print("SufLength: " + str(sufLength))
                    curNode.children[(sufChar,sufLength)] = self._Node(sufChar,sufLength)
                #print(curNode.children)
            print("curNode After Operations: " + str(curNode.children))
            print("-----------------------")
                    
    #Tells you how many indexes to move, in order to split.
    def splitPoint(self,S,child,suffix):
        
        childCursor = child[0]
        suffixCursor = suffix
        numCharMatch = 0
        suffixLength = len(S) - suffix
        
        fixThis = 0
        
        if child[0] > child[1]:
            fixThis = child[0]+abs(child[1]-child[0])
            
        elif child[1] == child[0]:
            fixThis = child[1]+child[0]
        else:
            fixThis = child[1]
            
        pleaseWork = 0
        
        if suffixCursor > suffixLength:
            pleaseWork = suffixCursor+abs(suffixCursor-suffixLength)
        elif suffixCursor == suffixLength:
            pleaseWork = suffixCursor+suffixLength
        else:
            pleaseWork = suffixLength
        
        print(childCursor)
        print(suffix)
        print("childCursor Before: " + str(childCursor))
        print("suffixCursor After: " + str(suffixCursor))
        
        while childCursor < fixThis and suffixCursor < pleaseWork and suffixCursor < len(S) and childCursor < len(S) and S[childCursor] == S[suffixCursor]:
            numCharMatch+=1
            childCursor+=1
            suffixCursor+=1
        
        print("childCursor After: " + str(childCursor))
        print("suffixCursor After: " + str(suffixCursor))
        if childCursor >= fixThis:
            print("FUCK THIS")
            return numCharMatch,False
        if numCharMatch == 0:
            return 1,False
        return numCharMatch,True
            
        
def keyPressed():
    global S
    global switch
    
    if key == TAB:
        switch = not switch
    elif key==u'\x08':
        S=S[:-1]
    elif key!=65535:
        S+=key
    redraw()
    
def setup():
    global S
    global switch
    switch = False
    S="mississippi$"
    size(1200, 1000)
    pixelDensity(displayDensity())
    noLoop()
    
def draw():
    global switch
    background(200,150,200)
    TextBox(S,10,10).draw()
    #print("STRING: " + str(S))
    
    if not switch:
        ST=suffixTrie(S)
        ST.draw(50,100)
    #print("HELLO")
    else:
        CST = compressedSuffixTrie(S)
        CST.draw(50,100)
    print("AFTER----------------------")
    #print(CST.root.children)
    #print(CST.root.children[(1,1)].children)
    #print(CST.root.children[(1,1)].children[(2,3)].children)
    #print(S[2:5])
    #CST.print_console()
    #print("ROOT: " + str(ST.root.children))
    #ST.draw(50,100)