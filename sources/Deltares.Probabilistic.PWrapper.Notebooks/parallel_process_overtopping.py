# In this example, we demonstrate how parallel computing - a method of performing multiple calculations 
# simultaneously to improve efficiency - is applied to estimate the probability of levee failure due to wave overtopping. 

# First, we import the necessary packages:

from probabilistic_library import ReliabilityProject, DistributionType, ReliabilityMethod
import time

# We consider the limit state function for wave overtopping, which was artificially slowed down:

from utils.models import z_func_overtopping_sleep

# And the following reliability project:

def define_project():

    project = ReliabilityProject()
    project.model = z_func_overtopping_sleep

    project.variables["h"].distribution = DistributionType.log_normal
    project.variables["h"].mean = 1.5
    project.variables["h"].deviation = 0.05

    project.variables["hm0"].distribution = DistributionType.log_normal
    project.variables["hm0"].mean = 1.5
    project.variables["hm0"].deviation = 0.25

    project.variables["tm10"].distribution = DistributionType.log_normal
    project.variables["tm10"].mean = 3
    project.variables["tm10"].deviation = 0.5

    project.variables["wave_direction"].distribution = DistributionType.deterministic
    project.variables["wave_direction"].mean = 0.0

    project.variables["dike_normal"].distribution = DistributionType.deterministic
    project.variables["dike_normal"].mean = 0.0

    project.variables["y_crest"].distribution = DistributionType.deterministic
    project.variables["y_crest"].mean = 6.0

    project.variables["q_crit"].distribution = DistributionType.log_normal
    project.variables["q_crit"].mean = 0.001
    project.variables["q_crit"].deviation = 0.01

    project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo
    project.settings.minimum_samples = 1000
    project.settings.maximum_samples = 1000
    project.settings.variation_coefficient = 0.02

    return project

# We perform the reliability calculations using the `crude_monte_carlo` method:

project = define_project()

project.run()

from utils.printing import print_results
print_results(project.design_point, project.settings)

# If not specified, the calculations are performed on a single processor. To utilize multiple processors, 
# we adjust the `setting project.settings.max_parallel_process`:

max_parallel_process = [1, 4, 8]

for val in max_parallel_process:

    project = define_project()
    project.settings.max_parallel_process = val

    t = time.time()
    project.run()
    elapsed = time.time() - t

    print(f"Max parallel process: {val}")
    print(f"Reliability index: {project.design_point.reliability_index}")
    print(f"Time elapsed: {elapsed} seconds")
    print(f"----------------------------------------------------------------")