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
        def __init__(self, b = 0 , f = 0):
            self.children = {}
            self.begin = b
            self.finish = f
        global S
        
        def draw(self,c,x,y):
            textbox=TextBox(S[c.begin:c.finish],x,y)
            textbox.draw()
            xend=x
            for childChar in self.children:
                (xend,childbox)=self.children[childChar].draw(self.children[childChar],xend,y+70)
                textbox.drawLineToOtherBoxBelow(childbox)
            return (max(xend,x+textbox.width()+10),textbox)
    
    def print_console(self):
        self.root.print_console(1) 
    

    def draw(self,x,y):
        self.root.draw(self.root,x,y)
    
    def __init__(self,S):
        self.root=self._Node(0,0)
        curNode = self.root
        
        #print("MY STRING: " + S)
        for i in range(len(S)):
            #print("I: " + str(i))
            #Suffix
            for sufChar in range(i,len(S)):
                #print(S[sufChar])
                #print("Current Node: " + str(curNode.children))
                if S[sufChar] not in curNode.children:
                    curNode.children[S[sufChar]] = self._Node(sufChar,sufChar+1)
                    #print(curNode.children)
                    curNode = curNode.children[S[sufChar]]
                else:
                    curNode = curNode.children[S[sufChar]]
            curNode = self.root

        self.compressor(self.root)
        #print(self.root.children)
    
    def compressor(self,parent):
        
        for child in parent.children.values():
            #If you have child has children, keep going down.
            if len(child.children) > 0:    
                self.compressor(child) 
                
            #If you have 1 child and your child's child has no children. 
            #Set my children to my grandkids (If my child had more than 1, aka the point where it splits) 
            #Set my ending Index to my grandkids. (This would be the furthest ending index at the leave. since it went down)
            #and passed up the finish from each node. 
            
            if len(parent.children) == 1:
                myGrandson = parent.children[parent.children.keys()[0]]
                parent.finish = myGrandson.finish
                parent.children = myGrandson.children
            
            #This effectively gets rid of nodes as you move the pointers away from some nodes, and have them deleted by the garbage collector. 
            #Achieving O(n) space.
                
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
    #print("AFTER----------------------")
    #print(CST.root.children)
    #print(CST.root.children[(1,1)].children)
    #print(CST.root.children[(1,1)].children[(2,3)].children)
    #print(S[2:5])
    #CST.print_console()
    #print("ROOT: " + str(ST.root.children))
    #ST.draw(50,100)