import numpy as np

## load data from file
u = np.loadtxt("./results/velocity_u.csv", delimiter=",")
v = np.loadtxt("./results/velocity_v.csv", delimiter=",")

tol = 1e-12 # defining precision value

# display BC message
print("=" * 40)
print("Boundary Condition Verification")
print("=" * 40)

# u BC
top_u = np.max(np.abs(u[:, -1] - 1.0))
bottom_u = np.max(np.abs(u[:, 0]))
left_u = np.max(np.abs(u[0, 1: -1]))
right_u = np.max(np.abs(u[-1, 1: -1]))

# v BC
top_v = np.max(np.abs(v[:, -1]))
bottom_v = np.max(np.abs(v[:, 0]))
left_v = np.max(np.abs(v[0, :]))
right_v = np.max(np.abs(v[-1, :]))

errors = [
    top_u,
    bottom_u,
    left_u,
    right_u,
    top_v,
    bottom_v,
    left_v,
    right_v,
]

labels = [
    ("Top wall u", top_u),
    ("Bottom wall u", bottom_u),
    ("Left wall u", left_u),
    ("Right wall u", right_u),
    ("Top wall v", top_v),
    ("Bottom wall v", bottom_v),
    ("Left wall v", left_v),
    ("Right wall v", right_v),
]

for name, err in labels:
    status = "PASS" if err < tol else "FAIL"
    print(f"{name:20s}: {status}    Error = {err:.3e}")

print()
print(f"Maximum BC Error : {max(errors):.3e}")

if max(errors) < tol:
    print("\nBoundary conditions verified successfully.")
else:
    print("\nBoundary conditions FAILED.")