# dev-env-python
[reference](https://towardsdatascience.com/use-cython-to-get-more-than-30x-speedup-on-your-python-code-f6cb337919b6)

## Why Cython:
> Third-party libraries like NumPy, which wrap C libraries, can improve the performance of some operations significantly, 
but sometimes you just need the raw speed and power of C directly in Python.

> Cython was developed to make it easier to write C extensions for Python, and to allow existing Python code to be transformed into C

> What’s more, Cython allows the optimized code to be shipped with a Python application without external dependencies.

Install cython

```python
source ~/Repos/e-moiton/src/py/venv/bin/activate
pip install cython
code --install-extension guyskk.language-cython
code --install-extension ms-vscode.cpptools

# compile to python to C
cd cython-example
python setup.py build_ext --inplace

```
