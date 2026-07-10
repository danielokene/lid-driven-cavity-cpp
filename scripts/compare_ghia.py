import numpy as np
import matplotlib.pyplot as plt


# Load simulation results
u = np.loadtxt("results/velocity_u.csv", delimiter=",")
v = np.loadtxt("results/velocity_v.csv", delimiter=",")

x = np.loadtxt("results/x.csv")
y = np.loadtxt("results/y.csv")


# Extract centerlines
mid = len(x) // 2

# u along vertical centreline (x = 0.5)
u_center = u[mid, :]

# v along horizontal centreline (y = 0.5)
v_center = v[:, mid]


# Load Ghia benchmark
ghia_u = np.loadtxt(
    "validation/ghia_Re100_u_centerline.csv",
    delimiter=",",
    skiprows=1
)

ghia_v = np.loadtxt(
    "validation/ghia_Re100_v_centerline.csv",
    delimiter=",",
    skiprows=1
)

# columns

ghia_y = ghia_u[:,0]
ghia_uvel = ghia_u[:,1]

ghia_x = ghia_v[:,0]
ghia_vvel = ghia_v[:,1]


# Plot
fig, ax = plt.subplots(1,2, figsize=(12,5))


# u velocity
ax[0].plot(
    u_center,
    y,
    lw=2,
    label="Present Solver"
)

ax[0].scatter(
    ghia_uvel,
    ghia_y,
    color="red",
    marker="o",
    s=25,
    label="Ghia et al. (1982)"
)

ax[0].set_xlabel("u")
ax[0].set_ylabel("y")

ax[0].set_title("Vertical Centreline")

ax[0].grid(True)

ax[0].legend()

# v velocity
ax[1].plot(
    x,
    v_center,
    lw=2,
    label="Present Solver"
)

ax[1].scatter(
    ghia_x,
    ghia_vvel,
    color="red",
    marker="o",
    s=25,
    label="Ghia et al. (1982)"
)

ax[1].set_xlabel("x")
ax[1].set_ylabel("v")

ax[1].set_title("Horizontal Centreline")

ax[1].grid(True)

ax[1].legend()

plt.tight_layout()

plt.savefig(
    "plots/ghia_validation.png",
    dpi=300,
    bbox_inches="tight"
)

print("Saved plots/ghia_validation.png")