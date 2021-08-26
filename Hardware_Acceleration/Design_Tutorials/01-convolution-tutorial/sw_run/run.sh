#!/bin/bash

# exit when any command fails
set -e

make app

# Run the program
./Filter2D.exe -f 6 -n 60
