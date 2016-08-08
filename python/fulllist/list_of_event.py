#!/usr/bin/env python

import wx

for x in dir(wx):
    if x.startswith("EVT_"):
        print x
