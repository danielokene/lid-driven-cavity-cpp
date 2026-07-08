from common import *

import numpy as np
import matplotlib.pyplot as plt

## calling the set plot style function
setPlotStyle()

## loading the csv file
data = np.loadtxt(
    RESULTS_DIR / "residual_history.csv",
    delimiter=",",
    skiprows=1  # skips the first header row
)

## arranging the data 
iterations = data[:, 0]
residual = data[:, 1]

## setting up the plot figure
plt.figure()
plt.semilogy(
    iterations,
    residual,
    linewidth=2
)

## labels and title
plt.xlabel("Iteration")
plt.ylabel("Velocity Residuals")
plt.title("SOlver Convergence History")

plt.grid(True)
saveFigure("residual.png")
plt.show()

## annotating the final residual value
plt.text(
    iterations[-1],
    residual[-1],
    f"{residual[-1]:.2e}"
)