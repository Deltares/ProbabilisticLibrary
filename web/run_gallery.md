
# Functionality

The batch file `run_gallery.bat` generates the markdown file for the gallery (`gallery.md`) and all needed image files. The files are placed in the [directory](https://github.com/Deltares/ProbabilisticLibrary/tree/master/sources/Deltares.Probabilistic.PWrapper.Notebooks/utils). After running the batch file, the gallery contents should be pushed to the repository.

# Requirements

```
pip install nbconvert
```

# Preparation of notebooks

The notebooks should be enhanced with tags.

### Gallery

Add the tag `gallery` for the cell from which the image should be taken. If multiple images exist, the last image is taken. The image can only be taken if a command *.show(), *.plot() or *.plot_<alphas|realizations|convergence>() exists in the cell.

### Category

A tag `statistics`, `reliability`, `uncertainty` or `model` should exist somewhere in the notebook. This is used to put the notebook in the right classification.

### Numeric value

Add a numeric value (positive integer) as tag to define the order of notebooks within a category. If not set, the notebook will appear at the end of the category.