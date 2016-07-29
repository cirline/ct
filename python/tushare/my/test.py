#!/usr/bin/env python

from mydb import *

md = mydb('my.db')

print(md.column_index('stocks', 'last_update'))
