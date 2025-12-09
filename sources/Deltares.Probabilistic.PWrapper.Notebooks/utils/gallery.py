import nbclient
import nbformat
from probabilistic_library import Stochast, DistributionType, DiscreteValue, HistogramValue, FragilityValue, ContributingStochast, ConditionalValue
from probabilistic_library import ReliabilityProject, DistributionType, ReliabilityMethod, StartMethod

import nbconvert
import nbformat
from nbconvert.preprocessors import ExecutePreprocessor
import os

path = "../Deltares.Probabilistic.PWrapper.Notebooks"
gallery_path = path + "/gallery"

if not os.path.exists(gallery_path):
   # Create a new directory because it does not exist
   os.makedirs(gallery_path)

max_cells_per_line = 3

class Category:
    def __init__(self, name):
        self.lines = []
        self.cells = []
        self.name = name
        self.empty = True
        self.lines.append(name)
        self.start_table();

    def start_table(self):
        self.lines.append('')
        self.lines.append('| | | |')
        self.lines.append('|-|-|-|')

    def create_cell (self, header, image):
        self.empty = False
        path = 'https://github.com/Deltares/ProbabilisticLibrary/blob/master/sources/Deltares.Probabilistic.PWrapper.Notebooks/'
        link = path + image.replace('.png', '.ipynb')
        self.cells.append(f'[<img src="{image}">]({link})')

        if len(self.cells) == max_cells_per_line:
            self.new_line()

    def new_line(self):
        if len(self.cells) > 0:
            while len(self.cells) < max_cells_per_line:
                self.cells.append(' ')
            self.lines.append('| ' + str.join(' | ', self.cells) + ' |')
            self.cells.clear()

def convert_file(file_name):

    print(file_name)

    pl_notebook = nbformat.read(path + "/" + file_name, as_version = nbformat.NO_CONVERT )
    ep = ExecutePreprocessor(timeout=600, kernel_name='python3')

    image = file_name.replace('.ipynb', '.png')
    image_path = gallery_path + "/" + image

    header = pl_notebook.cells[0].source.split('\n')[0].lstrip(' #')

    category = ''
    for cell in pl_notebook.cells:
        if cell.metadata.hasattr('tags'):
            if 'gallery' in cell.metadata.tags:
                new_lines = []
                for line in cell.source.split('\n'):
                    plot_line = False
                    if line.rstrip().endswith('.plot()'):
                        line = 'plot_object = ' + line.replace('.plot()', '.get_plot()')
                        plot_line = True
                    if line.rstrip().endswith('.show()'):
                        line = 'plot_object = ' + line.replace('.show()', '')
                        plot_line = True
                    if plot_line:
                        line = line + f'\nplot_object.title("{header}")' 
                        line = line + f'\nplot_object.savefig("{image_path}")' 
                    new_lines.append(line)
                cell.source = str.join('\n', new_lines)
            for cat in [cat.name.lower() for cat in categories]:
                if cat in cell.metadata.tags:
                    category = cat

    if category != '':
        for i in range(len(categories)):
            if category.lower() == categories[i].name.lower():
                categories[i].create_cell(header, image)

        out = ep.preprocess(pl_notebook, {'metadata': {'path': path}})

categories = []
categories.append(Category('Statistics'))
categories.append(Category('Reliabilty'))
categories.append(Category('Uncertainty'))
categories.append(Category('Sensitivity'))
categories.append(Category('Model'))

for file in os.listdir(path):
    if file.endswith(".ipynb"):
        convert_file(file)

lines = []
for cat in categories:
    if not cat.empty:
        cat.new_line()
        lines.extend(cat.lines)

with open(gallery_path + "/gallery.md", "w") as file:
    file.writelines(line + '\n' for line in lines)
