#!/usr/bin/env python

import wx

class KeyEvent(wx.Frame):
    def __init__(self, parent, id, title):
        wx.Frame.__init__(self, parent, id, title)
        panel = wx.Panel(self, -1)
        panel.Bind(wx.EVT_CHAR, self.OnKeyDown)
        self.Show(True)

    def OnKeyDown(self, event):
        print "event", event
        """
        if event.GetKeyCode() == ord('f'):
            if self.GetPosition()==((0,0)):
                self.SetPosition((300,300))
                self.SetSize((500,300))
            else:
                self.SetPosition((0,0))
                self.SetSize(wx.DisplaySize())
        if event.GetKeyCode() == ord('q'):
            self.Close()
        else:
            event.Skip()
        """

app = wx.App()
KeyEvent(None, -1, 'k.py')
app.MainLoop()
