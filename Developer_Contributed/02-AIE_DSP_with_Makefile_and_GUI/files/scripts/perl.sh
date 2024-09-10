#!/bin/bash

##################################################################
echo "Override perl related env variables"
PERL_5_24_1=/tools/batonroot/rodin/devkits/lnx64/perl-5.24.1
export PERL_HOME=${PERL_5_24_1}
export PATH=${PERL_5_24_1}/bin:${PATH}

echo "Updated PERL_HOME: ${PERL_HOME}"
echo "Updated PATH: ${PATH}"
