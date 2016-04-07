#!/usr/bin/env python

import hashlib  
import os  
import wx  
class NotBookMainFrame(wx.Frame):  
    def __init__(self,parent,title):  
        wx.Frame.__init__(self,parent,title=title)  

        self.tb = self.CreateToolBar()  
        self.control = wx.TextCtrl(self,style=wx.TE_MULTILINE|wx.TE_LINEWRAP,size=(300,500))  

        self.sbar = self.CreateStatusBar()  
        menuB  = self.createMenuBar()  
        self.SetMenuBar(menuB)  

        acceltbl = wx.AcceleratorTable([(wx.ACCEL_CTRL, ord('Q'), self.selectA.GetId())])  
        self.SetAcceleratorTable(acceltbl)   

        self.Show(True)  

    def createMenuBar(self):  
        fileM = wx.Menu()  
        newF = fileM.Append(wx.NewId(),"new(N)\tCtrl+N","openaalearyexistdefile.")  
        fmitem = fileM.Append(wx.NewId(),"open(O)\tCtrl+O","openaalearyexistdefile.")  
        save = fileM.Append(wx.NewId(),  "save(S)\tCtrl+S","savecurrentdefile.")  
        toSave = fileM.Append(wx.NewId(),"othersaveas(A)..","savecurrentdefile.")  
        fileM.AppendSeparator()  
        fileM.Append(wx.NewId(),"pageset(U)..","setpagestyle.")  
        printL = fileM.Append(wx.NewId(),"print(P)\tCtrl+P","printpage.")  
        fileM.AppendSeparator()  
        qit = fileM.Append(wx.NewId(),"exit(X)","closeallopendefile.")  

        self.Bind(wx.EVT_MENU, self.OnNewFile, newF)  
        self.Bind(wx.EVT_MENU, self.OnOpen, fmitem)  
        self.Bind(wx.EVT_MENU, self.OnSave, save)  
        self.Bind(wx.EVT_MENU, self.OnQuit, qit)  

        editM = wx.Menu()  
        editM.Append(wx.NewId(),"cancel(U)\tCtrl+Z","cancellastdedo.")  
        editM.AppendSeparator()  
        editM.Append(wx.NewId(),"cut(U)\tCtrl+X","cut.")  
        editM.Append(wx.NewId(),"copy(T)\tCtrl+C","copy.")  
        editM.Append(wx.NewId(),"paste(C)\tCtrl+V","paste.")  
        editM.Append(wx.NewId(),"del(P)\tDel","del.")  
        editM.AppendSeparator()  
        editM.Append(wx.NewId(),"find(F)\tCtrl+F","find.")  
        editM.Append(wx.NewId(),"findaa(N)\tF3","findaa.")  
        editM.Append(wx.NewId(),"del(P)\tDel","del.")  
        editM.Append(wx.NewId(),"replace(R)\tCtrl+H","replace.")  
        editM.Append(wx.NewId(),"jump(G)\tCtrl+G","jump.")  
        editM.AppendSeparator()  
        self.selectA = editM.Append(wx.NewId(),"allselect(A)\tCtrl+A","allselect.")  

        self.Bind(wx.EVT_MENU, self.OnSelectAll, self.selectA)  

        editM.Append(wx.NewId(),"date/date(D)\tF5","date.")  
        posM = wx.Menu()  
        self.autoCutoverLine = self.newline = posM.Append(wx.NewId(),"autocr(W)","autocr.",kind=wx.ITEM_CHECK)  

        self.Bind(wx.EVT_MENU, self.autoCutoverL, self.autoCutoverLine)  

        self.showToolStatus = 0;  
        posM.Append(wx.NewId(),"font(F)..","setfont.")  
        #cat[V]  
        viewM = wx.Menu()  
        viewM.Append(wx.NewId(),"status","status.")  
        self.tool=viewM.Append(wx.NewId(),"toolbar","toolbar",kind=wx.ITEM_CHECK)  
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.tool)  
        #help[H]  
        helpM = wx.Menu()  
        helpM.Append(wx.NewId(),"cathelp(H)","cathelp.")  
        about = helpM.Append(wx.NewId(),"aboutnotepad(A)","aboutnotepad.")  
        self.Bind(wx.EVT_MENU, self.OnAbout, about)  
        #create a menuBar  
        menuB = wx.MenuBar()  
        # append a menu  
        menuB.Append(fileM,"file(F)")  
        menuB.Append(editM,"edit(E)")  
        menuB.Append(posM,"style[O]")  
        menuB.Append(viewM,"cat[V]")  
        menuB.Append(helpM,"help[H]")  
        return menuB  

    def autoCutoverL(self,event):  
        print "hell"  
        self.control.SetStyle(-1,-1,wx.TextAttr("wx.TE_WORDWRAP"))  

    def OnSelectAll(self,event):  
        self.control.SelectAll()  

    def OnKeyDown(self,event):  
        key = event.GetKeyCode();  
        if key == ord('f'):  
            self.fileM.Show()  
        else:  
            self.control.AppendText(chr(key))  

    def ToggleToolBar(self,event):  
        self.showToolStatus+=1;  
        if self.showToolStatus % 2 == 1:  
            print 1111  
            self.control.SetInsertionPoint(50)  
            self.tb.Show()   
        else:  
            print 2222  
            self.tb.Hide()  
    def OnNewFile(self,event):  
        if self.control.IsEmpty() <> True:  
            dlg = wx.MessageDialog(self, "aswillchangesavetononetitle","notepad",wx.YES_NO | wx.ICON_QUESTION | wx.CANCEL)  
            retCode = dlg.ShowModal()  
        if retCode == wx.ID_YES:  
            # save  
            self.OnSave(event)  

            self.control.SetValue("")  
        elif retCode == wx.ID_NO:  

            self.control.SetValue("")  
        else:  

            dlg.Close();  

        dlg.Destroy()  

    def OnSave(self,event):  

        if self.control.IsEmpty():  
            return;  
        self.dirname=''  
        dlg = wx.FileDialog(self,"choose a file",self.dirname,"","*.*",wx.FD_SAVE)  

        if dlg.ShowModal() == wx.ID_OK:  
            self.filename = dlg.GetFilename()  
            self.dirname = dlg.GetDirectory()  
            f = open(os.path.join(self.dirname, self.filename), 'w')  
            f.write(self.control.GetValue());  
            f.close()  
        dlg.Destroy()  

        self.Title  = self.filename + " - notepad"  


    def OnOpen(self,event):  
        print self.control.GetValue()  
        self.dirname=''  
        self.dirname=''  
        dlg = wx.FileDialog(self,"choose a file",self.dirname,"","*.*",wx.OPEN)  
        if dlg.ShowModal() == wx.ID_OK:  
            self.filename = dlg.GetFilename()  
            self.dirname = dlg.GetDirectory()  
            f = open(os.path.join(self.dirname, self.filename), 'r')  
            self.control.SetValue(f.read())  
            f.close()  
        dlg.Destroy()  
        self.control.SetFocus()  
        wx.StaticText(self.sbar, label=self.filename + ","+ str(self.control.GetNumberOfLines()) + " line",pos=(0,1))  

    def OnQuit(self,event):  
        self.Close()  

    def OnAbout(self,event):  
        dlg = wx.MessageDialog(self, "hello,baby","title is a baby",wx.OK)  
        dlg.ShowModal()  
        self.control.SelectAll();  
        dlg.Destroy()  

    def OnHello(self, event):  
        pass       

    def createButtonBar(self, panel, yPos = 10):  
        xPos = 0  
        for eachLabel, eachHandler in self.buttonData():  
            pos = (xPos, yPos)  
            button = self.buildOneButton(panel, eachLabel,eachHandler, pos)  
            xPos += button.GetSize().width  

    def buildOneButton(self, parent, label, handler, pos=(0,0)):  
        button = wx.Button(parent, -1, label, pos)  
        self.Bind(wx.EVT_BUTTON, handler, button)  
        return button  

    @staticmethod  
    def GetMd5(content):   
        return hashlib.new("md5", content).hexdigest()   

if __name__=="__main__":  
    app = wx.App(False)  
    frame = NotBookMainFrame(None,"nonetitle - notepad")  
    app.MainLoop()  
