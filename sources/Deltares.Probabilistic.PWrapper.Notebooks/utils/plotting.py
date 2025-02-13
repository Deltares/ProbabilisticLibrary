
import matplotlib.pyplot as plt

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