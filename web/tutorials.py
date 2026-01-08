import nbformat

from nbconvert.preprocessors import ExecutePreprocessor
import os

directory = os.path.dirname(os.path.abspath(__file__))
current_dir = os.getcwd()
path = current_dir

gallery_path = path + "/gallery"
notebook_path = path + "/../sources/Deltares.Probabilistic.PWrapper.Notebooks"
notebook_web_path = 'https://github.com/Deltares/ProbabilisticLibrary/blob/master/sources/Deltares.Probabilistic.PWrapper.Notebooks/'

os.chdir(notebook_path)

if not os.path.exists(gallery_path):
   # Create a new directory because it does not exist
   os.makedirs(gallery_path)

max_cells_per_line = 3
cell_counter = 1
cell_size = 32

class Cell:
    cell_counter = 0

    def __init__(self, index, name, header):
        self.index = index
        self.id = cell_counter
        Cell.cell_counter += 1

        id = str(Cell.cell_counter)

        self.def_lines : list[str] = []
        self.def_lines.append(f'.. |{id}_img| image:: {name}.png')
        self.def_lines.append(f'   :width: 180px')
        self.def_lines.append(f'   :alt: {header}')
        self.def_lines.append(f'')
        self.def_lines.append(f'.. |{id}_nb| replace:: `{header} <../_examples/{name}.ipynb>`__')
        self.def_lines.append(f'')

        self.nb_lines : list[str] = []
        self.nb_lines.append(f'   ../_examples/{name}.ipynb')

        self.table_lines : list[str] = []
        self.table_lines.append(f'| |{id}_img|')
        self.table_lines.append(f'| |{id}_nb|')


    def __lt__(self, other):
        return self.index < other.index

class Category:
    def __init__(self, name):
        self.table_lines : list[str] = []
        self.notebook_lines : list[str] = []
        self.cells : list[Cell] = []
        self.name = name
        self.empty = True

    def create_cell (self, index, header, image):
        self.empty = False
        name = image.replace('.png', '')
        name = name.replace('.ipynb', '')
        self.cells.append(Cell(index, name, header))

    def get_def_lines(self):
        self.cells.sort()

        def_lines : list[str] = []
        for cell in self.cells:
            def_lines.extend(cell.def_lines)

        return def_lines

    def get_table_lines(self):
        self.cells.sort()

        table_lines : list[str] = []
        table_lines.append(self.name)
        table_lines.append(self._extend('', '-', len(self.name)))
        table_lines.append('')

        header_line = ''
        first_line = ''
        second_line = ''
        counter = 0

        for cell in self.cells:
            header_line += self._extend('+', '-', cell_size)
            first_line += self._extend(cell.table_lines[0], ' ', cell_size)
            second_line += self._extend(cell.table_lines[1], ' ', cell_size)

            counter += 1
            if counter == max_cells_per_line:
                header_line += '+'
                first_line += '|'
                second_line += '|'
                table_lines.append(header_line)
                table_lines.append(first_line)
                table_lines.append(second_line)

                header_line = ''
                first_line = ''
                second_line = ''
                counter = 0

        if counter > 0 and counter < max_cells_per_line:
            while counter < max_cells_per_line:
                header_line += self._extend('+', '-', cell_size)
                first_line += self._extend('|', ' ', cell_size)
                second_line += self._extend('|', ' ', cell_size)
                counter += 1

            header_line += '+'
            first_line += '|'
            second_line += '|'

            table_lines.append(header_line)
            table_lines.append(first_line)
            table_lines.append(second_line)

        bottom_line = ''
        for i in range(max_cells_per_line):
            bottom_line += self._extend('+', '-', cell_size)
        bottom_line += '+'
        table_lines.append(bottom_line)

        table_lines.append('')
        return table_lines

    def get_nb_lines(self):
        self.cells.sort()

        nb_lines : list[str] = []
        for cell in self.cells:
            nb_lines.extend(cell.nb_lines)

        return nb_lines

    def _extend(self, text, trailing, size):
        while len(text) < size:
            text += trailing
        return text

def convert_file(file_name):

    print(file_name)

    pl_notebook = nbformat.read(notebook_path + "/" + file_name, as_version = nbformat.NO_CONVERT )
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
                    categories[i].create_cell(index, header, image)
                else:
                    categories[i].create_cell(index, header, file_name)

        if has_plot:
            out = ep.preprocess(pl_notebook, {'metadata': {'path': notebook_path}})

categories : list[Category] = []
categories.append(Category('Statistics'))
categories.append(Category('Reliability'))
categories.append(Category('Uncertainty'))
categories.append(Category('Model'))

for file in os.listdir(notebook_path):
    if file.endswith(".ipynb"):
        convert_file(file)

lines : list[str] = []

lines.append('Tutorials')
lines.append('=========')
lines.append('')

for cat in categories:
    if not cat.empty:
        lines.extend(cat.get_def_lines())

for cat in categories:
    if not cat.empty:
        lines.extend(cat.get_table_lines())

lines.append('')
lines.append('.. toctree::')
lines.append('   :maxdepth: 1')
lines.append('   :caption: Tutorials')
lines.append('')

for cat in categories:
    if not cat.empty:
        lines.extend(cat.get_nb_lines())


with open(gallery_path + "/tutorials.rst", "w") as file:
    file.writelines(line + '\n' for line in lines)