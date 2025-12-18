import nbformat

from nbconvert.preprocessors import ExecutePreprocessor
import os

directory = os.path.dirname(os.path.abspath(__file__))

notebook_path = 'https://github.com/Deltares/ProbabilisticLibrary/blob/master/sources/Deltares.Probabilistic.PWrapper.Notebooks/'

os.chdir(directory)
os.chdir('..')

current_dir = os.getcwd()

path = current_dir
gallery_path = path + "/gallery"

if not os.path.exists(gallery_path):
   # Create a new directory because it does not exist
   os.makedirs(gallery_path)

max_cells_per_line = 3

class Cell:
    def __init__(self, index, contents):
        self.index = index
        self.contents = contents

    def __lt__(self, other):
        return self.index < other.index

class Category:
    def __init__(self, name):
        self.lines : list[str] = []
        self.cells : list[Cell] = []
        self.name = name
        self.empty = True
        self.lines.append(f'## {name}')
        self.start_table();

    def start_table(self):
        self.lines.append('')
        self.lines.append('| | | |')
        self.lines.append('|:-:|:-:|:-:|')

    def create_image_cell (self, index, header, image):
        self.empty = False
        link = notebook_path + image.replace('.png', '.ipynb')
        contents = f'[<img src="{image}">]({link})[{header}]({link})'
        self.cells.append(Cell(index, contents))

    def create_text_cell (self, index, header, notebook):
        self.empty = False
        link = notebook_path + notebook
        contents = f'[{header}]({link})'
        self.cells.append(Cell(index, contents))

    def finalize(self):
        self.cells.sort()

        contents = []
        for cell in self.cells:
            contents.append(cell.contents)
            if len(contents) == max_cells_per_line:
                self.write_line(contents)
                contents = []

        self.write_line(contents)

    def write_line(self, contents):
        if len(contents) > 0:
            while len(contents) < max_cells_per_line:
                contents.append(' ')
            self.lines.append('| ' + str.join(' | ', contents) + ' |')
            contents.clear()

def convert_file(file_name):

    print(file_name)

    pl_notebook = nbformat.read(path + "/" + file_name, as_version = nbformat.NO_CONVERT )
    ep = ExecutePreprocessor(timeout=600, kernel_name='python3')

    image = file_name.replace('.ipynb', '.png')
    header = pl_notebook.cells[0].source.split('\n')[0].lstrip(' #')

    category = ''
    index = 9999999
    has_plot = False
    for cell in pl_notebook.cells:
        if cell.metadata.hasattr('tags'):
            if 'gallery' in cell.metadata.tags:
                new_lines : list[str] = []
                has_plot = True
                for line in cell.source.split('\n'):
                    plot_line = False
                    for plot_statement in ['plot', 'plot_alphas', 'plot_realizations', 'plot_convergence']:
                        if line.rstrip().endswith(f'.{plot_statement}()'):
                            line = 'plot_object = ' + line.replace(f'.{plot_statement}()', f'.get_{plot_statement}()')
                            plot_line = True
                    if line.rstrip().endswith('.show()'):
                        line = 'plot_object = ' + line.replace('.show()', '')
                        plot_line = True
                    if plot_line:
                        image_path = gallery_path + "/" + image
                        line = line + f'\nplot_object.title("{header}")' 
                        line = line + f'\nplot_object.savefig("{image_path}")' 
                    new_lines.append(line)
                cell.source = str.join('\n', new_lines)
            for cat in [cat.name.lower() for cat in categories]:
                if cat in cell.metadata.tags:
                    category = cat
            for tag in cell.metadata.tags:
                if tag.isdigit():
                    index = int(tag)

    if category != '':
        for i in range(len(categories)):
            if category.lower() == categories[i].name.lower():
                if has_plot:
                    categories[i].create_image_cell(index, header, image)
                else:
                    categories[i].create_text_cell(index, header, file_name)

        if has_plot:
            out = ep.preprocess(pl_notebook, {'metadata': {'path': path}})

categories : list[Category] = []
categories.append(Category('Statistics'))
categories.append(Category('Reliability'))
categories.append(Category('Uncertainty'))
categories.append(Category('Model'))

for file in os.listdir(path):
    if file.endswith(".ipynb"):
        convert_file(file)

lines : list[str] = []
for cat in categories:
    if not cat.empty:
        cat.finalize()
        lines.extend(cat.lines)

with open(gallery_path + "/gallery.md", "w") as file:
    file.writelines(line + '\n' for line in lines)