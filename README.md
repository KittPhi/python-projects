# All Python Projects
- Cython
- Pyarmor
- Custom pip package
- Python examples
- Intel Realsense Depth Camera
- Skeletal Tracking


## Cython setup:
- The `setup.py` file allows you to cythonize a python script `.pyx` and build an `.so` file without the need for type definitions! 

```python
# ensure cython is installed
pip install cython

# setup.py
from distutils.core import setup
from Cython.Build import cythonize

setup(ext_modules = cythonize('helloworld.pyx'))

# Build by executing:
python setup.py build_ext --inplace

```

## cython-helloworld
- basic cythonized helloword

## cython-run-python
- compares runtime between a python script and cythonized python script
