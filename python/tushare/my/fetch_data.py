#!/usr/bin/env python

from datetime import datetime, timedelta
from mydb import *
import tushare as ts

class fetch_data(object):
    dbcols = [
            ['open', 'float'],
            ['high', 'float'],
            ['close', 'float'],
            ['low', 'float'],
            ['volume', 'float'],
            ['price_change', 'float'],
            ['p_change', 'float'],
            ['ma5', 'float'],
            ['ma10', 'float'],
            ['ma20', 'float'],
            ['v_ma5', 'float'],
            ['v_ma10', 'float'],
            ['v_ma20', 'float'],
            ['turnover', 'float']]

    def __init__(self, name):
        db = mydb(name)
        self.db = db

    def get_last_valid_date(self, period = 30):
        now = datetime.now()
        start = now - timedelta(days = period)
        start_date = start.strftime('%Y-%m-%d')

        data = ts.get_hist_data('sh', start_date)
        try:
            return data.index[0]
        except Exception as e:
            # print(e)
            return None

    def get_now_date(self):
        now = datetime.now()
        return now.strftime('%Y-%m-%d')

    def get_pass_date(self, days_pass):
        now = datetime.now()
        start = now - timedelta(days = days_pass)
        return start.strftime('%Y-%m-%d')

    def get_last_update_date(self, code):
        try:
            data = self.db.get_last_record(code, 'date', 'desc')
            return data[self.db.column_index(code, 'date')]
        except Exception as e:
            print(e)
            return None

    def time_delta(self, prev_time):
        now = datetime.now()
        delta = now - prev_time;
        return delta.total_seconds()

    def sync(self, start = '2016-07-01'):
        today = self.get_now_date()
        last_valid_date = self.get_last_valid_date()

        allstocks = ts.get_stock_basics()
        allnum = len(allstocks)

        subsql = ", date varchar(32) unique"

        print("now:%s, last valid:%s, num:%d", today, last_valid_date, allnum)

        update_process = 0
        for code in allstocks.index:
            print("%s:" % code)
            self.db.check_table_and_creat(code, self.dbcols, subsql)
            last_update_date = self.get_last_update_date(code)
            if(last_update_date != None):
                if(last_update_date == last_valid_date):
                    print("%s up-to-date" % code)
                    continue
                else:
                    start = last_update_date

            time_tag = datetime.now()
            df = ts.get_hist_data(code, start)
            print("get_hist_data %s ... %f" % (start, self.time_delta(time_tag)))

            time_tag = datetime.now()
            for i in range(len(df))[::-1]:
                d = df.iloc[i]
                sql = "insert into '%s' values (NULL, '%s', %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f);" % \
                        (code, df.index[i], d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7], d[8], d[9], d[10], d[11], d[12], d[13])

                self.db.execute(sql)

            self.db.commit()
            print("insert ... %f" % self.time_delta(time_tag))
            print("%d/%d\n" % (update_process, allnum))
            update_process = update_process + 1

