#!/usr/bin/env python

import wx
import threading
import time as ostm
import tushare as ts
from datetime import *
import string

class main_window(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title, size = (80, 100))

        self.initUI()

    def initUI(self):
        self.statusBar = self.CreateStatusBar()

        self.mainPanel = wx.Panel(self)
        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        self.mainPanel.SetSizer(self.mainSizer)

        sbs = self.initStaticBoxSizer(self.mainPanel, "", wx.VERTICAL, self.mainSizer)

        self.sPrice = []
        for ss in slist:
            print ss
            st = self.initStaticText(self.mainPanel, '0', sbs)
            self.sPrice.append(st)

    def initStaticBoxSizer(self, parent, label, ori, layout):
        sb = wx.StaticBox(parent, label = label)
        sbs = wx.StaticBoxSizer(sb, ori)
        layout.Add(sbs, 0, wx.EXPAND)
        return sbs

    def initStaticText(self, parent, txt, layout):
        st = wx.StaticText(parent, label = txt)
        layout.Add(st)
        return st

    def updateUI(self, time, msg, update):
        self.statusBar.SetStatusText(time)
        if(update):
            i = 0
            while(i < len(slist)):
                pp = self.sPrice[i].GetLabel()
                self.sPrice[i].SetLabel(msg[i])
                ppf = string.atof(pp)
                nnf = string.atof(msg[i])
                if(nnf > ppf):
                    self.sPrice[i].SetForegroundColour('red')
                elif(nnf < ppf):
                    self.sPrice[i].SetForegroundColour('blue')
                else:
                    self.sPrice[i].SetForegroundColour('blank')
                i = i + 1

class TushareThread(threading.Thread):
    def __init__(self, context):
        threading.Thread.__init__(self)
        self.context = context
    
    def run(self):
        print('running ...')
        i = 0
        update = False
        price = []
        while(True):
            now = datetime.now().strftime('%H:%M:%S')
            if(i == 0):
                price = []
                for ss in slist:
                    df = ts.get_realtime_quotes(ss)
                    price.append(df['price'][0])
                update = True
            wx.CallAfter(self.context.updateUI, now, price, update)
            ostm.sleep(1)
            if(i < 4):
                i = i + 1
            else:
                i = 0
            update = False

slist = []
slist.append('601398')
slist.append('601766')
slist.append('601668')

app = wx.App(False)
mframe = main_window(None, "main")
mframe.Centre()
mframe.Show()
point = mframe.GetPosition()
mframe.MoveXY(point[0] * 2, point[1])

t1 = TushareThread(mframe)
t1.start()

app.MainLoop()
