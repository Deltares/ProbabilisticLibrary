set root=%userprofile%\AppData\Local\miniforge3

call %root%\Scripts\activate.bat %root%
call conda activate prob_lib_dev
call python gallery.py

pause