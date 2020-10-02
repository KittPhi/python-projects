## Why Cython:
> Becauses it increases the speed of Python by 4 times.

> Third-party libraries like NumPy, which wrap C libraries, can improve the performance of some operations significantly, 
but sometimes you just need the raw speed and power of C directly in Python.

> Cython was developed to make it easier to write C extensions for Python, and to allow existing Python code to be transformed into C

> What’s more, Cython allows the optimized code to be shipped with a Python application without external dependencies.

### Install cython

[cython reference](https://cython.readthedocs.io/en/latest/src/tutorial/cython_tutorial.html)


```python
# activate venv
source ~/Repos/e-moiton/src/py/venv/bin/activate

cd ~/Repos/dev-env-python/cython-pyarmor-run-python

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

# OPTION 2: use pyximport; an experimental compilation library
    import pyximport; pyximport.install()
    import helloworld
        Hello World

# Which will leave a file in your local directory called run_cython.so in unix
# Now to use this file: start the python interpreter and simply import it as if it was a regular python module:
    import run_cython

# You can compare the speed between python and cython
python test_code.py
```
## Why PyArmor
> Due to the interpreted nature of Python, source code protection is a challenge.

> PyArmor allows the obfuscation of Python code.

```python
# activate venv
source ~/Repos/e-moiton/src/py/venv/bin/activate
cd ~/Repos/dev-env-python/cython-pyarmor-run-python
pip install pyarmor

# temp move files
mkdir tmp && mv run_python.py setup.py test_code.py tmp

# obfascate `run_cython.pyx`
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 run_cython.pyx

# Next cythonize both run_cython.py and pytransform.py with extra options -k and --lenient to generate `run_cython.c` and `pytransform.c`:
cd dist
cythonize -3 -k --lenient run_cython.pyx pytransform.py

# compile `run_cython.c`
gcc -shared $(python-config --cflags) $(python-config --ldflags) \
     -o run_cython$(python-config --extension-suffix) run_cython.c -fPIC

cd dist
gcc -shared $(python-config --cflags) $(python-config --ldflags) \
    -o pytransform$(python-config --extension-suffix) pytransform.c -fPIC

mv run_cython.py dist/tmp

# You can compare the speed between python and cython
python test_code.py

```