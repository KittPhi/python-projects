[reference](https://cython.readthedocs.io/en/latest/src/tutorial/cython_tutorial.html)

Save helloworld.py as `helloworld.pyx`

create `setup.py`

```python
from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize('helloworld.pyx'))
```

Build Cython files
```python
python setup.py build_ext --inplace

mkdir distribution && mv helloworld.so distribution
cd distribution
```

create a test file inside distribution directory, called `test-helloworld.py`

```python
import helloworld

print(helloworld)
```

Test by running:

```python
python test-helloworld.py
```