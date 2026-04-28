import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt("../../output.csv", skiprows=1)

# columns: t x0 y0 x1 y1 x2 y2 x3 y3
t = data[:, 0]
names  = ["Sun", "Earth", "Jupiter", "Halley"]
colors = ["yellow", "blue", "orange", "gray"]
sizes  = [200, 50, 100, 10]  # visual sizes for scatter

fig, axes = plt.subplots(1, 2, figsize=(16, 8), facecolor="black")

ax = axes[0]
ax.set_facecolor("black")
ax.set_title("Trajectories", color="white")

for i, (name, color) in enumerate(zip(names, colors)):
    x = data[:, 1 + i*2]
    y = data[:, 2 + i*2]
    ax.plot(x, y, color=color, linewidth=0.8, label=name)
    ax.plot(x[-1], y[-1], "o", color=color, markersize=6)

ax.legend(facecolor="black", labelcolor="white")
ax.tick_params(colors="white")
for spine in ax.spines.values():
    spine.set_edgecolor("white")

ax2 = axes[1]
ax2.set_facecolor("black")
ax2.set_title("Inner solar system (zoom)", color="white")

for i, (name, color, size) in enumerate(zip(names[:3], colors[:3], sizes[:3])):
    x = data[:, 1 + i*2]
    y = data[:, 2 + i*2]
    ax2.plot(x, y, color=color, linewidth=0.8, label=name)
    ax2.plot(x[-1], y[-1], "o", color=color, markersize=6)

ax2.set_xlim(-6, 6)
ax2.set_ylim(-6, 6)
ax2.legend(facecolor="black", labelcolor="white")
ax2.tick_params(colors="white")
for spine in ax2.spines.values():
    spine.set_edgecolor("white")

plt.tight_layout()
plt.savefig("solar_system.png", dpi=150, facecolor="black")
plt.show()
print("Saved to solar_system.png")