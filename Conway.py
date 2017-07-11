import subprocess
import os
import time
from Tkinter import *

class Main(object):
    def __init__(self):
        self.cellList = []
        self.cellListToken = []
        self.currentTurn = 0
        self.mostCellsAlive = 0
        self.position = []
        self.token = False;
        self.autoplay = False;
        self.root = Tk()
        self.subProc = subprocess.Popen(os.path.join(os.path.dirname(__file__), "Cpp\Conway\Release\Conway.exe"), stdin = subprocess.PIPE, stdout = subprocess.PIPE)
        self.subProcToken = subprocess.Popen(os.path.join(os.path.dirname(__file__), "Conway.exe"), stdin = subprocess.PIPE, stdout = subprocess.PIPE)
        self.run = True

        self.scrollSpeed = 20
        self.keyDict = {'w' : False, 's' : False, 'a' : False, 'd' : False, 'q' : False, 'e' : False }

        self.offsetx = 0
        self.offsety = 0

        self.root.bind("<Key>", self.Keys)
        self.root.bind("<KeyRelease>", self.Keys)

        self.controlFrame = Frame(self.root)
        self.controlFrame.pack()

        self.controlFrameUp = Frame(self.controlFrame)
        self.controlFrameUp.pack(side = TOP, fill = X)

        self.controlFrameDown = Frame(self.controlFrame)
        self.controlFrameDown.pack(side = TOP, fill = X)

        self.turnBut = Button(self.controlFrameUp, text = "Autoplay", command = lambda: self.TurnAndUpdate(0))
        self.turnBut.pack(side = LEFT)

        self.turnBut = Button(self.controlFrameUp, text = "Turn", command = lambda: self.TurnAndUpdate(1))
        self.turnBut.pack(side = LEFT)

        self.turnButA = Button(self.controlFrameUp, text = "5 Turns", command = lambda: self.TurnAndUpdate(15))
        self.turnButA.pack(side = LEFT)

        self.toggelTokenBut = Button(self.controlFrameUp, text = "Toggle Token Algorithm", command = self.FlipToken)
        self.toggelTokenBut.pack(side = LEFT)

        self.cellListLabel = Label(self.controlFrameDown, text = "Cell List:")
        self.cellListLabel.pack(side = LEFT)

        self.pCellList = Button(self.controlFrameDown, text = "Push", command = self.SetCellList)
        self.pCellList.pack(side = LEFT)

        self.pCellList = Button(self.controlFrameDown, text = "Get", command = self.GetCellList)
        self.pCellList.pack(side = LEFT)

        self.pCellList = Button(self.controlFrameDown, text = "Push Token", command = self.SetCellListToken)
        self.pCellList.pack(side = LEFT)

        self.displayFrame = Frame(self.root)
        self.displayFrame.pack(side = LEFT, fill = Y)

        self.currentTurnLabel = Label(self.displayFrame, text = "Current Turn: 0")
        self.currentTurnLabel.pack(side = TOP)

        self.cellsAliveLabel = Label(self.displayFrame, text = "Cells alive: 0")
        self.cellsAliveLabel.pack(side = TOP)

        self.mostCellsAliveLabel = Label(self.displayFrame, text = "Most cells alive: 0")
        self.mostCellsAliveLabel.pack(side = TOP)

        self.currentlyLookingAtLabel = Label(self.displayFrame, text = "a.cgol")
        self.currentlyLookingAtLabel.pack(side = TOP, fill = X)

        self.loadNextButton = Button(self.displayFrame, text = "Load Next", command = self.LoadNextPosition)
        self.loadNextButton.pack(side = TOP, fill = X)

        self.loadNextButton = Button(self.displayFrame, text = "Load Start", command = self.LoadStart)
        self.loadNextButton.pack(side = TOP, fill = X)

        self.loadNextButton = Button(self.displayFrame, text = "Load End", command = self.LoadEnd)
        self.loadNextButton.pack(side = TOP, fill = X)

        self.loadAcornButton = Button(self.displayFrame, text = "Acorn", command = self.LoadAcorn)
        self.loadAcornButton.pack(side = TOP, fill = X)

        self.canMain = Canvas(self.root, bg = "white", height = 750, width = 1000)
        self.canMain.pack(side = RIGHT)
        self.scaling = 10.0

        #self.cellList.append(Cell(0,0))
        #self.cellList.append(Cell(1,0))
    	#self.cellList.append(Cell(2,0))
    	#self.cellList.append(Cell(4,0))
    	#self.cellList.append(Cell(5,0))
    	#self.cellList.append(Cell(4,1))
    	#self.cellList.append(Cell(5,1))
    	#self.cellList.append(Cell(5,2))

        #self.cellList.append(Cell(1,0))
        #self.cellList.append(Cell(1,1))
        #self.cellList.append(Cell(2,0))

        self.UpdateGui()
        
        self.root.after(50, self.KeyLoop)
        self.root.mainloop()

    def LoadNextPosition(self):
        self.AdvancePostion()

        self.LoadStart()
        #self.subProc.stdin.write(".\\data\\" + "".join(self.position) + ".cgol\n")
        #print ".\\data\\" + "".join(self.position) + ".cgol\n"
        #print "C:\Users\Yaso\Desktop\BLL\Cpp\Conway\Conway\data\\" + "".join(self.position) + ".cgol\n"

    def LoadStart(self):
        self.subProc.stdin.write("x")
        self.subProc.stdin.write(os.path.join(os.path.dirname(__file__), "Cpp\Conway\Conway\data\\start\\") + "".join(self.position) + ".cgol\n")

        self.GetCellList()

        self.UpdateGui()

    def LoadEnd(self):
        self.subProc.stdin.write("x")
        self.subProc.stdin.write(os.path.join(os.path.dirname(__file__), "Cpp\Conway\Conway\data\\end\\") + "".join(self.position) + ".cgol\n")

        self.GetCellList()

        self.UpdateGui()

    def AdvancePostion(self):
        if self.position == []:
            self.position.append('a')
        else:
            for i in range(0,len(self.position) + 1):
                invertedi = len(self.position) - i - 1
                if self.position[invertedi] != 'z':
                    self.position[invertedi] = chr(ord(self.position[invertedi]) + 1)
                    
                    for j in range(0,i):
                        self.position[len(self.position) - j - 1] = 'a'

                    break

                if len(self.position) == i:
                    self.position[-1] = 'a'
                    self.position.append('a')

    def FlipToken(self):
        self.token = not self.token

        if self.token:
            self.cellListToken = self.cellList
            self.subProcToken.stdin.write("s") #take my list
            self.subProcToken.stdin.write(str(len(self.cellList)) + "\n")

            for c in self.cellList:
                self.subProcToken.stdin.write(str(c.x) +  "\n")
                self.subProcToken.stdin.write(str(c.y) +  "\n")

        self.UpdateGui()
    

    def TurnAndUpdate(self, turns):
        if turns > 0:
            self.Turn(turns)

            self.GetCellList()

            self.UpdateGui()
        else:
            self.autoplay = not self.autoplay

        if self.autoplay:
            self.root.after(500, self.TurnAndUpdate, 1)


    def Turn(self, turns):
        self.subProc.stdin.write("t") #next turn
        self.subProc.stdin.write(str(turns) + "\n")
        
        if self.token:
            self.subProcToken.stdin.write("t") #next turn
            self.subProcToken.stdin.write(str(turns) + "\n")

        self.currentTurn += turns


    def UpdateGui(self):
        self.halfx = (float(self.canMain["width"]) / 2) + self.offsetx
        self.halfy = (float(self.canMain["height"]) / 2) + self.offsety

        self.currentTurnLabel.config(text = "Current Turn: %d" % (self.currentTurn))
        self.cellsAliveLabel.config(text = "Cells alive: %d" % (len(self.cellList)))
        self.mostCellsAliveLabel.config(text = "Most cells alive: %d" % (self.mostCellsAlive))
        self.currentlyLookingAtLabel.config(text = "".join(self.position) + ".cgol")

        self.canMain.delete("all")
        self.canMain.create_line(self.halfx + 200, self.halfy, self.halfx - 200, self.halfy)
        self.canMain.create_line(self.halfx, self.halfy + 200, self.halfx, self.halfy - 200)

        for c in self.cellList:
            self.canMain.create_oval(self.halfx + (c.x * self.scaling), self.halfy + (c.y * self.scaling), self.halfx + (c.x * self.scaling + self.scaling), self.halfy + (c.y * self.scaling + self.scaling), fill = "black")

        if self.token:
            for c in self.cellListToken:
                self.canMain.create_oval(self.halfx + (c.x * self.scaling), self.halfy + (c.y * self.scaling), self.halfx + (c.x * self.scaling + self.scaling), self.halfy + (c.y * self.scaling + self.scaling), outline = "red")

    def GetCellList(self):
        self.subProc.stdin.write("r") #send your list
        
        self.InterpretCellList("yup")

        if self.token:
            self.subProcToken.stdin.write("r") #send your list
        
            self.InterpretTokenCellList("yup")

    def InterpretCellList(self, inp):
        if inp == "c":
            self.cellList = []
            x = 0
            y = 0
            inp = self.subProc.stdout.readline().rstrip()
            while inp != "e":
                x, y = inp.split("/", 1)
                self.cellList.append(Cell(x, -int(y)))
                inp = self.subProc.stdout.readline().rstrip()

            if (self.mostCellsAlive < len(self.cellList)):
                self.mostCellsAlive = len(self.cellList)
            self.UpdateGui()
        else:
            self.root.after(20, self.InterpretCellList, self.subProc.stdout.readline().rstrip())

    def InterpretTokenCellList(self, inp):
        if inp == "c":
            self.cellListToken = []
            x = 0
            y = 0
            inp = self.subProcToken.stdout.readline().rstrip()
            while inp != "e":
                x, y = inp.split("/", 1)
                self.cellListToken.append(Cell(x, int(y)))
                inp = self.subProcToken.stdout.readline().rstrip()

            if (self.mostCellsAlive < len(self.cellList)):
                self.mostCellsAlive = len(self.cellList)
            self.UpdateGui()
        else:
            self.root.after(20, self.InterpretTokenCellList, self.subProcToken.stdout.readline().rstrip())


    def SetCellList(self):
        self.subProc.stdin.write("s") #take my list
        self.subProc.stdin.write(str(len(self.cellList)) + "\n")

        for c in self.cellList:
            self.subProc.stdin.write(str(c.x) +  "\n")
            self.subProc.stdin.write(str(c.y) +  "\n")

        self.subProcToken.stdin.write("s") #take my list
        self.subProcToken.stdin.write(str(len(self.cellList)) + "\n")

        for c in self.cellList:
            self.subProcToken.stdin.write(str(c.x) +  "\n")
            self.subProcToken.stdin.write(str(c.y) +  "\n")

    def SetCellListToken(self):
        if self.token:
            self.cellList = self.cellListToken
            self.SetCellList()
            
            self.UpdateGui()

    def KeyLoop(self):
        if self.keyDict['w']:
            self.offsety += self.scaling * 2
        if self.keyDict['s']:
            self.offsety -= self.scaling * 2

        if self.keyDict['a']:
            self.offsetx += self.scaling * 2
        if self.keyDict['d']:
            self.offsetx -= self.scaling * 2

        if self.keyDict['q']:
            self.scaling *= 1.05
            self.offsetx *= 1.05
            self.offsety *= 1.05
        if self.keyDict['e']:
            self.scaling *= 0.95
            self.offsetx *= 0.95
            self.offsety *= 0.95
        
        self.UpdateGui()
        self.root.after(50, self.KeyLoop)


    def Keys(self, event):
        if event.type == "2": #2 = KeyPressed; because a simple 2 as an integer was too difficult
            self.keyDict[event.char] = True
        elif event.type == "3": #3 = KeyReleased
            self.keyDict[event.char] = False
        
        if event.char == "r":
            self.scaling = 10
            self.offsetx = 0
            self.offsety = 0
        
       
    def LoadAcorn(self):
        self.cellList = []
        self.cellList.append(Cell(0,0))
        self.cellList.append(Cell(1,0))
        self.cellList.append(Cell(1,2))
        self.cellList.append(Cell(3,1))
        self.cellList.append(Cell(4,0))
        self.cellList.append(Cell(5,0))
        self.cellList.append(Cell(6,0))

        self.SetCellList()

        self.UpdateGui()
  
    def __del__(self):
        self.subProc.stdin.write("q") #kys
        self.subProcToken.stdin.write("q") #kys too

class Cell(object): #this should be a struct but oh well python
    def __init__ (self, x, y):
        self.x = int(x)
        self.y = int(y)

Main()
