rem script to install sphinx
rem install https://nodejs.org/en (not sure about this)

set root=%userprofile%\AppData\Local\miniforge3

call %root%\Scripts\activate.bat %root%
call conda env create -f environment.yml
call conda activate prob_lib_dev
call poetry install 

pause
