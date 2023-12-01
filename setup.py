from pathlib import Path
from pybind11.setup_helpers import Pybind11Extension, build_ext
from setuptools import setup

rvl = Pybind11Extension(
    'rvl',
    [str(fname) for fname in Path('src').glob('*.cpp')],
    include_dirs=['include'],
    extra_compile_agrs=['-O3']
)

setup(
    name='rvl',
    version=0.1,
    author="Mason Peterson",
    author_email="masonbp@mit.edu",
    description="RVL data compression python bindings.",
    ext_modules=[rvl],
    cmdclass={'build_ext': build_ext}
)