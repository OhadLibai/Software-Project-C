
from setuptools import Extension, setup

module = Extension("SymNMF", sources=['symnmfmodule.c'])
setup(name='SymNMF',
     version='1.0',
     description='Python wrapper for custom C extension',
     ext_modules=[module])