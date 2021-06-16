#!/usr/bin/bash

DATE=`date +%Y%m%d_%H%M`
echo "Startof CheckMakefile.sh $DATE"

#export ENVROOT=/proj/rdi/env/stable/hierdesign
#source ${ENVROOT}/skel/g_profile
export ENVROOT=/proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization
source /proj/rdi/env/stable/hierdesign/skel/g_profile
umask 002

# "branch" env variable should be present in Jenkins Jobs
if [ "${branch}" == "master" ]; then
	EXT_2020_2=".2020.2"
else
	EXT_2020_2=""
fi  
if [[ -z "$exception" ]]; then
	dedicated_exception=/proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/Exception.${libname}${EXT_2020_2}.json
	if [[ -f $dedicated_exception ]]; then
		export exception=$dedicated_exception
	else
		export exception=/proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/Exception${EXT_2020_2}.json
	fi
fi

echo "[INFO] : using $exception"
python /proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/CheckMakefile.py --path ${WORKSPACE} --exception $exception --forbidden-variables /proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/forbidden_variables.json --version ${TOOLVERSION}

DATE=`date +%Y%m%d_%H%M`
echo "End of CheckMakefile.sh $DATE"
