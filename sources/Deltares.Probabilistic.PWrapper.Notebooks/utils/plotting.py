
import matplotlib.pyplot as plt
import numpy as np
from probabilistic_library import StandardNormal, SensitivityProject, SensitivityMethod, DistributionType

# plot distribution
def plot_dist(val_grid, stochast):

    pdf = [stochast.get_pdf(val) for val in val_grid]
    cdf = [stochast.get_cdf(val) for val in val_grid]
    
    fig, ax1 = plt.subplots()
    color = "tab:blue"
    ax1.set_xlabel("value (-)")
    ax1.set_ylabel("pdf (-)", color=color)
    ax1.plot(val_grid, pdf)
    ax1.tick_params(axis="y", labelcolor=color)
    ax2 = ax1.twinx()
    color = "tab:red"
    ax2.set_ylabel("cdf (-)", color=color)
    ax2.plot(val_grid, cdf, "r--", label="pdf")
    ax2.tick_params(axis="y", labelcolor=color)

# plot piechart of alpha values
def piechart_alphas(project):

    alphas = [project.design_point.alphas[val.name].alpha**2 for val in project.model.input_parameters]
    names = [val.name for val in project.model.input_parameters]
    
    plt.figure()
    plt.pie(alphas, labels=names, autopct='%1.2f%%')
    plt.title("alpha^2", fontsize=14, fontweight='bold')

# plot realizations when at least 2 random variables
def plot_realizations_2d(project):

    n = len(project.design_point.realizations)
    z = [project.design_point.realizations[id].z for id in range(n)]

    # 2 variables with the highest alpha
    alphas = [project.design_point.alphas[val.name].alpha for val in project.model.input_parameters]
    index_last_two = np.argsort(np.abs(alphas))[-2:]

    r_1 = [realization.input_values[int(index_last_two[0])] for realization in project.design_point.realizations]
    r_2 = [realization.input_values[int(index_last_two[1])] for realization in project.design_point.realizations]
    colors = ["r" if val < 0 else "g" for val in z]

    # plot realizations
    plt.figure()
    plt.grid(True)    
    plt.scatter(r_1, r_2, color=colors, alpha=0.5)
    plt.scatter(project.design_point.alphas[int(index_last_two[0])].x, 
                project.design_point.alphas[int(index_last_two[1])].x, 
                label="design point", 
                color="black")
    plt.xlabel(project.model.input_parameters[int(index_last_two[0])].name)
    plt.ylabel(project.model.input_parameters[int(index_last_two[1])].name)
    plt.legend()
    plt.title("Realizations: Red = Failure, Green = No Failure", fontsize=14, fontweight='bold')

# plot realizations when at least 3 random variables
def plot_realizations_3d(project):

    n = len(project.design_point.realizations)
    z = [project.design_point.realizations[id].z for id in range(n)]

    # 3 variables with the highest alpha
    alphas = [project.design_point.alphas[val.name].alpha for val in project.model.input_parameters]
    index_last_three = np.argsort(np.abs(alphas))[-3:]

    r_1 = [realization.input_values[int(index_last_three[0])] for realization in project.design_point.realizations]
    r_2 = [realization.input_values[int(index_last_three[1])] for realization in project.design_point.realizations]
    r_3 = [realization.input_values[int(index_last_three[2])] for realization in project.design_point.realizations]
    colors = ["r" if val < 0 else "g" for val in z]

    # plot realizations
    fig = plt.figure()
    ax = fig.add_subplot(projection='3d')
    ax.grid(True)    
    ax.scatter(r_1, r_2, color=colors, alpha=0.5)
    ax.scatter(project.design_point.alphas[int(index_last_three[0])].x, 
                project.design_point.alphas[int(index_last_three[1])].x, 
                project.design_point.alphas[int(index_last_three[2])].x,
                label="design point", 
                color="black")
    ax.set_xlabel(project.model.input_parameters[int(index_last_three[0])].name)
    ax.set_ylabel(project.model.input_parameters[int(index_last_three[1])].name)
    ax.set_zlabel(project.model.input_parameters[int(index_last_three[2])].name)
    #ax.set_legend()
    ax.set_title("Realizations: Red = Failure, Green = No Failure", fontsize=14, fontweight='bold')

# plot beta and failure probability
def plot_beta_prob(project):

    n = len(project.design_point.realizations)
    z = [project.design_point.realizations[id].z for id in range(n)]
    beta = [project.design_point.realizations[id].beta for id in range(n)]

    prob = [1-StandardNormal.get_p_from_u(val) for val in beta]

    plt.figure()
    plt.plot(beta)
    plt.xlabel("model run (-)")
    plt.ylabel("beta (-)")
    plt.grid()
    plt.title("beta values", fontsize=14, fontweight='bold')

    plt.figure()
    plt.plot(prob)
    plt.xlabel("model run (-)")
    plt.ylabel("failure probability (-)")
    plt.grid()
    plt.title("failure probability", fontsize=14, fontweight='bold')

def evaluate_one_value(sens, key_var, val):

    sens.variables[key_var].distribution = DistributionType.deterministic
    sens.variables[key_var].mean = val
    sens.run()
    return sens.stochast.mean

def find_Z_0(project, f, given_var, given_val, find_var, start_int, end_int, tol=1e-5, max_iter=100):

    sens = SensitivityProject()
    sens.model = f

    sens.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
    sens.settings.minimum_samples = 1
    sens.settings.maximum_samples = 1

    for key in project.model.input_parameters:
        if key.name not in [project.model.input_parameters[given_var].name, project.model.input_parameters[find_var].name]:
            sens.variables[key.name].distribution = DistributionType.deterministic
            sens.variables[key.name].mean = project.design_point.alphas[key.name].x
        elif key.name==project.model.input_parameters[given_var].name:
            sens.variables[key.name].distribution = DistributionType.deterministic
            sens.variables[key.name].mean = given_val

    f_start_int = evaluate_one_value(sens, find_var, start_int)
    f_end_int = evaluate_one_value(sens, find_var, end_int)

    if f_start_int * f_end_int >= 0:
        print("The function must have different signs at the endpoints a and b.")
        return np.nan

    for _ in range(max_iter):
        c = (start_int + end_int) / 2
        f_c = evaluate_one_value(sens, find_var, c)
        if f_c == 0 or (end_int - start_int) / 2 < tol:
            return c
        if f_c * f_start_int < 0:
            end_int = c
        else:
            start_int = c

    return (start_int + end_int) / 2

def plot_Z_0(project, f, given_var, given_val_range, find_var, start_int, end_int):

    find_val = [find_Z_0(project, f, given_var, val, find_var, start_int, end_int, tol=1e-5, max_iter=100) for val in given_val_range]

    x_given_var = project.design_point.alphas[given_var].x
    x_find_var = project.design_point.alphas[find_var].x

    other_dp = []
    for key in project.model.input_parameters:
        if key.name not in [given_var, find_var]:
            other_dp.append(f"{key.name} = {round(project.design_point.alphas[key.name].x, 2)}")

    plt.figure()
    plt.plot(given_val_range, find_val, label="Z=0")
    plt.scatter(x_given_var, x_find_var, label="design point", color="black")
    plt.xlabel(given_var)
    plt.ylabel(find_var)
    plt.legend()
    plt.title(other_dp)
    plt.grid()