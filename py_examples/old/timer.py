# timeit is more accurate. use time.default_timer()
# time for Python 3 use time.perf_counter()
# time for Pyhton 2 use time.time() or time.clock()
# 1 second = 1000 milliseconds, same as 0.001 second = 1 millisecond
# https://stackoverflow.com/questions/17579357/time-time-vs-timeit-timeit

import time
import timeit

def some_func():
    x = 0
    for i in range(50):
        y = 1000
        x = x + y
    return x

def time_1():
    start = time.perf_counter()
    some_func()
    end = time.perf_counter()
    elapsed = end - start
    print("time: %0.7f" % elapsed, " seconds")
    print("time: %0.7f" % (elapsed*1000), " milliseconds")

def time_2():
    start = timeit.default_timer()
    some_func()
    end = timeit.default_timer()
    elapsed = end - start
    print(f"timeit: {elapsed:0.7f} seconds")
    elapsed = (end - start) * 1000  # convert seconds to milliseconds
    print(f"timeit: {elapsed:0.7f} milliseconds")

time_1()
time_2()
