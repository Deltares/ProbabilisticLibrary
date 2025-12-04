#!/bin/bash

set -e
for nb in *.ipynb; do
	case $nb in
		safety_levels.ipynb) continue;;
	esac

	echo "##teamcity[testStarted name='$nb']"
	jupyter-nbconvert --to notebook --ExecutePreprocessor.timeout=-1 --execute $nb
	echo "##teamcity[testFinished name='$nb']"
done
