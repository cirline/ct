import numpy as np
import pandas as pd
import time

print np.random.randn(3)
print '-----------\n'

df = pd.DataFrame(np.random.randn(2, 14))
print df
print '-----------\n'

df = pd.DataFrame(np.random.randn(2, 4), columns = list('ABCD'))
print df
print '-----------\n'

df = pd.DataFrame({'A':np.random.randn(4)})
print df
print '-----------\n'

print df.dtypes

print df.head(2)

print df.tail(1)

print df.index

print df.columns

print df.values

print df.describe

print df.T
