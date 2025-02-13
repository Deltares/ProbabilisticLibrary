
from probabilistic_library import StandardNormal, ReliabilityMethod

# print design point
def print_results(dp, project):

    beta = dp.reliability_index

    print(f"Beta = {beta}")

    pf = StandardNormal.get_q_from_u(beta)
    print(f"Probability of failure = {pf}")

    for alpha in dp.alphas:
        print(f"{alpha.variable.name}: alpha = {alpha.alpha}, x = {alpha.x}")

    if dp.is_converged:
        print(f"Converged (convergence = {dp.convergence} < {project.settings.variation_coefficient})")
    else:
        print(f"Not converged (convergence = {dp.convergence} > {project.settings.variation_coefficient})")
        
    print(f"Model runs = {dp.total_model_runs}")

# print convergence information and model runs
def print_statistics(dp, project):
    if dp.is_converged:
        print(f"Converged (convergence = {dp.convergence} < {project.settings.variation_coefficient})")
    else:
        print(f"Not converged (convergence = {dp.convergence} > {project.settings.variation_coefficient})")
    
    print(f"Model runs = {dp.total_model_runs}")

# print for hybrid methods
def print_hybrid(dp, project):

    beta = dp.reliability_index

    print(f"Beta = {beta}")

    pf = StandardNormal.get_q_from_u(beta)
    print(f"Probability of failure = {pf}")

    for alpha in dp.alphas:
        print(f"{alpha.variable.name}: alpha = {alpha.alpha}, x = {alpha.x}")

    if project.settings.reliability_method == ReliabilityMethod.directional_sampling_then_form:
        print("Direction sampling step:")
        print_statistics(dp.contributing_design_points[0], project)
        print("FORM step:")
        print_statistics(dp.contributing_design_points[1], project)
    else:
        print_statistics(dp, project)