#!/bin/bash
#
# Copyright (C) 2026, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani


# Make sure python is installed and the version is 3.9 to 3.14
# Check if Python 3 is installed
if ! command -v python3 &> /dev/null
then
    echo "Python 3 is not installed. Please install Python 3 and try again."
fi
# Check Python version
PYTHON_VERSION_MAJOR="$(python3 --version | cut -d ' ' -f 2 | cut -d '.' -f 1)"
PYTHON_VERSION_MINOR="$(python3 --version | cut -d ' ' -f 2 | cut -d '.' -f 2)"
if [[ ( $PYTHON_VERSION_MAJOR != 3 || $PYTHON_VERSION_MINOR -lt 9 || $PYTHON_VERSION_MINOR -gt 14) ]]; then
    echo "Python version must be between 3.9 and 3.14. Please install the correct version and try again."
fi


python3 -m venv my-venv
source my-venv/bin/activate
pip install --upgrade pip
pip install jupyter
pip install numpy
pip install matplotlib
pip install scipy
pip list
