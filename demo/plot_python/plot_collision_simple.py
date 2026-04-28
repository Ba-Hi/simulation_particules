import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

data = np.loadtxt("output.csv", skiprows=1)
# colonnes : t x0 y0 x1 y1 x2 y2 x3 y3

t  = data[:, 0]
xs = [data[:, 1 + i*2] for i in range(4)]
ys = [data[:, 2 + i*2] for i in range(4)]

colors = ['steelblue', 'steelblue', 'steelblue', 'tomato']
labels = ['fixe 0', 'fixe 1', 'fixe 2', 'mobile']

# ── Figure 1 : trajectoires ──────────────────────────────────
fig1, ax1 = plt.subplots(figsize=(7, 7))
for i in range(4):
    ax1.plot(xs[i], ys[i], color=colors[i], lw=0.8,
             label=labels[i] if i == 0 else ('_' if i < 3 else labels[i]))
    ax1.plot(xs[i][0],  ys[i][0],  'o', color=colors[i], ms=8, alpha=0.5)
    ax1.plot(xs[i][-1], ys[i][-1], 's', color=colors[i], ms=8)

ax1.set_title("Trajectoires")
ax1.set_xlabel("x"); ax1.set_ylabel("y")
ax1.legend()
ax1.set_aspect('equal')
ax1.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("trajectoires_simple.png", dpi=150)
print("Saved: trajectoires_simple.png")

# ── Figure 2 : snapshots 3 instants ──────────────────────────
frames = [0, len(data)//2, -1]
titres = [f"t = {t[0]:.2f}", f"t = {t[len(t)//2]:.2f}", f"t = {t[-1]:.2f}"]

fig3, axes = plt.subplots(1, 3, figsize=(14, 5))
for ax, fi, titre in zip(axes, frames, titres):
    for i in range(3):
        ax.scatter(xs[i][fi], ys[i][fi], s=300, c='steelblue', zorder=3)
    ax.scatter(xs[3][fi], ys[3][fi], s=300, c='tomato', zorder=3)
    ax.set_title(titre)
    ax.set_xlim(7, 17); ax.set_ylim(7, 17)
    ax.set_aspect('equal')
    ax.grid(True, alpha=0.3)

axes[0].scatter([], [], c='steelblue', label='fixes')
axes[0].scatter([], [], c='tomato',    label='mobile')
axes[0].legend()
plt.tight_layout()
plt.savefig("snapshots_simple.png", dpi=150)
print("Saved: snapshots_simple.png")

# ── Animation GIF ────────────────────────────────────────────
fig4, ax4 = plt.subplots(figsize=(6, 6))
ax4.set_xlim(7, 17); ax4.set_ylim(7, 17)
ax4.set_aspect('equal')
ax4.grid(True, alpha=0.3)
ax4.set_title("Animation collision simple")

scat_fix = ax4.scatter([], [], s=300, c='steelblue', label='fixes')
scat_mob = ax4.scatter([], [], s=300, c='tomato',    label='mobile')
time_txt = ax4.text(0.02, 0.95, '', transform=ax4.transAxes)
ax4.legend()

stride = max(1, len(data) // 300)

def init():
    scat_fix.set_offsets(np.empty((0, 2)))
    scat_mob.set_offsets(np.empty((0, 2)))
    return scat_fix, scat_mob, time_txt

def update(frame):
    fi = frame * stride
    fix_xy = np.column_stack([[xs[i][fi] for i in range(3)],
                               [ys[i][fi] for i in range(3)]])
    scat_fix.set_offsets(fix_xy)
    scat_mob.set_offsets([[xs[3][fi], ys[3][fi]]])
    time_txt.set_text(f't = {t[fi]:.3f}')
    return scat_fix, scat_mob, time_txt

n_frames = len(data) // stride
ani = animation.FuncAnimation(fig4, update, frames=n_frames,
                               init_func=init, blit=True, interval=30)
ani.save("collision_simple.gif", writer='pillow', fps=30)
print("Saved: collision_simple.gif")

plt.show()