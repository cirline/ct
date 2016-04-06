#!/usr/bin/env python

import wx
import os

class main_window(wx.Frame):
    """ overide __init__ """
    def __init__(self, parent, title):
        wx.Frame.__init__(self, parent, title = title, size = (400, 200))

        """ text box """
        self.control = wx.TextCtrl(self, style = wx.TE_MULTILINE)

        """ status bar """
        self.CreateStatusBar()

        """ sizer """
        self.sizer = wx.BoxSizer(wx.HORIZONTAL)
        self.buttons = []
        for i in range(0, 6):
            self.buttons.append(wx.Button(self, -1, "Button" + str(i)))
            self.sizer.Add(self.buttons[i], 1, wx.EXPAND)
        self.sizer2 = wx.BoxSizer(wx.VERTICAL)
        self.sizer2.Add(self.control, 1, wx.EXPAND)
        self.sizer2.Add(self.sizer, 0, wx.EXPAND)
        self.SetSizer(self.sizer2)
        self.SetAutoLayout(1)
        self.sizer.Fit(self)

        """ menu bar """
        menu = wx.Menu()
        menu_open = menu.Append(wx.ID_OPEN, "Open", "open file")
        menu.AppendSeparator()
        menu_about = menu.Append(wx.ID_ABOUT, "&About", " about this program")
        menu_exit = menu.Append(wx.ID_EXIT, "E&xit", "exit program")
        menu_bar = wx.MenuBar()
        menu_bar.Append(menu, "&File")
        self.SetMenuBar(menu_bar)

        """ action """
        self.Bind(wx.EVT_MENU, self.on_open, menu_open)
        self.Bind(wx.EVT_MENU, self.on_about, menu_about)
        self.Bind(wx.EVT_MENU, self.on_exit, menu_exit)

    """ open file dialog """
    def on_open(self, event):
        self.dirname = ""
        dlg = wx.FileDialog(self, "file dialog", self.dirname, "", "*.*", wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), "r")
            self.control.SetValue(f.read())
            print f.read()
            f.close()
        dlg.Destroy()

    def on_about(self, event):
        dlg = wx.MessageDialog(self, "dialog content", "dialog title", wx.OK)
        dlg.ShowModal()
        dlg.Destroy()

    def on_exit(self, event):
        self.Close(True)

app = wx.App(False)
#frame = wx.Frame(None, wx.ID_ANY, "hello world")
#frame.Show(True)
frame = main_window(None, "main window title")
frame.Show(True)

app.MainLoop()

