

pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 foo.py

cd dist
cythonize -3 -k --lenient foo.py pytransform.py

