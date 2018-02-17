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
        stroke(0, 50, 0)
        fill(100, 255, 100)
        strokeWeight(0.5)
        ellipse(location[0],location[1],50,50)

def drawTree(start,leaf,level,Angle,startLength,thickness):

    
    if level == 0:
        return
    else:
        end = drawLineAngle((0,0,0),start,Angle,startLength,thickness)
        drawTree(end,leaf,level-1,Angle + 25,startLength-level,level-1)
        drawTree(end,leaf,level-1,Angle - 25,startLength-level,level-1)
        
def keyPressed():
    global leaf
    if key=="l":
        leaf = not leaf

def setup():
    global leaf
    global Angle
    global level
    global thickness
    global changeAngle
    global startLength
    startLength = 100
    changeAngle = 10
    thickness = 20
    level = 3
    Angle = 0
    leaf=True

def draw():
    clear()
    background(255)
    drawTree((550,800),leaf,level,Angle,startLength,thickness)