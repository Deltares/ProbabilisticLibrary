"@echo off" 
rem Execute this file in this directory with the wheel files provided 
python -m pip install --find-links=. KratosMultiphysics==9.5.0.5 --force-reinstall
python -m pip install --find-links=. KratosStructuralMechanicsApplication==9.5.0.5 --force-reinstall
python -m pip install --find-links=. KratosGeoMechanicsApplication==9.5.0.5 --force-reinstall

