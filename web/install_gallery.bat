rem script to install packages for generating gallery

set root=%userprofile%\AppData\Local\miniforge3

call %root%\Scripts\activate.bat %root%
call conda activate prob_lib_dev
call conda install nbconvert 

pause
