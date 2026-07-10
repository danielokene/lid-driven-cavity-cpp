import numpy as np
import matplotlib.pyplot as plt

speed = np.loadtxt("results/velocity_magnitude.csv", delimiter=",")

N = speed.shape[0]

x = np.linspace(0,1,N)
y = np.linspace(0,1,N)

X,Y = np.meshgrid(x,y,indexing="ij")

plt.figure(figsize=(7,7))

plt.contourf(X,Y,speed,50,cmap="viridis")

plt.colorbar(label="Speed")

plt.title("Velocity Magnitude")

plt.axis("equal")

plt.tight_layout()

plt.savefig(
    "plots/velocity_plot.png",
    dpi=300,
    bbox_inches="tight"
)

print("Saved plots/velocity_plot.png")