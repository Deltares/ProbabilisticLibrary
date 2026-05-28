#!/bin/bash

set -e

# Notebook exceptions: add filenames here to skip them in CI.
# Example:
#   SKIP_NOTEBOOKS=(
#     "Case_2_DSheetPiling_Tutorial.ipynb"
#   )
SKIP_NOTEBOOKS=(
	"Case_2_DSheetPiling_Tutorial.ipynb"
	"Case_3_DSheetPiling_FragilityCurve.ipynb"
)

should_skip() {
    local nb="$1"
    for skip_nb in "${SKIP_NOTEBOOKS[@]}"; do
        if [ "$nb" = "$skip_nb" ]; then
            return 0
        fi
    done
    return 1
}

for nb in *.ipynb; do
	if should_skip "$nb"; then
		echo "Skipping notebook (exception list): $nb"
		continue
	fi

	echo "##teamcity[testStarted name='$nb']"
	jupyter-nbconvert --to notebook --ExecutePreprocessor.timeout=-1 --execute $nb
	echo "##teamcity[testFinished name='$nb']"
done
