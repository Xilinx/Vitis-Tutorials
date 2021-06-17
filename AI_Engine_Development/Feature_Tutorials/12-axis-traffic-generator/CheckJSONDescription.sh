#!/usr/bin/bash

DATE=`date +%Y%m%d_%H%M`
echo "Startof CheckJSONDescription.sh $DATE"

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

if [[ -z "$GENERATE" ]]; then
	GENERATE=True
fi

if [[ ! -z "$select_allowed_platforms" && "$select_allowed_platforms" != '' ]] ; then
    export SELECT_ALLOWED_PLATFORMS=$select_allowed_platforms
    echo "[INFO]: CheckJsonDescription.sh: SELECT_ALLOWED_PLATFORMS=$SELECT_ALLOWED_PLATFORMS"
fi

if [[ ! -z "$force_platforms" && "$force_platforms" != '' ]] ; then
    export FORCE_PLATFORMS=$force_platforms
    echo "[INFO]: CheckJsonDescription.sh: FORCE_PLATFORMS=$FORCE_PLATFORMS"
fi

ADDITIONAL_ARG=""
if [[ ! -z "$coverage" && "$coverage" == 'true' ]] ; then
	echo "[INFO] CheckJSONDescription.sh : Add coverage vars to testinfo.yml"
	ADDITIONAL_ARG=" --coverage True "
fi

#echo "[INFO] : using $exception"
echo "[DEBUG] env vars"
set
echo "[DEBUG] python /proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/CheckJsonDescription.py --library-name ${libname} --path ${WORKSPACE} --exception $exception --generate ${GENERATE} --output $WORKSPACE/tcrepo --version ${TOOLVERSION} ${ADDITIONAL_ARG}"
python /proj/xtools/dsv/projects/sdx_libs/CheckLibraryStandardization/CheckJsonDescription.py --library-name ${libname} --path ${WORKSPACE} --exception $exception --generate ${GENERATE} --output $WORKSPACE/tcrepo --version ${TOOLVERSION} ${ADDITIONAL_ARG}

DATE=`date +%Y%m%d_%H%M`
echo "End of CheckJSONDescription.sh $DATE"
