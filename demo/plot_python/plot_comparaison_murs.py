import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv('comparaison_murs.csv')

plt.figure(figsize=(10, 8))

# Graphe de Position
plt.subplot(2, 1, 1)
plt.plot(df['t'], df['x_cin'], label='Cinématique (Mur dur)', color='red', linestyle='--')
plt.plot(df['t'], df['x_pot'], label='Potentiel (Mur doux)', color='blue')
plt.axhline(0, color='black', linewidth=1, label='Paroi (x=0)')
plt.ylabel('Position X')
plt.title('Comparaison des Conditions aux Limites : Position et Vitesse')
plt.legend()
plt.grid(True)

# Graphe de Vitesse
plt.subplot(2, 1, 2)
plt.plot(df['t'], df['v_cin'], label='Cinématique (Mur dur)', color='red', linestyle='--')
plt.plot(df['t'], df['v_pot'], label='Potentiel (Mur doux)', color='blue')
plt.xlabel('Temps (s)')
plt.ylabel('Vitesse X')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('comparaison_murs.png')
plt.show()
