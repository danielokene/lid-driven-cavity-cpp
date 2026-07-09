import numpy as np

## load data from file
u = np.loadtxt("./results/velocity_u.csv", delimiter=",")
v = np.loadtxt("./results/velocity_v.csv", delimiter=",")

N = u.shape[0]

length = 1.0
height = 1.0

dx = length / (N - 1)
dy = height / (N - 1)

# compute divergence
divergence = np.zeros_like(u)

for i in range(1, N - 1):
    for j in range(1, N - 1):
        dudx = (u[i + 1, j] - u[i - 1, j]) / (2 * dx)
        dvdy = (v[i, j + 1] - v[i, j - 1]) / (2 * dy)

        divergence[i, j] = dudx + dvdy

print("Location of max divergence:")
idx = np.unravel_index(np.argmax(np.abs(divergence)), divergence.shape)
print(idx)
print("Value =", divergence[idx])


# Display max div, mean and RMS
interior = divergence[3:-3, 3:-3]

print("=" * 45)
print("Continuity Verification")
print("=" * 45)

print(f"Maximum |div| : {np.max(np.abs(interior)):.6e}")
print(f"Mean |div|    : {np.mean(np.abs(interior)):.6e}")
print(f"RMS  |div|    : {np.sqrt(np.mean(interior**2)):.6e}")