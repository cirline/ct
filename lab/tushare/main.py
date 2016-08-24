#!/usr/bin/env python

import wx
import threading
import time as ostm
import tushare as ts
from datetime import *
import string

class main_window(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title, size = (85, 80))

        ws = self.GetWindowStyle()
        self.SetWindowStyle(ws | wx.STAY_ON_TOP)

        self.initUI()

    def initUI(self):
        # self.statusBar = self.CreateStatusBar()

        self.mainPanel = wx.Panel(self)
        self.mainSizer = wx.BoxSizer(wx.VERTICAL)
        self.mainPanel.SetSizer(self.mainSizer)

        sbs = self.initStaticBoxSizer(self.mainPanel, "", wx.VERTICAL, self.mainSizer)

        self.sPrice = []
        for ss in slist:
            print ss
            st = self.initStaticText(self.mainPanel, '0', sbs)
            self.sPrice.append(st)

        self.statusBar = self.initStaticText(self.mainPanel, '0', sbs)

    def initStaticBoxSizer(self, parent, label, ori, layout):
        sb = wx.StaticBox(parent, label = label)
        sbs = wx.StaticBoxSizer(sb, ori)
        layout.Add(sbs, 0, wx.EXPAND)
        return sbs

    def initStaticText(self, parent, txt, layout):
        st = wx.StaticText(parent, label = txt)
        layout.Add(st)
        return st

    def updateUI(self, status, df_list, update):
        #self.statusBar.SetStatusText(time)
        self.statusBar.SetLabel(status)
        if(update):
            i = 0
            while(i < len(slist)):
                df = df_list[i]
                price = df['price'][0]
                nnf = string.atof(price)
                pre_close = df['pre_close'][0]
                ccf = string.atof(pre_close)
                crange = (nnf - ccf) / ccf * 100
                if(crange > 0):
                    self.sPrice[i].SetForegroundColour('red')
                elif(crange < 0):
                    self.sPrice[i].SetForegroundColour('blue')
                    crange = -1 * crange
                else:
                    self.sPrice[i].SetForegroundColour('blank')

                self.sPrice[i].SetLabel("%s %.2f" % (price, crange))
                i = i + 1

class TushareThread(threading.Thread):
    def __init__(self, context):
        threading.Thread.__init__(self)
        self.context = context
    
    def run(self):
        print('running ...')
        i = 0
        update = False
        df_list = []
        while(True):
            now = datetime.now().strftime('%H:%M:%S')
            if(i == 0):
                df_list = []
                for ss in slist:
                    df = ts.get_realtime_quotes(ss)
                    df_list.append(df)
                update = True
            if(i == 0):
                now = '.'
            elif(i == 1):
                now = '..'
            elif(i == 2):
                now = '...'
            elif(i == 3):
                now = '....'
            elif(i == 4):
                now = '.....'
            wx.CallAfter(self.context.updateUI, now, df_list, update)
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
