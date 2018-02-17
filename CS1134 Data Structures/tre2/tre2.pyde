import sys
import random
import math


def setup():
    size(1100, 1100)
    background(255)
    pixelDensity(displayDensity())

def drawLineAngle(color, start, angle, length, width):
    angle += 180  # make up zero degrees
    end = (start[0] + math.sin(math.radians(angle)) * length,
           start[1] + math.cos(math.radians(angle)) * length)
    stroke(*color)
    if width:
        strokeWeight(width)
    else:
        noStroke()
    line(*(start + end))
    return end

def drawLeaf(location):
    global count
    stroke(0, 50, 0)
    fill(100, 255, 100)
    strokeWeight(0.5)
    ellipse(location[0],location[1],20,20)

count = 0

#Move your mouse around to bend the tree and grow it.
def drawTree(start,leaf,level,Angle,changeAngle,startLength,thickness,num):
    global count
    global myColor
    global changeColor
    
    if changeColor:
        myColor = (random.randint(0,255), random.randint(0,255), random.randint(0,255))
        
    
    if level == 0:
        drawLeaf(start)
        return
    else:
        
        end = drawLineAngle(myColor,start,Angle,startLength,level*2)
        startLength = (height - mouseY)/10
             
        drawTree(end,leaf,level-1,Angle + changeAngle,changeAngle - ((mouseX - (width/2)))/50,startLength-(startLength/level),level-1,num)
        drawTree(end,leaf,level-1,Angle - changeAngle,changeAngle + ((mouseX - (width/2)))/50,startLength-(startLength/level),level-1,num)
        
        
        if leaf: 
            drawLeaf(end)
            
        if num:
            count+=1
            fill(0,0,0)
            text(count,end[0],end[1])
            textAlign(CENTER)

            
            
def keyPressed():
    global leaf
    global num
    global moreLevel
    global lessLevel
    global changeColor
    
    #l for leaves on and off
    if key=="l":
        leaf = not leaf
    #n for showing numbers
    if key=="n":
        num = not num
    #w for increasing levels
    if key=="w":
        moreLevel = not moreLevel
    #s for decreasing levels
    if key=="s":
        lessLevel = not lessLevel
    #c for having it cycle though random colors, press again to stop at a random color.
    if key=="c":
        changeColor = not changeColor
        
def setup():
    global leaf
    global Angle
    global level
    global thickness
    global changeAngle
    global startLength
    global count
    global num
    global endLeaf
    global moreLevel
    global lessLevel
    global changeColor
    global myColor
    myColor = (0,0,0)
    changeColor = False
    lessLevel = False
    moreLevel = False
    endLeaf = False
    num = True
    count = 0
    startLength = 100
    changeAngle = 25
    level = 1
    Angle = 0
    leaf=True

#Changes the mouse Coordinate to act more like the normal coordinates on a normal graph
#With negative coordinates.
def normalCoordinate(mX):
    if mX > width/2:
        return mouseX - (width/2)
    else:
        return mouseX + (width/2)

def draw():
    global count
    global level
    global moreLevel
    global lessLevel
    count = 0
    clear()
    background(255)
    
    
    if moreLevel:
        level+=1
        
    if lessLevel:
        level-=1
        
    moreLevel = False
    lessLevel = False
    print (mouseX - (width/2))
    drawTree((550,800),leaf,level,Angle,changeAngle,startLength,level,num)