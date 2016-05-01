#!/usr/bin/env python

import wx

for x in dir(wx):
    if x.startswith("ID_"):
        print x
