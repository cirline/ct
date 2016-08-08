#!/usr/bin/env python

from mydb import *
from fetch_data import *

md = mydb('my.db')

print(md.column_index('stocks', 'last_update'))

# print(md.get_onecell('stocks', 'code', '601398', 'lowest'))

fd = fetch_data('hist_data.db')
fd.sync()

