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

def get_lowest(code):
    now = datetime.now()
    date_now = now.strftime('%Y-%m-%d')

    one_month_ago = now - timedelta(days = 30)
    date_1m_ago = one_month_ago.strftime('%Y-%m-%d')

    data = ts.get_hist_data(code, date_1m_ago, date_now)
    close = data['close']

    print(close)

    lowest = close[0]
    for p in close:
        if lowest > p:
            lowest = p
    return lowest

try:
    conn = sqlite3.connect('my.db')
    cursor = conn.cursor()
    if not check_column(cursor, "stocks", "lowest"):
        insert_column(cursor, "stocks", "lowest", "varchar", 16)

    print(get_lowest('601398'))

except Exception as e:
    print(e)
check_record(cursor, "stocks", "code", "601398")
check_record(cursor, "stocks", "code", 601398)

