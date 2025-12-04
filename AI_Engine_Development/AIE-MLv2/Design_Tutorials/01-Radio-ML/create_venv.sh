#!/bin/bash
#
# Copyright (C) 2024, Advanced Micro Devices, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Author: Faisal El-Shabani


echo "Container File Notices"
echo ""
echo "NOTICE - BY INVOKING THIS SCRIPT AND USING THE SOFTWARE INSTALLED BY THE SCRIPT, YOU AGREE ON BEHALF OF YOURSELF AND YOUR EMPLOYER (IF APPLICABLE) TO BE BOUND TO THE LICENSE AGREEMENTS APPLICABLE TO THE PACKAGES IDENTIFIED BELOW THAT YOU INSTALL BY RUNNING THE SCRIPT. YOU UNDERSTAND THAT THE INSTALLATION OF THE PACKAGES LISTED BELOW MAY ALSO RESULT IN THE INSTALLATION ON YOUR SYSTEM OF ADDITIONAL PACKAGES NOT LISTED BELOW IN ORDER TO OPERATE (EACH, A ‘DEPENDENCY’). ADVANCED MICRO DEVICES, INC., ON BEHALF OF ITSELF AND ITS SUBSIDIARIES AND AFFILIATES, DOES NOT GRANT TO YOU ANY RIGHTS OR LICENSES TO ANY SUCH DEPENDENCY. THE SCRIPT ITSELF IS LICENSED TO YOU SUBJECT TO THE FOLLOWING TERMS: "
echo ""
echo "Copyright © 2025 Advanced Micro Devices, Inc. All Rights Reserved. "
echo ""
echo "This file is licensed under the following license terms (MIT):  "
echo ""
echo "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:"
echo "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software."
echo ""
echo "THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
echo ""
echo "This file pulls in the following packages subject to the licenses identified below:"
echo "| Package                                | Licenses                                 | URL                                              |"
echo "|----------------------------------------|------------------------------------------|--------------------------------------------------|"
echo "| jupyter                                | BSD                                      | https://pypi.org/project/jupyter/                |"
echo "| tensorflow                             | Apache 2.0                               | https://pypi.org/project/tensorflow/             |"
echo "| matplotlib                             | OSI approved python software foundation  | https://pypi.org/project/matplotlib/             |"
echo "| pydot                                  | OSI approved MIT                         | https://pypi.org/project/pydot/                  |"
echo "| bfloat16                               | Apache software                          | https://pypi.org/project/bfloat16/               |"
echo "| scikit-learn                           | BSD-3-Clause                             | https://pypi.org/project/scikit-learn/           |"
echo ""

echo "Do you want to continue? (yes/no)"
read response

if [[ "$response" == "yes" ]]; then
  echo "Installing Python Libraries"
  python3 -m venv my-venv
  source my-venv/bin/activate
  pip install --upgrade pip
  pip install jupyter
  pip install tensorflow
  pip install matplotlib
  pip install pydot
  pip install bfloat16
  pip install scikit-learn
  pip list
fi

if [[ "$response" == "no" ]]; then
  echo "Permission not granted to proceed"
fi
