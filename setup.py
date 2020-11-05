from setuptools import setup

setup(
    name='pip-package',
    version='0.0.1',
    description='Hello world pip package from private github repo',
    url='git@github.com:KittPhi/pip-package.git',
    author='Kitt Phi',
    author_email='kittphi@gmail.com',
    license='unlicense',
    packages=['pip-package'],
    zip_safe=False
)