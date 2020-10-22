> python
```python
$ python -V
Python 2.7.17

$ python3.7 -V
Python 3.7.9
```

> TEST 1:
```python
#TEST 1:
cythonize -k --lenient foo.py

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
     -o foo$(python-config --extension-suffix) foo.c -fPIC

mv foo.py tmp && mv ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/test.py ~/Repos/dev-env-python/pyarmor-cython-helloworld

ecg@ecg:~/Repos/dev-env-python/pyarmor-cython-helloworld$ python test.py
module name is foo
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
  File "foo.py", line 4, in init foo
NameError: name '__file__' is not defined

ecg@ecg:~/Repos/dev-env-python/pyarmor-cython-helloworld$ python3.7 test.py
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
ImportError: /home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/foo.so: undefined symbol: _Py_ZeroStruct

# reset test
rm -rf foo.c foo.so && mv test.py ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp && mv ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/foo.py . && clear
```


> TEST 2:
```python
# TEST 2:
cythonize -3 -k --lenient foo.py

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
     -o foo$(python-config --extension-suffix) foo.c -fPIC

mv foo.py tmp && mv ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/test.py ~/Repos/dev-env-python/pyarmor-cython-helloworld

$ python test.py
module name is foo
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
  File "foo.py", line 4, in init foo
NameError: name '__file__' is not defined

$ python3 test.py
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
ImportError: /home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/foo.so: undefined symbol: _Py_ZeroStruct

# reset test
rm -rf foo.c foo.so && mv test.py ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp && mv ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/foo.py . && clear

```

> TEST 3:
```python
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 foo.py

cd dist && cythonize -3 -k --lenient foo.py pytransform.py

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
     -o foo$(python-config --extension-suffix) foo.c -fPIC

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
    -o pytransform$(python-config --extension-suffix) pytransform.c -fPIC

cp ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/test.py .

mkdir temp && mv foo.py pytransform.py ~/Repos/dev-env-python/pyarmor-cython-helloworld/dist/temp && ls

# run tests
$ python test.py
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
  File "foo.py", line 3, in init foo
NameError: name '__file__' is not defined

$ python3.7 test.py
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
ImportError: /home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/dist/foo.so: undefined symbol: _Py_ZeroStruct

# reset tests
cd .. && rm -rf dist && clear && ls
```

> TEST 4: WORKED! by changing python to python3.7 in gcc commands!
```python
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 foo.py

cd dist && cythonize -3 -k --lenient foo.py pytransform.py

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o foo$(python3.7-config --extension-suffix) foo.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o pytransform$(python3.7-config --extension-suffix) pytransform.c -fPIC

cp ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/test.py .

# Need 4 dist files; relative to foo.py 
mv pytransform.py _pytransform.so pytransform.cpython-37m-x86_64-linux-gnu.so foo.cpython-37m-x86_64-linux-gnu.so ~/Repos/dev-env-python/pyarmor-cython-helloworld/tmp && cd ../tmp

# test
python3.7 test.py

$ python3.7 test.py
Hello World
module name is __main__
module file is test.py
<module 'foo' from '/home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/tmp/foo.cpython-37m-x86_64-linux-gnu.so'>

$ python3.7 -c 'import foo'
Hello World

# Reset Folders to test again
rm -rf pytransform.py _pytransform.so pytransform.cpython-37m-x86_64-linux-gnu.so foo.cpython-37m-x86_64-linux-gnu.so __pycache__ _pytransform.so && cd .. && rm -rf dist && ls

```


> TEST 5: On Ubuntu 20
```python
# run tests: OLD
$ python test.py
Hello World!
module name is __main__
module file is test.py
```

```python
$ pip -V
pip 20.2.4 from /home/ecg/.local/lib/python3.7/site-packages/pip (python 3.7)

$ pip3 -V
pip 20.2.4 from /home/ecg/.local/lib/python3.7/site-packages/pip (python 3.7)

$ python3.7 -m pip -V
pip 20.2.4 from /home/ecg/.local/lib/python3.7/site-packages/pip (python 3.7)

$ python -V
Python 2.7.17

$ python3 -V
Python 3.6.9

$ python3.7 -V
Python 3.7.9

# ERROR:
$ python3 test.py
Traceback (most recent call last):
  File "test.py", line 1, in <module>
    import foo
ImportError: /home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/dist/foo.so: undefined symbol: _Py_ZeroStruct

# ERROR:
$ python3 -c 'import foo'
Traceback (most recent call last):
  File "<string>", line 1, in <module>
ImportError: /home/ecg/Repos/dev-env-python/pyarmor-cython-helloworld/dist/foo.so: undefined symbol: _Py_ZeroStruct

# So I installed ... 
pip install --user pycocotools
# didn't fix problem


```