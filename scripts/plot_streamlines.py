import numpy as np
import matplotlib.pyplot as plt

u = np.loadtxt("results/velocity_u.csv", delimiter=",")
v = np.loadtxt("results/velocity_v.csv", delimiter=",")

N = u.shape[0]

x = np.loadtxt("results/x.csv")
y = np.loadtxt("results/y.csv")

X,Y = np.meshgrid(x, y)

speed = np.sqrt(u**2 + v**2)

plt.figure(figsize=(7,7))

plt.streamplot(
    X[0, :],
    Y[:, 0],
    u.T,
    v.T,
    density=2,
    color=speed.T,
    linewidth=1
)

plt.colorbar(label="Velocity magnitude")

plt.title("Lid Driven Cavity")

plt.xlabel("x")
plt.ylabel("y")

plt.axis("equal")

plt.tight_layout()

plt.savefig(
    "plots/streamline_plot.png",
    dpi=300,
    bbox_inches="tight"
)

print("Saved plots/streamline_plot.png")
