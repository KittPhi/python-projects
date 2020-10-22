# Working example of Pyarmor and Cython working together. "Hello World"

> python
```python
$ python -V
Python 2.7.17

$ python3.7 -V
Python 3.7.9
```

> TESTED (Test 4): WORKED! by changing python to python3.7 in gcc commands!
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