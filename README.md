
```python

cd ~/Repos/dev-env-python/

pip install cython
code --install-extension guyskk.language-cython
code --install-extension ms-vscode.cpptools

# rename your python file to '.pyx' and add cython type definitions

# OPTION 1: create a `setup.py` (like a python makefile)
    from distutils.core import setup
    from Cython.Build import cythonize

    setup(ext_modules = cythonize('run_cython.pyx'))

# Use `setup.py` file to build the cython file`run_cython.pyx`
python setup.py build_ext --inplace

# Which will leave a file in your local directory called run_cython.so in unix
# Now to use this file: start the python interpreter and simply import it as if it was a regular python module:
    import run_cython

# You can compare the speed between python and cython
python test_code.py
```