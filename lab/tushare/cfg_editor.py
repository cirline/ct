#!/usr/bin/env python

import ConfigParser

class cfg_editor:
    def __init__(self, filename):
        self.filename = filename
        self.parser = ConfigParser.ConfigParser()
        self.parser.read(filename)

    def sections(self):
        return self.parser.sections()

    def options(self, section):
        return self.parser.options(section)

    def items(self, section):
        return self.parser.items(section)

    def get(self, section, option):
        return self.parser.get(section, option)

    def getint(self, section, option):
        return self.parser.getint(section, option)

    def getfloat(self, section, option):
        return self.parser.getfloat(section, option)

    def add_section(self, section):
        return self.parser.add_section(section)

    def set(self, section, option, value):
        return self.parser.set(section, option, value)

    def write(self):
        return self.parser.write(open(self.filename, 'w'))

