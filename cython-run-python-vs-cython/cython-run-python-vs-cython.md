```python
cd ~/Repos/dev-env-python/cython-run-python-vs-cython
```

> TEST 1:
> removed any type definitions
> create `setup.py`

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize('run_cython.pyx'))
```

```python python3.7
python3.7 setup.py build_ext --inplace
# creates run_cython.cpython-37m-x86_64-linux-gnu.so
mv run_cython.cpython-37m-x86_64-linux-gnu.so temp && cd temp && ls 

$ python3.7 test_code.py 
    Python time = 2.384185791015625e-06
    Cython time = 1.1920928955078125e-06
    Speedup = 2.0

$ python test_code.py
    Traceback (most recent call last):
    File "test_code.py", line 2, in <module>
        import run_cython
    ImportError: No module named run_cython

# reset test environment
clear && ls && rm -rf __pycache__ run_python.pyc run_cython.cpython-37m-x86_64-linux-gnu.so && ls && cd .. && rm -rf build run_cython.c


rm -rf run_python.pyc run_cython.c build

```
> TEST 2: python
```python
python setup.py build_ext --inplace
# produced run_cython.so
mv run_cython.so temp && cd temp && ls

$ python test_code.py 
    Python time = 5.96046447754e-06
    Cython time = 2.14576721191e-06
    Speedup = 2.77777777778

$ python3 test_code.py
    Traceback (most recent call last):
    File "test_code.py", line 2, in <module>
        import run_cython
    ImportError: /home/ecg/Repos/dev-env-python/cython-run-python-vs-cython/temp/run_cython.so: undefined symbol: _Py_ZeroStruct

# still inside temp, delete files, to reset test
rm -rf __pycache__ run_cython.so run_python.pyc && ls && cd .. && ls && rm -rf run_cython.c build && ls
```

> Both get warning:
```python
$ python3.7 setup.py build_ext --inplace
Compiling run_cython.pyx because it changed.
[1/1] Cythonizing run_cython.pyx
/home/ecg/.local/lib/python3.7/site-packages/Cython/Compiler/Main.py:369: FutureWarning: Cython directive 'language_level' not set, using 2 for now (Py2). This will change in a later release! File: /home/ecg/Repos/dev-env-python/cython-run-python-vs-cython/run_cython.pyx
  tree = Parsing.p_module(s, pxd, full_module_name)
running build_ext
building 'run_cython' extension
creating build
creating build/temp.linux-x86_64-3.7
x86_64-linux-gnu-gcc -pthread -Wno-unused-result -Wsign-compare -DNDEBUG -g -fwrapv -O2 -Wall -g -fdebug-prefix-map=/build/python3.7-zVxTFU/python3.7-3.7.9=. -fstack-protector-strong -Wformat -Werror=format-security -g -fdebug-prefix-map=/build/python3.7-zVxTFU/python3.7-3.7.9=. -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 -fPIC -I/usr/include/python3.7m -c run_cython.c -o build/temp.linux-x86_64-3.7/run_cython.o
x86_64-linux-gnu-gcc -pthread -shared -Wl,-O1 -Wl,-Bsymbolic-functions -Wl,-Bsymbolic-functions -Wl,-z,relro -Wl,-Bsymbolic-functions -Wl,-z,relro -g -fdebug-prefix-map=/build/python3.7-zVxTFU/python3.7-3.7.9=. -fstack-protector-strong -Wformat -Werror=format-security -Wdate-time -D_FORTIFY_SOURCE=2 build/temp.linux-x86_64-3.7/run_cython.o -o /home/ecg/Repos/dev-env-python/cython-run-python-vs-cython/run_cython.cpython-37m-x86_64-linux-gnu.so
```