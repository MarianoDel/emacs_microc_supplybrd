import matplotlib.pyplot as plt
import numpy as np

#####################
# UN CANAL DESCARGA #
#####################
# mediciones 1ch en uso otro fadding
# horario expresado en minutos, sin diferencia
x_raw = [35, 60, 90, 120, 150, 180, 210, 240, 275, 322, 396, 421, 451, 481, 511, 562, 572, 577, 578, 579]

# porcentaje restante de bateria
y_pct = [73, 72, 68,  66,  63,  62,  61,  58,  58,  57,  53,  52,  51,  48,  47,  36,  22,  13,   9,   8]

# convierto a numpy array y aplico la diferencia de minutos inicial
x_dif = np.array(x_raw) - 35
# offset temporal
# x_dif = np.array(x_dif) + 150
# print(x_dif)

# convierto a numpy array
y_pct = np.array(y_pct)

fig, ax = plt.subplots()

ax.plot(x_dif, y_pct, label='1Ch 1fadding', color='#2ca02c', linewidth=2)

# personalizar ejes
ax.set_title('Descarga de Bateria', fontsize=14, fontweight='bold')
ax.set_xlabel('minutos', fontsize=12)
ax.set_ylabel('%Bateria', fontsize=12)
ax.grid(True, linestyle='--', alpha=0.6)
# ax.legend(loc='upper right')

# A magenta, dashed line with a width of 2
# ax.axvline(x=2.5, color='red', linestyle='--', linewidth=2)
ax.axvline(x=360, color='magenta', linewidth=2)

# plt.show()


########################
# DOS CANALES DESCARGA #
########################
x2_raw = [34, 40, 60, 151, 229, 289, 349, 464, 506, 566, 587, 614, 638, 647, 651, 657, 661, 663, 667]

# porcentaje restante de bateria
y2_pct = [97, 95, 88,  76,  69,  63,  58,  56,  53,  51,  48,  47,  44,  41,  38,  34,  31,  28,  23]

# convierto a numpy array y aplico la diferencia de minutos inicial
x2_dif = np.array(x2_raw) - 34
# print(x_dif)

# convierto a numpy array
y2_pct = np.array(y2_pct)

# fig, ax = plt.subplots()

# ax.plot(x2_dif, y2_pct, label='Bateria Restante', color='#2ca02c', linewidth=2)
ax.plot(x2_dif, y2_pct, label='2Ch', color='orange', linewidth=2)
# ax.legend(loc='upper right')
# personalizar ejes
# ax.set_title('Descarga de Bateria 1Ch', fontsize=14, fontweight='bold')
# ax.set_xlabel('minutos', fontsize=12)
# ax.set_ylabel('%Bateria', fontsize=12)
# ax.grid(True, linestyle='--', alpha=0.6)
# ax.legend(loc='upper right')

# # A magenta, dashed line with a width of 2
# # ax.axvline(x=2.5, color='red', linestyle='--', linewidth=2)
# ax.axvline(x=360, color='magenta', linewidth=2)


###########################
# CUATRO CANALES DESCARGA #
###########################
x4_raw = [ 0, 30, 60, 90, 120, 150, 180, 216, 240, 270, 300, 330, 360, 391, 420, 450, 462, 465, 467, 470]

# porcentaje restante de bateria
y4_pct = [95, 88, 83, 79,  73,  69,  66,  62,  61,  58,  57,  53,  52,  49,  47,  43,  38,  36,  33,  31]

# convierto a numpy array
y4_pct = np.array(y4_pct)

ax.plot(x4_raw, y4_pct, label='4Ch', color='blue', linewidth=2)
ax.legend(loc='upper right')

plt.show()




# # Generar un espacio lineal equidistante (inicio, final, cant de valores)
# x = np.linspace(0, 10, 100)
# y = np.sin(x)

# fig, ax = plt.subplots()

# ax.plot(x, y, label='Seno de X', color='#2ca02c', linewidth=2)

# # personalizar ejes
# ax.set_title('Grafico Onda Senoidal', fontsize=14, fontweight='bold')
# ax.set_xlabel('Eje de Tiempo', fontsize=12)
# ax.set_ylabel('Amplitud', fontsize=12)
# ax.grid(True, linestyle='--', alpha=0.6)
# ax.legend(loc='upper right')

# plt.show()


# # Dos graficos comparten el eje Y
# x = np.linspace(0, 10, 100)
# y1 = np.sin(x)
# y2 = np.sin(2*x)

# fig, axs = plt.subplots(1,2,sharex=True, sharey=True)

# # grafico izquierda indice 0
# axs[0].plot(x, y1, color='blue')
# axs[0].set_title('Frecuencia 1')
# axs[0].grid(True, linestyle=':')

# # grafico derecha indice 1
# axs[1].plot(x, y2, color='green')
# axs[1].set_title('Frecuencia 2')
# axs[1].grid(True, linestyle=':')

# # Titulo global de la figura
# fig.suptitle('Analisis de Frecuencias 1x2', fontsize=16, fontweight='bold')

# #ajustar a hoja
# plt.tight_layout()
# plt.show()

