from probabilistic_library import Stochast, DistributionType, DiscreteValue, HistogramValue, FragilityValue, ContributingStochast, ConditionalValue

cells = []
lines = []

max_cells_per_line = 3

def start_table():
    lines.append('')
    lines.append('| | | |')
    lines.append('|-|-|-|')

def create_cell (header, notebook, plot):
    plot.title(header)
    plot.savefig(notebook + '.png')
    path = 'https://github.com/Deltares/ProbabilisticLibrary/blob/master/sources/Deltares.Probabilistic.PWrapper.Notebooks/'
    link = path + notebook + '.ipynb'
    image = notebook + '.png'
    cells.append(f'[<img src="{image}">]({link})')

    if len(cells) == max_cells_per_line:
        new_line()

def new_line():
    if len(cells) > 0:
        while len(cells) < max_cells_per_line:
            cells.append(' ')
        lines.append('| ' + str.join(' | ', cells) + ' |')
        cells.clear()

lines.append('1. Distribution functions:')

import matplotlib.pyplot as plt

#plt.figure(figsize=(1,1))

# overview distribution functions

start_table()

header = 'overview distribution functions'
notebook = 'overview_distribution_functions'

stochast = Stochast()
stochast.distribution = DistributionType.gumbel
stochast.scale = 2.0
stochast.shift = 3.0

plot = stochast.get_plot()

create_cell(header, notebook, plot)

# Operations on distribution functions

header = 'Operations on distribution functions'
notebook = 'operations_on_distribution_functions'

stochast = Stochast()
stochast.distribution = DistributionType.normal
stochast.location = 0.0
stochast.scale = 1.0
stochast.truncated = True
stochast.minimum = -0.5

plot = stochast.get_plot()

create_cell(header, notebook, plot)

# Composite distributions

header = 'Composite distributions'
notebook = 'composite_distribution'

w1 = 0.2
w2 = 0.8

x1 = Stochast()
x1.distribution = DistributionType.normal
x1.location = 1.0
x1.scale = 0.5

x2 = Stochast()
x2.distribution = DistributionType.normal
x2.location = 4.0
x2.scale = 0.3

stochast = Stochast()
stochast.distribution = DistributionType.composite
stochast.contributing_stochasts.append(ContributingStochast.create(w1, x1))
stochast.contributing_stochasts.append(ContributingStochast.create(w2, x2))

plot = stochast.get_plot()

create_cell(header, notebook, plot)

# Fitting with prior distribution

header = 'Fitting with prior distribution'
notebook = 'fitting_with_prior_distribution'

stochast = Stochast()
stochast.distribution = DistributionType.gumbel
stochast.scale = 2.0
stochast.shift = 3.0

plot = stochast.get_plot()

create_cell(header, notebook, plot)

# Including statistical uncertainty

header = 'Including statistical uncertainty'
notebook = 'statistical_uncertainty'

x_values = [1,2,3,4,5]
means = [12, 15, 18, 20, 21]
sigmas = [2, 2.5, 3.5, 3, 2.5]

stochast = Stochast()
stochast.distribution = DistributionType.normal
stochast.mean = 0
stochast.deviation = 1
stochast.conditional = True
stochast.conditional_source = "q"

for i in range(0, len(x_values)):
    conditional = ConditionalValue()
    conditional.x = x_values[i]
    conditional.mean = means[i]
    conditional.deviation = sigmas[i]
    stochast.conditional_values.append(conditional)

plot = stochast.get_plot()

create_cell(header, notebook, plot)

new_line()

lines.append('2. Distribution functions:')

lines.append('3. Reliability analysis of a single component:')


with open("gallery.md", "w") as file:
    file.writelines(line + '\n' for line in lines)
