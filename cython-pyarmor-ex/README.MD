pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 foo.py

cd dist && cythonize -3 -k --lenient foo.py pytransform.py

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
     -o foo$(python-config --extension-suffix) foo.c -fPIC

gcc -shared $(python-config --cflags) $(python-config --ldflags) \
    -o pytransform$(python-config --extension-suffix) pytransform.c -fPIC


```python
# this never worked
mkdir tmp && mv foo.py pytransform.py tmp
python -c 'import foo'
```