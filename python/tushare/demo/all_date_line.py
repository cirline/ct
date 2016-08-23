#!/usr/bin/python

import tushare as ts

rc = ts.get_hist_data('601398')
print rc
