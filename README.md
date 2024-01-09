# rvlpy

Python bindings for RVL data decompression.

# Setup

Clone this repo and pull subrepositories, cd into this repo and run:

```
pip install -e . -vvv
```

If this raises an error about not being about to find `pybind11`, this solution from [stackoverflow](https://stackoverflow.com/questions/61235727/no-module-named-pybind11-after-installing-pybind11) may help:

```
pip install wheel setuptools pip --upgrade
```

# References

The c++ code was taken from the [Fast Lossles Depth Image Cmopression paper](https://www.microsoft.com/en-us/research/uploads/prod/2018/09/p100-wilson.pdf).