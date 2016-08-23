#!/usr/bin/env python

import wx
import threading
import time as ostm
import tushare as ts
from datetime import *

class main_window(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title, size = (80, 150))

        self.initUI()

    def initUI(self):
        self.mainPanel = wx.Panel(self)
        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        self.mainPanel.SetSizer(self.mainSizer)

        sbs = self.initStaticBoxSizer(self.mainPanel, "", wx.HORIZONTAL, self.mainSizer)
        self.stime = self.initStaticText(self.mainPanel, 'time', sbs)

        self.sPrice = []
        for ss in slist:
            print ss
            sbs = self.initStaticBoxSizer(self.mainPanel, ss, wx.HORIZONTAL, self.mainSizer)
            st = self.initStaticText(self.mainPanel, 'static text', sbs)
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

    def updateUI(self, time, msg):
        self.stime.SetLabel(time)
        i = 0
        while(i < len(slist)):
            self.sPrice[i].SetLabel(msg[i])
            i = i + 1

class TushareThread(threading.Thread):
    def __init__(self, context):
        threading.Thread.__init__(self)
        self.context = context
    
    def run(self):
        print('running ...')
        while(True):
            now = datetime.now().strftime('%H:%M:%S')
            price = []
            for ss in slist:
                df = ts.get_realtime_quotes(ss)
                price.append(df['price'][0])
            wx.CallAfter(self.context.updateUI, now, price)
            ostm.sleep(3)

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
