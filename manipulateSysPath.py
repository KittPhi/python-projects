# https://leemendelowitz.github.io/blog/how-does-python-find-packages.html
# Python imports work by searching the directories listed in sys.path.

# In VS Code select Interpreter Python 3.7.5 /usr/bin/python

# Run
import sys, os
print('\n'.join(sys.path), '\n')

# This wont work: ###################################################################
# import hi

# Traceback (most recent call last):
#   File "/home/ecg/Repos/dev-env-python/manipulate-sys-path.py", line 3, in <module>
#     import hi
# ModuleNotFoundError: No module named 'hi'

#####################################################################################
# Create a hi module in your home directory. ########################################
home_dir = os.path.expanduser("~")
print(home_dir) # /home/ecg

my_module_file = os.path.join(home_dir, "hi.py")
with open(my_module_file, 'w') as f:
  f.write('print("hi")\n')
  f.write('a=10\n')

# Add the home directory to sys.path ################################################
sys.path.append(home_dir)

# Now this works, and prints hi!
import hi 
print(hi.a)
#####################################################################################
# To see where modules are installed
import numpy
print(numpy.__file__)
# /usr/lib/python3.7/numpy/__init__.py

# wont work:
# The file attribute is not present for C modules that are statically linked into the interpreter; 
# for extension modules loaded dynamically from a shared library, it is the pathname of the shared library file.

# So, for example this doesn't work:
# Because sys module is statically linked to the interpreter - it is essentially part of the interpreter!
import sys
# print(sys.__file__)

import site
print(site.__file__)
# /usr/lib/python3.7/site.py