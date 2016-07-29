#!/usr/bin/env python

from datetime import datetime, timedelta
import tushare as ts
import sqlite3

def check_column(cursor, table, column):
    sql = "select * from sqlite_master where name = '%s' and sql glob '*, %s *';" % (table, column)
    #sql = "select * from sqlite_master;"
    cursor.execute(sql)
    count = cursor.fetchall()
    return (len(count) > 0)

def insert_column(cursor, table, column, dtype, bits):
    sql = "alter table %s add column %s %s(%d);" % (table, column, dtype, bits)
    cursor.execute(sql)

def check_record(cursor, table, column, value):
    if isinstance(value, str):
        sql = "select %s from %s where %s = '%s';" % (column, table, column, value)
    elif isinstance(value, int):
        sql = "select %s from %s where %s = %d;" % (column, table, column, value)
    cursor.execute(sql)
    count = cursor.fetchall()
    return (len(count) > 0)

def update_record(cursor, table, code, column, value):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')
    if check_record(cursor, table, "code", code):
        sql = "update %s set %s = '%s', last_update = '%s' where code = '%s';" % (table, column, value, date_now, code)
    else:
        sql = "insert into %s (code, %s, last_update) values ('%s', '%s', '%s');" % (table, column, code, value, date_now)
    cursor.execute(sql)

def get_last_update(cursor, code):
    sql = "select * from stocks where code = '%s'" % code
    print("get_last_update sql: %s" % sql)
    cursor.execute(sql)
    count = cursor.fetchone()
    """ last_update is null ? """
    if(count == None):
        return None
    else:
        if(count[3] == None):
            print("last update none")
        else:
            print("last update 2 = %s" % count[3])
        return count[3]

def get_lowest(cursor, code):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')

    date_last_update = get_last_update(cursor, code)
    if(date_last_update == None):
        print('get 1 mount ago')
        one_month_ago = now - timedelta(days = 5)
        date_start = one_month_ago.strftime('%Y-%m-%d')
    else:
        date_start = date_last_update

    print("date now is %s, last update is %s" % (date_now, date_start))
    if(date_now == date_start):
        return 0

    lowest = 9999;
    try:
        data = ts.get_hist_data(code, date_start, date_now)
        close = data['close']
        print("close data len: %d" % len(close))
        lowest = close[0]
        for p in close:
            if lowest > p:
                lowest = p
    except Exception as e:
        print(e)

    return lowest

def update_stock_lowest(cursor, code):
    lowest = get_lowest(cursor, code)
    if(lowest > 0):
        update_record(cursor, "stocks", code, "lowest", lowest)
    return lowest

conn = sqlite3.connect('my.db')
cursor = conn.cursor()
if not check_column(cursor, "stocks", "lowest"):
    insert_column(cursor, "stocks", "lowest", "varchar", 16)
if not check_column(cursor, "stocks", "last_update"):
    insert_column(cursor, "stocks", "last_update", "varchar", 32)

allstocks = ts.get_stock_basics()

i = 0
for stock in allstocks.index:
    lowest = update_stock_lowest(cursor, stock)
    print("%d: %s - %f\n" % (i, stock, lowest))
    conn.commit()
    i = i + 1

cursor.close()
conn.close()

