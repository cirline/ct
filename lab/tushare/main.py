#!/usr/bin/env python

import wx
import threading
import time as ostm
import tushare as ts
from datetime import *
import string
from cfg_editor import *

class main_window(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title, size = (110, 85))

        ws = self.GetWindowStyle()
        self.SetWindowStyle(ws | wx.STAY_ON_TOP)

        self.actions = ui_action(self)
        self.initUI()

    def initUI(self):
        self.initMenuBar()
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

    def add_menu_item(self, menu, itemid, text, desc, func):
        item = menu.Append(itemid, text, desc)
        if(func != None):
            self.Bind(wx.EVT_MENU, func, item)

    def initMenuBar(self):
        menubar = wx.MenuBar()
        self.SetMenuBar(menubar)

        menu = wx.Menu()
        self.add_menu_item(menu, wx.NewId(), 'config', 'config', self.actions.opencfg)
        self.add_menu_item(menu, wx.ID_CLOSE, 'close\tctrl+x', 'close', self.actions.close)
        menubar.Append(menu, 'Menu')

    def initStaticBoxSizer(self, parent, label, ori, layout):
        sb = wx.StaticBox(parent, label = label)
        sbs = wx.StaticBoxSizer(sb, ori)
        layout.Add(sbs, 0, wx.EXPAND)
        return sbs

    def initStaticText(self, parent, txt, layout):
        st = wx.StaticText(parent, label = txt, style = wx.ALIGN_RIGHT)
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
                    self.sPrice[i].SetForegroundColour('black')

                if(nnf > 1000):
                    nnf = nnf % 1000

                s = "%6.2f %5.2f" % (nnf, crange)
                print(s)

                self.sPrice[i].SetLabel(s)
                i = i + 1

class config_window(wx.Frame):
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title)

class ui_action:
    def __init__(self, parent):
        self.parent = parent

    def opencfg(self, event):
        print('open config panel')
        oc_frame = config_window(self.parent, 'config')
        oc_frame.Show()

    def close(self, event):
        print('bye')
        self.parent.Close()

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
            #now = datetime.now().strftime('%H:%M:%S')
            now = datetime.now()
            pull = (now > active_time[0] and now < active_time[1]) or \
                    (now > active_time[2] and now < active_time[3])
            pull = True
            if(i == 0 and pull):
                try:
                    list_temp = []
                    for ss in slist:
                        df = ts.get_realtime_quotes(ss)
                        list_temp.append(df)
                    df_list = list_temp
                    update = True
                    print('get_realtime_quotes update')
                except Exception as e:
                    print('get_realtime_quotes: %s' % e)
            s = '.'
            for x in range(i):
                s = s + '.'
            wx.CallAfter(self.context.updateUI, s, df_list, update)
            ostm.sleep(1)
            if(i < 4):
                i = i + 1
            else:
                i = 0
            update = False

slist = []
cfg = cfg_editor('cfg.conf')
for section in cfg.sections():
    slist.append(section)

now = datetime.now()
active_time = []
active_time.append(now.replace(hour=9, minute=30, second=0))
active_time.append(now.replace(hour=11, minute=30, second=0))
active_time.append(now.replace(hour=13, minute=00, second=0))
active_time.append(now.replace(hour=15, minute=00, second=0))

app = wx.App(False)
mframe = main_window(None, "oooo")
mframe.Centre()
mframe.Show()
point = mframe.GetPosition()
mframe.MoveXY(point[0] * 2, point[1])

t1 = TushareThread(mframe)
t1.start()

app.MainLoop()
