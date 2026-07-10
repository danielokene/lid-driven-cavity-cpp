import numpy as np
import matplotlib.pyplot as plt

p = np.loadtxt("results/pressure.csv", delimiter=",")

N = p.shape[0]

x = np.linspace(0,1,N)
y = np.linspace(0,1,N)

X,Y = np.meshgrid(x,y,indexing="ij")

plt.figure(figsize=(7,7))

plt.contourf(X,Y,p,40,cmap="coolwarm")

plt.colorbar(label="Pressure")

plt.title("Pressure")

plt.axis("equal")

plt.tight_layout()

plt.tight_layout()

plt.savefig(
    "plots/pressure_plot.png",
    dpi=300,
    bbox_inches="tight"
)

print("Saved plots/pressure_plot.png")
