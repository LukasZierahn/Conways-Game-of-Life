import subprocess
import os
import time
from Tkinter import *

class Main(object):
    def __init__(self):
        self.cellList = []
        self.currentTurn = 0
        self.mostCellsAlive = 0
        self.champ = subprocess.Popen(os.path.join(os.path.dirname(__file__), "Conway.exe"), stdin = subprocess.PIPE, stdout = subprocess.PIPE)
        #self.contender = self.champ
        self.contender = subprocess.Popen(os.path.join(os.path.dirname(__file__), "Cpp\Conway\Debug\Conway.exe"), stdin = subprocess.PIPE, stdout = subprocess.PIPE)

        self.Contest(2000)

    def Contest(self, turns):
        self.cellList = []
        self.cellList.append(Cell(0,0))
        self.cellList.append(Cell(1,0))
        self.cellList.append(Cell(1,2))
        self.cellList.append(Cell(3,1))
        self.cellList.append(Cell(4,0))
        self.cellList.append(Cell(5,0))
        self.cellList.append(Cell(6,0))

        self.SetCellList(self.contender)

        self.Turn(self.contender, turns)

        print "It took the contester %d ms to math it" % (self.GetCellList(self.contender))

        self.cellList = []
        self.cellList.append(Cell(0,0))
        self.cellList.append(Cell(1,0))
        self.cellList.append(Cell(1,2))
        self.cellList.append(Cell(3,1))
        self.cellList.append(Cell(4,0))
        self.cellList.append(Cell(5,0))
        self.cellList.append(Cell(6,0))

        self.SetCellList(self.champ)

        self.Turn(self.champ, turns)

        print "It took the defender %d ms to math it" % (self.GetCellList(self.champ))



    def Turn(self, subProc, turns):
        subProc.stdin.write("t") #next turn
        subProc.stdin.write(str(turns) + "\n")

    def GetCellList(self, subProc):
        subProc.stdin.write("r") #send your list

        inp = "yup"
        while inp != "c"and inp != "":
            inp = subProc.stdout.readline().rstrip()
            time.sleep(0.05)

        self.cellList = []
        x = 0
        y = 0
        inp = subProc.stdout.readline().rstrip()
        while inp != "e":
            x, y = inp.split("/", 1)
            self.cellList.append(Cell(x, y))
            inp = subProc.stdout.readline().rstrip()

        return int(subProc.stdout.readline().rstrip())

    def SetCellList(self, subProc):
        subProc.stdin.write("s") #take my list
        subProc.stdin.write(str(len(self.cellList)) + "\n")

        for c in self.cellList:
            subProc.stdin.write(str(c.x) +  "\n")
            subProc.stdin.write(str(c.y) +  "\n")

        subProc.stdin.write("e")


    def __del__(self):
        self.contender.stdin.write("q") #kys
        self.champ.stdin.write("q") #kys

class Cell(object): #this should be a struct but oh well python
    def __init__ (self, x, y):
        self.x = int(x)
        self.y = int(y)

Main()
