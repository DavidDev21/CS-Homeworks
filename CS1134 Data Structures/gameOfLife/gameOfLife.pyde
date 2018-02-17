import sys
import math
import random
import copy

def setup():
    global cellSize
    global population
    global nextGen
    global pause
    pause = False
    cellSize = 200
    population = [[0 for i in range(0,width,cellSize)]for x in range(0,height,cellSize)]
    population[0][1] = 1
    population[1][1] = 1
    population[2][1] = 1
    nextGen = copy.deepcopy(population)#[[0 for i in range(0,width,cellSize)]for x in range(0,height,cellSize)]
    size(600,600)
    background(255)
    pixelDensity(displayDensity())
    

def drawGrid(cellSize):
    global population
    global nextGen
    for x in range(0,width,cellSize):
        for y in range(0,height,cellSize):
            if nextGen[x//cellSize][y//cellSize] == 0:
               fill(255,255,255)
               rect(x,y,cellSize,cellSize)
            else:
                fill(0,0,0)
                rect(x,y,cellSize,cellSize)
    
                
    if pause == False:            
        liveOrDie()
        population = copy.deepcopy(nextGen)
    population=copy.deepcopy(nextGen)
def mousePressed():
    global population
    if population[mouseX//cellSize][mouseY//cellSize]:
        population[mouseX//cellSize][mouseY//cellSize] = 0
    else:
        population[mouseX//cellSize][mouseY//cellSize] = 1
        

def checkForMe(x,y):
    global population
    global nextGen 
    print 'population'
    print population 
    print 'First Next Gen'
    print nextGen
    #exit()
    neighbor=0  
    #nextGen = copy.deepcopy(population) 
    row_count=width//cellSize;
    #print row_count
    
    for i in range(-1,2):
        for j in range(-1,2):
            
            if(i,j)!=(0,0):
                if ((x+i)<row_count) and ((x+i)>=0) and ((y+j) < row_count) and ((y+j)>=0):
                    if x==1 and y==0:
                        print 'tmp',
                        print i , j
                        #if i==0 and j==1:
                            #print 'tmp ',
                            #print population[x+i][y+j]
                    if(population[x+i][y+j]==1):
                        neighbor+=1  
                        #if x==1 and y==0:
                        #    print "neighbor++ at "+str(x+i)+', '+str(y+j)
    if x==1 and y==1:
        print neighbor
        print population[x][y]
                            
    if population[x][y]==1 and (neighbor < 2 or neighbor > 3):
        nextGen[x][y] = 0

    elif population[x][y]==0 and neighbor == 3:
        nextGen[x][y] = 1
    if x==1 and y==1:
        print nextGen
    #if neighbor>=3:
    #    print 'neightbor'
    
    print 'next gen'
    print nextGen
    print
    #exit()
    

def liveOrDie():
    global population
    global nextGen
    for x in range(0,width,cellSize):
        for y in range(0,height,cellSize):     
            cellX = x//cellSize
            cellY = y//cellSize   
            checkForMe(cellX,cellY)
            
                       
                            
def keyPressed():
    global pause    
    if key == " ":
        pause = not pause
        
def draw():
    global population
    global nextGen
    global pause
    global cellSize
    clear()
    background(255)    
    drawGrid(cellSize)
    