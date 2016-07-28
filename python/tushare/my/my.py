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
    if check_record(cursor, table, "code", code):
        sql = "update %s set %s = '%s' where code = '%s';" % (table, column, value, code)
    else:
        sql = "insert into %s (code, %s) values ('%s', '%s');" % (table, column, code, value)
    cursor.execute(sql)

def get_last_update(cursor, code):
    sql = "select * from stocks where code = '%s'" % code

def get_lowest(code):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')

    date_last_update = get_last_update(code)

    one_month_ago = now - timedelta(days = 30)
    date_1m_ago = one_month_ago.strftime('%Y-%m-%d')

    data = ts.get_hist_data(code, date_1m_ago, date_now)
    close = data['close']

    lowest = 9999;
    try:
        print("data number: %d" % len(close))
        lowest = close[0]
        for p in close:
            if lowest > p:
                lowest = p
    except Exception as e:
        print(e)

    return lowest

def update_stock_lowest(cursor, code):
    lowest = get_lowest(code)
    update_record(cursor, "stocks", code, "lowest", lowest)
    return lowest

conn = sqlite3.connect('my.db')
cursor = conn.cursor()
if not check_column(cursor, "stocks", "lowest"):
    insert_column(cursor, "stocks", "lowest", "varchar", 16)
if not check_column(cursor, "stocks", "last_update"):
    insert_column(cursor, "stocks", "last_update", "varchar", 32)
"""
allstocks = ts.get_stock_basics()

i = 0
for stock in allstocks.index:
    lowest = update_stock_lowest(cursor, stock)
    print("%d: %s - %f" % (i, stock, lowest))
    conn.commit()
    i = i + 1
"""
cursor.close()
conn.close()
