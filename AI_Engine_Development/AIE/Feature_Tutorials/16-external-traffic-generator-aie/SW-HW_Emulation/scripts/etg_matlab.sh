echo ${PWD}
/tools/gensys/matlab/R2022a/bin/matlab -batch "scriptDir = fullfile(pwd, 'TrafficGenerator/MATLAB');addpath(scriptDir);scriptToRun = 'xtg_aie.m';run(scriptToRun);rmpath(scriptDir);"

