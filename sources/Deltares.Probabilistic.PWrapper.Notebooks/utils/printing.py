
from probabilistic_library import StandardNormal, ReliabilityMethod

# print convergence information and model runs
def print_conv(dp, project):

    if dp.is_converged:
        print(f"Converged (convergence = {dp.convergence} < {project.settings.variation_coefficient})")
    else:
        print(f"Not converged (convergence = {dp.convergence} > {project.settings.variation_coefficient})")
    
    print(f"Model runs = {dp.total_model_runs}")

# print design point
def print_results(project):

    dp = project.design_point
    beta = dp.reliability_index

    print(f"Beta = {beta}")

    pf = StandardNormal.get_q_from_u(beta)
    print(f"Probability of failure = {pf}")

    for alpha in dp.alphas:
        print(f"{alpha.variable.name}: alpha = {alpha.alpha}, x = {alpha.x}")

    print_conv(dp, project)

# print for hybrid methods
def print_hybrid(project):

    dp = project.design_point
    beta = dp.reliability_index

    print(f"Beta = {beta}")

    pf = StandardNormal.get_q_from_u(beta)
    print(f"Probability of failure = {pf}")

    for alpha in dp.alphas:
        print(f"{alpha.variable.name}: alpha = {alpha.alpha}, x = {alpha.x}")

    if project.settings.reliability_method == ReliabilityMethod.directional_sampling_then_form:
        print("Direction sampling step:")
        print_conv(dp.contributing_design_points[0], project)
        print("FORM step:")
        print_conv(dp.contributing_design_points[1], project)
    else:
        print_conv(dp, project)

# print input and output variables
def print_input_output_var(project):

    print("Input parameters:")
    for input_parameter in project.model.input_parameters:
        print(input_parameter)
            
    print("")
    print("Output parameters:")
    for output_parameter in project.model.output_parameters:
        print(output_parameter)