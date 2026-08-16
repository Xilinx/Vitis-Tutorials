# Copyright (c) 2026 Midhat Nashar
# SPDX-License-Identifier: MIT
"""Validate the Python packages required by the M19 tutorial."""

import sys
from importlib.metadata import version

import aie
import ml_dtypes
import numpy
import pyxrt
from numpy.lib import NumpyVersion


def main():
    expected = {
        "mlir-aie": "1.4.1",
        "llvm-aie": "21.0.0.2026080301+c9c5ecb7",
    }
    actual = {name: version(name) for name in expected}
    if actual != expected:
        raise RuntimeError(f"package mismatch: expected {expected}, found {actual}")
    if not (
        NumpyVersion("2.5.1") <= NumpyVersion(numpy.__version__) < NumpyVersion("3.0.0")
    ):
        raise RuntimeError(f"NumPy {numpy.__version__} is outside [2.5.1, 3.0)")
    if NumpyVersion(ml_dtypes.__version__) < NumpyVersion("0.5.4"):
        raise RuntimeError(f"ml_dtypes {ml_dtypes.__version__} is below 0.5.4")

    print("Python:", sys.version)
    print("Packages:", actual)
    print("NumPy:", numpy.__version__)
    print("ml_dtypes:", ml_dtypes.__version__)
    print("aie:", aie.__file__)
    print("pyxrt:", pyxrt.__file__)


if __name__ == "__main__":
    main()
