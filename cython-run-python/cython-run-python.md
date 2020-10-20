# removed any type definitions

> create `setup.py`

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize('run_cython.pyx'))
```

> Build Cython files
```python
python setup.py build_ext --inplace

mkdir distribution && mv run_cython.so run_python.py test_code.py distribution && cd distribution
```

> Create a test file inside distribution directory, called `test-code.py`

```python
import run_python
import run_cython
import time

number = 10

start = time.time()
run_python.test(number)
end =  time.time()

py_time = end - start
print("Python time = {}".format(py_time))

start = time.time()
run_cython.test(number)
end =  time.time()

cy_time = end - start
print("Cython time = {}".format(cy_time))

print("Speedup = {}".format(py_time / cy_time))
```

> Test by running:

```python
python test-code.py
```