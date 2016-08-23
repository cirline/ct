#!/usr/bin/env python

from datetime import datetime, timedelta
import tushare as ts
import sqlite3
from mydb import *

"""
* record_up_to_date - check the record is up-to-date
*
* the return value is an unicode of the last_update or None,
* or 'up-to-date' while record up-to-date
"""
def record_last_update(db, code):
    cell = 'last_update'

    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')
    date_last_update = db.get_onecell('stocks', 'code', code, cell)
    if(date_last_update == None):
        return None
    elif(date_last_update == now):
        return 'up-to-date'
    else:
        return date_last_update

def record_update_last_update(db, code):
    cell = 'last_update'

    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')
    db.update_onecell('stocks', 'code', code, cell, date_now)

def get_now_date():
    now = datetime.now()
    return now.strftime('%Y-%m-%d')

def pull_net_data(code, last_update, default_day):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')

    if(last_update == None):
        temp = now - timedelta(days = default_day)
        last_update = temp.strftime('%Y-%m-%d')

    print('%s ~ %s' % (last_update, date_now))
    return ts.get_hist_data(code, last_update, date_now)

def get_lowest(db, data, code, item, last_update, defval):
    if(last_update != None):
        lowest = db.get_onecell('stocks', 'code', code, item + '_lowest')
        if(lowest != None):
            defval = lowest

    try:
        item_data = data[item]
        print("item data len: %d" % (len(item_data)))
        for p in item_data:
            if(isinstance(p, float) and defval > p):
                defval = p
    except Exception as e:
        print(e)

    return defval

def record_update_lowest(db, data, code, item, last_update):
    lowest = get_lowest(db, data, code, item, last_update, 9999)
    if(lowest > 0 and lowest < 9999):
        db.update_onecell('stocks', 'code', code, item + '_lowest', lowest)
    return lowest

def record_update_newest(db, code, item):
    pass

def update_record(db, code):
    last_update = record_last_update(db, code)
    if(last_update != 'up-to-date'):
        print("%s pulling ..." % code)
        data = pull_net_data(code, last_update, 7)
        print('%s update ma5 lowest ...' % code)
        record_update_lowest(db, data, code, 'ma5', last_update)
        record_update_last_update(db, code)
    else:
        print("%s up-to-date" % code)

db = mydb('my.db')
dbcolumns = []
dbcolumns.append(['code', 'varchar(32)'])
dbcolumns.append(['ma5_lowest', 'float'])
dbcolumns.append(['close', 'float'])
dbcolumns.append(['last_update', 'varchar(32)'])
db.check_db('stocks', dbcolumns)

allstocks = ts.get_stock_basics()

for code in allstocks.index:
    print("%s:" % code)
    update_record(db, code)
    db.commit()
    print("\n")

db.close()

