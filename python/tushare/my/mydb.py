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

    def execute(self, sql):
        if(self.cursor == None):
            return None
        return self.cursor.execute(sql)

    def fetchone(self):
        if(self.cursor == None):
            return None
        return self.cursor.fetchone()

    def fetchall(self):
        if(self.cursor == None):
            return None
        return self.cursor.fetchall()

    def commit(self):
        if(self.conn == None):
            return None
        self.conn.commit()

    def close(self):
        if(self.conn != None):
            self.conn.close()
            self.conn = None
        if(self.cursor != None):
            self.cursor.close()
            self.cursor = None

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

    def check_table(self, table):
        if(self.get_onecell('sqlite_master', 'name', table, '*') == None):
            return False
        else:
            return True

    """
    * insert_column - insert a column into a table
    * @table: table name
    * @column: column will be insert
    * @dtype: column date type
    *
    * the return is a boolean value
    """
    def insert_column(self, table, column, dtype):
        if(self.cursor == None):
            return False
        sql = "alter table '%s' add column %s %s;" % (table, column, dtype)
        self.cursor.execute(sql)
        return True

    """
    * column_index - map column label to index by sqlite_master
    * @table: table name
    * @column: column label
    *
    * the return value is a integer for column index, or -1 while column not exist
    """
    def column_index(self, table, column):
        if(self.cursor == None):
            return -1
        sql = "select * from sqlite_master where name = '%s';" % (table)
        self.cursor.execute(sql)
        result = self.cursor.fetchone()
        if(result != None):
            table_sql = result[4]
            index = table_sql.count(',', 0, table_sql.find(', %s' % column)) + 1
            return index
        else:
            return -1

    """
    * get_last_record - get last record
    * @table: table name
    * @order: record order key
    * @desc: is order by desc ?
    """
    def get_last_record(self, table, order = 'id', desc = ''):
        if(self.cursor == None):
            return None
        sql = "select * from '%s' order by %s %s;" % (table, order, desc)
        self.execute(sql)
        return self.fetchone()

    """
    * get_onecell - get one cell value
    * @table: table name
    * @key: record key
    * @key: record key value
    * @column: the cell to be read
    """
    def get_onecell(self, table, key, keyval, column):
        if(self.cursor == None):
            return None
        sql = "select %s from %s where %s = '%s';" % (column, table, key, keyval)
        # print("getonecell sql: %s" % sql)
        self.cursor.execute(sql)
        result = self.cursor.fetchone()
        if(result == None):
            return None
        else:
            return result[0]

    def update_onecell(self, table, key, keyval, cell, cellval):
        if(self.get_onecell(table, key, keyval, key) != None):
            sql = "update '%s' set %s = '%s' where %s = '%s';" % (table, cell, cellval, key, keyval)
        else:
            sql = "insert into '%s' (%s, %s) values ('%s', '%s');" % (table, key, cell, keyval, cellval)
        self.execute(sql)

    """
    * check_table_and_creat - check table and creat it while not exist
    * @table: table name
    * @columns: column group
    * @subsql: create table subsql, for some constraint columns
    """
    def check_table_and_creat(self, table, columns, subsql = None):
        if(not self.check_table(table)):
            sql = "create table '%s' (id integer primary key autoincrement %s);" % (table, subsql)
            self.execute(sql)

        for column in columns:
            if(not self.check_column(table, column[0])):
                self.insert_column(table, column[0], column[1])

