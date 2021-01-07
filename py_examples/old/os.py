# Built-in library functions.
# import os
from os import path

def check():
    print('File|Directory exists:', path.exists("test.py"))
    print('File exists:', path.isfile('test.txt'))
    print('Directory exists:', path.isdir('test'))

check()