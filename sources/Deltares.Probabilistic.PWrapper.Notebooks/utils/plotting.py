
import matplotlib.pyplot as plt
import numpy as np
from probabilistic_library import StandardNormal

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

# plot results for the linear a, b function
def plot_linear_a_b(project):

    from utils.models import linear_a_b

    r_1 = [realization.input_values[0] for realization in project.design_point.realizations]
    r_2 = [realization.input_values[1] for realization in project.design_point.realizations]
    
    a = np.arange(0.8, 1.1, 0.001)
    b = np.arange(0.8, 1.1, 0.001)
    z = [linear_a_b(val_a, val_b) for val_a in a for val_b in b]
    z = np.array(z).reshape(len(a), len(b))
    a, b = np.meshgrid(a, b)

    plt.figure()
    plt.contourf(a, b, z, levels=50)
    plt.colorbar()

    plt.scatter(r_1, r_2, label="realizations", color="gray")
    plt.scatter(project.design_point.alphas["a"].x, project.design_point.alphas["b"].x, label="design point", color="black")
    plt.xlim([min(r_1)-0.1, max(r_1)+0.1])
    plt.ylim([min(r_2)-0.1, max(r_2)+0.1])
    plt.xlabel("a")
    plt.ylabel("b")
    plt.legend()
    plt.title("Z-function values", fontsize=14, fontweight='bold')