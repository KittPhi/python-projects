> python
```python
$ python -V
Python 2.7.17

$ python3.7 -V
Python 3.7.9
```

> Tested (Works!): To see if pyarmor+cython works in venv. Still requires gcc to have python3.7
```python
# test if pyarmor+cython work in venv
source ~/Repos/e-motion/src/py/venv/bin/activate

$ python -V
Python 3.7.9

$ pip -V
pip 9.0.1 from /home/ecg/Repos/e-motion/src/py/venv/lib/python3.7/site-packages (python 3.7)

$ pip install cython pyarmor

#########################################################################################
# TEST 5 (SUCCESS): cythonize the venv folder
cd ~/Repos/e-motion/src/py/venv/lib64/python3.7/site-packages/repetitions

cythonize -3 -k --lenient butterworth_filter.py rep_detection_30s.py rep_detection.py tug.py && ls

# Create .so file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o butterworth_filter$(python3.7-config --extension-suffix) butterworth_filter.c -fPIC && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o rep_detection_30s$(python3.7-config --extension-suffix) rep_detection_30s.c -fPIC && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o rep_detection$(python3.7-config --extension-suffix) rep_detection.c -fPIC && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o tug$(python3.7-config --extension-suffix) tug.c -fPIC && ls

mv *.py temp && mv temp/__init__.py .
#########################################################################################
# TEST 4 (FAILED): rep-detection repo
cythonize -3 -k --lenient butterworth_filter.py rep_detection_30s.py rep_detection.py && ls

# Create .so file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o butterworth_filter$(python3.7-config --extension-suffix) butterworth_filter.c -fPIC && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o rep_detection_30s$(python3.7-config --extension-suffix) rep_detection_30s.c -fPIC && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o rep_detection$(python3.7-config --extension-suffix) rep_detection.c -fPIC && ls

mv *.py temp

git add .
git tag -a 1.4.4 -m "cython"
git commit -m "cython"
git push origin 1.4.4
git push --set-upstream origin cython

[1] stderr: Traceback (most recent call last):
[1]   File "/home/ecg/Repos/e-motion/src/py/lambda.py", line 7, in <module>
[1]     from helpers.pose_estimator_handler import Pose_Estimator_Handler
[1]   File "/home/ecg/Repos/e-motion/src/py/helpers/pose_estimator_handler.py", line 2, in <module>
[1]     from analyze_test.analyze_test import analyze_test
[1]   File "analyze_test.py", line 6, in init analyze_test
[1]   File "/home/ecg/Repos/e-motion/src/py/analyze_test/repetition_detection/repetitions.py", line 2, in <module>
[1]     from repetitions.butterworth_filter import butterworth_filter
[1] ModuleNotFoundError: No module named 'repetitions.butterworth_filter'
[1] 
[1] child process exited with code 1
# tried excluding butterworth_filter
[1] stderr: Traceback (most recent call last):
[1]   File "/home/ecg/Repos/e-motion/src/py/lambda.py", line 7, in <module>
[1]     from helpers.pose_estimator_handler import Pose_Estimator_Handler
[1]   File "/home/ecg/Repos/e-motion/src/py/helpers/pose_estimator_handler.py", line 2, in <module>
[1]     from analyze_test.analyze_test import analyze_test
[1]   File "analyze_test.py", line 6, in init analyze_test
[1]   File "/home/ecg/Repos/e-motion/src/py/analyze_test/repetition_detection/repetitions.py", line 3, in <module>
[1]     from repetitions.rep_detection import reps_and_time
[1] ModuleNotFoundError: No module named 'repetitions.rep_detection'
[1] 
[1] child process exited with code 1

# put back the way it was:
git add .
git tag -a 1.4.5 -m "removed cython"
git commit -m "removed cython"
git push origin 1.4.5
git push

python3.7 -m pip install git+https://git-codecommit.us-east-1.amazonaws.com/v1/repos/repetitions-detection-pip3 --upgrade


#########################################################################################
# TEST 3 (SUCCESS): 
cythonize -3 -k --lenient analyze_test.py && ls

# Create .so file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o analyze_test$(python3.7-config --extension-suffix) analyze_test.c -fPIC && ls

# remove original py files...
rm analyze_test.py

yarn electron-dev # SUCCESS

# Continue to /home/ecg/Repos/e-motion/src/py/analyze_test/helpers , create c files. Except error_correction.py #####
cythonize -3 -k --lenient compute_velocity.py get_test_failure_reasons.py invert_values.py merge_predictions.py

# Create .so file ... do for each file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o compute_velocity$(python3.7-config --extension-suffix) compute_velocity.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o get_test_failure_reasons$(python3.7-config --extension-suffix) get_test_failure_reasons.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o invert_values$(python3.7-config --extension-suffix) invert_values.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o merge_predictions$(python3.7-config --extension-suffix) merge_predictions.c -fPIC

rm *.c compute_velocity.py get_test_failure_reasons.py invert_values.py merge_predictions.py

yarn electron-dev # FAILED, TRY WITHOUT MOVING .C FILES

[1] stderr: Traceback (most recent call last):
[1]   File "/home/ecg/Repos/e-motion/src/py/lambda.py", line 7, in <module>
[1]     from helpers.pose_estimator_handler import Pose_Estimator_Handler
[1]   File "/home/ecg/Repos/e-motion/src/py/helpers/pose_estimator_handler.py", line 2, in <module>
[1]     from analyze_test.analyze_test import analyze_test
[1]   File "analyze_test.py", line 3, in init analyze_test
[1] ModuleNotFoundError: No module named 'analyze_test.helpers.error_correction'

# ATTEMPT TO NOT REMOVE .C AND SAME ERROR:
# ATTEMPT TO NOT CYTHONIZE ERROR_CORRECTION: SUCCESS!

# Continue to /home/ecg/Repos/e-motion/src/py/analyze_test/repetition_detection, create c file
cythonize -3 -k --lenient repetitions.py && ls

# create .so file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o repetitions$(python3.7-config --extension-suffix) repetitions.c -fPIC && ls

rm *.c repetitions.py

yarn electron-dev # FAILED 
[1] stderr: Traceback (most recent call last):
[1]   File "/home/ecg/Repos/e-motion/src/py/lambda.py", line 7, in <module>
[1]     from helpers.pose_estimator_handler import Pose_Estimator_Handler
[1]   File "/home/ecg/Repos/e-motion/src/py/helpers/pose_estimator_handler.py", line 2, in <module>
[1]     from analyze_test.analyze_test import analyze_test
[1]   File "analyze_test.py", line 6, in init analyze_test
[1]   File "repetitions.py", line 2, in init repetitions
[1] ModuleNotFoundError: No module named 'repetitions.butterworth_filter'; 'repetitions' is not a package

# Continue to Repetition-Detection-pip3 Repo
# Reference: https://luminousmen.com/post/resolve-cython-and-numpy-dependencies





#########################################################################################
# TEST 2: SUCCESSFUL
cythonize -3 -k --lenient analyze_test.py && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o analyze_test$(python3.7-config --extension-suffix) analyze_test.c -fPIC && ls

# remove original py files...
rm analyze_test.py

$ yarn electron-dev
#########################################################################################
# TEST 1: FAILED
# Begin... /home/ecg/Repos/e-motion/src/py/analyze_test
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 analyze_test.py

cd dist && cythonize -3 -k --lenient analyze_test.py pytransform.py && ls

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o analyze_test$(python3.7-config --extension-suffix) analyze_test.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o pytransform$(python3.7-config --extension-suffix) pytransform.c -fPIC

# remove original py files...
rm ../analyze_test.py analyze_test.py && cp * ../

$ yarn electron-dev
[1] stderr: Traceback (most recent call last):
[1]   File "/home/ecg/Repos/e-motion/src/py/lambda.py", line 7, in <module>
[1]     from helpers.pose_estimator_handler import Pose_Estimator_Handler
[1]   File "/home/ecg/Repos/e-motion/src/py/helpers/pose_estimator_handler.py", line 2, in <module>
[1]     from analyze_test.analyze_test import analyze_test
[1]   File "analyze_test.py", line 1, in init analyze_test
[1] ImportError: cannot import name pyarmor_runtime
[1] 
[1] child process exited with code 1

#########################################################################################
# Begin... /home/ecg/Repos/e-motion/src/py/analyze_test/helpers
# Will obfuscates all the *.py in the same folder (creates a dist folder with them):
cd /home/ecg/Repos/e-motion/src/py/analyze_test/helpers
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 compute_velocity.py
# gererates files in dist. 

# (creates .c files)
cd dist && cythonize -3 -k --lenient compute_velocity.py error_correction.py get_test_failure_reasons.py invert_values.py merge_predictions.py pytransform.py

# Create .so file ... do for each file
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o compute_velocity$(python3.7-config --extension-suffix) compute_velocity.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o error_correction$(python3.7-config --extension-suffix) error_correction.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o get_test_failure_reasons$(python3.7-config --extension-suffix) get_test_failure_reasons.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o invert_values$(python3.7-config --extension-suffix) invert_values.c -fPIC

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o merge_predictions$(python3.7-config --extension-suffix) merge_predictions.c -fPIC

# Do this last ... Create .so file for pytransform
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o pytransform$(python3.7-config --extension-suffix) pytransform.c -fPIC

# Move all new files in dist directory one up && delete all .c files and dist directory
mv * .. && cd .. && rm -rf dist *.c 


# Begin... /home/ecg/Repos/e-motion/src/py/analyze_test/repetition_detection
cd /home/ecg/Repos/e-motion/src/py/analyze_test/repetition_detection
pyarmor obfuscate --package-runtime 0 --no-cross-protection --restrict 0 repetitions.py

# (creates .c files)
cd dist && cythonize -3 -k --lenient repetitions.py pytransform.py

gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o repetitions$(python3.7-config --extension-suffix) repetitions.c -fPIC

# Do this last ... Create .so file for pytransform
gcc -shared $(python3.7-config --cflags) $(python3.7-config --ldflags) -o pytransform$(python3.7-config --extension-suffix) pytransform.c -fPIC

rm -rf ../repetitions.py repetitions.py *.c && mv * ../. && cd .. && rm -rf dist

```