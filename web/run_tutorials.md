
# Functionality

The batch file `run_tutorials.bat` generates the rst file for the tutorials (`tutorials.rst`) and all needed image files. The files are placed in the [directory](https://github.com/Deltares/ProbabilisticLibrary/tree/master/web/gallery). After running the batch file, the gallery contents should be pushed to the repository.

# Requirements

```
pip install nbconvert
```

The batch script is run in environment prob_lib_dev

# Preparation of notebooks

The notebooks should be enhanced with tags.

### Gallery

Add the tag `gallery` for the cell from which the image should be taken. If multiple images exist, the last image is taken. The image can only be taken if a command *.show(), *.plot() or *.plot_<alphas|realizations|convergence>() exists in the cell.

### Category

A tag `statistics`, `reliability`, `uncertainty` or `model` should exist somewhere in the notebook. This is used to put the notebook in the right classification.

### Numeric value

Add a numeric value (positive integer) as tag to define the order of notebooks within a category. If not set, the notebook will appear at the end of the category.