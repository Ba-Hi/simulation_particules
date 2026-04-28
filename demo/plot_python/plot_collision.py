# test/plot_collision.py
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

data = np.loadtxt("./output_tp4.csv", skiprows=1)
n_particules = 8000
n_rect = 6400  # catégorie 0
n_sq   = 1600  # catégorie 1

# Prendre 3 instants : initial, milieu, final
frames = [0, len(data)//2, -1]
fig, axes = plt.subplots(1, 3, figsize=(18, 6))
titres = ["t = 0 (initial)", f"t ≈ {19.5/2:.1f}", "t = 19.5 (final)"]

for ax, fi, titre in zip(axes, frames, titres):
    row = data[fi]
    # colonnes : t x0 y0 x1 y1 ...
    xs = row[1::2]
    ys = row[2::2]
    ax.scatter(xs[:n_rect], ys[:n_rect], s=0.5, c='blue',  label='rectangle')
    ax.scatter(xs[n_rect:], ys[n_rect:], s=0.5, c='red',   label='carré')
    ax.set_title(titre)
    ax.set_aspect('equal')
    ax.legend(markerscale=10)

plt.tight_layout()
plt.savefig("collision.png", dpi=150)
plt.show()