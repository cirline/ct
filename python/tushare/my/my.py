#!/usr/bin/env python

from datetime import datetime, timedelta
import tushare as ts
import sqlite3
from mydb import *

def update_last_update(db, code):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')
    db.update_onecell('stocks', 'code', code, 'last_update', date_now)

def get_lowest(db, code, item):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')
    lowest = 9999

    date_last_update = db.get_onecell('stocks', 'code', code, 'last_update')
    if(date_last_update == None):
        one_month_ago = now - timedelta(days = 5)
        date_start = one_month_ago.strftime('%Y-%m-%d')
    else:
        date_start = date_last_update
        lowest = db.get_onecell('stocks', 'code', code, item + '_lowest')
        if(lowest == None):
            lowest = 9999

    # print("now: %s, start: %s, lowest = %s" % (date_now, date_start, lowest))
    if(date_now == date_start):
        return 0

    try:
        data = ts.get_hist_data(code, date_start, date_now)
        item_data = data[item]
        for p in item_data:
            if lowest > p:
                lowest = p
    except Exception as e:
        print(e)

    return lowest

def update_lowest(db, code, item):
    lowest = get_lowest(db, code, item)
    if(lowest > 0 and lowest < 9999):
        db.update_onecell('stocks', 'code', code, item + '_lowest', lowest)
    return lowest

db = mydb('my.db')
dbcolumns = []
dbcolumns.append(['code', 32])
dbcolumns.append(['ma5_lowest', 32])
dbcolumns.append(['last_update', 32])
db.check_db('stocks', dbcolumns)

allstocks = ts.get_stock_basics()

i = 0
for code in allstocks.index:
    lowest = update_lowest(db, code, 'ma5')
    update_last_update(db, code)
    print("%d: %s - %f\n" % (i, code, lowest))
    db.commit()
    i = i + 1

db.close()

