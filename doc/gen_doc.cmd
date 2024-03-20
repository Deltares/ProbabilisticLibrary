@echo off

pdflatex -quiet funcdesign
bibtex -quiet funcdesign
pdflatex -quiet funcdesign
pdflatex -quiet funcdesign

pdflatex -quiet scientific_background
bibtex -quiet scientific_background
pdflatex -quiet scientific_background
pdflatex -quiet scientific_background
