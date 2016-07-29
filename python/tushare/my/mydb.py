#!/usr/bin/env python

import sqlite3

class mydb(object):
    def __init__(self, name):
        try:
            self.conn = sqlite3.connect(name)
            self.cursor = self.conn.cursor()
        except Exception as e:
            print(e)
            if(self.conn != None):
                self.conn.close()
            self.cursor = None
            self.conn = None

    def check_column(self, table, column):
        if(self.cursor == None):
            return False
        sql = "select * from sqlite_master where name = '%s' and sql glob '*, %s *';" % (table, column)
        self.cursor.execute(sql)
        count = self.cursor.fetchall()
        if(count != None):
            return (len(count) > 0)
        else:
            return False

    def insert_column(self, table, column, dtype, bits):
        if(self.cursor == None):
            return False
        sql = "alter table %s add column %s %s(%d);" % (table, column, dtype, bits)
        self.cursor.execute(sql)
        return True

    def column_index(self, table, column):
        if(self.cursor == None):
            return -1
        sql = "select * from sqlite_master where name = '%s';" % (table)
        self.cursor.execute(sql)
        result = self.cursor.fetchone()
        if(result != None):
            table_sql = result[4]
            print("create table sql: %s" % table_sql)
            index = table_sql.count(',', 0, table_sql.find(', %s' % column)) + 1
            print(index)
        else:
            return -1


    def get_onecell(self, table, key, keyval, column):
        pass

    def check_db(self):
        if not check_column("stocks", "last_update"):
            insert_column("stocks", "last_update", "varchar", 32)
        if not check_column("stocks", "lowest"):
            insert_column("stocks", "lowest", "varchar", 32)
        if not check_column("stocks", "ma5_lowest"):
            insert_column("stocks", "ma5_lowest", "varchar", 32)

