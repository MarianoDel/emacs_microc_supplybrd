import matplotlib.pyplot as plt
import numpy as np

# matplotlib con orientacion a objetos (debe usar .subplots())

#####################################
# 1x1 default una fila una columna  #
#####################################
# fig, ax = plt.subplots()

# x = [1, 2, 3, 4]
# y = [10, 11, 12, 14]

# ax.plot(x, y, color='blue', marker='o')
# ax.set_title('grafico de una foto')

# plt.show()


#############################
# 1x2 una fila dos columnas #
#############################
# axs es una lista de indices a graficos
# fig, axs = plt.subplots(1,2)

# x = [1, 2, 3, 4]
# y1 = [1, 4, 9, 16]
# y2 = [16, 9, 4, 1]

# # grafico izquierda indice 0
# axs[0].plot(x, y1, color='green', marker='s')
# axs[0].set_title('Grafico Izquierda[0]')

# # grafico derecha indice 1
# axs[1].bar(x, y2, color='orange')
# axs[1].set_title('Grafico Derecha[1]')

# #ajustar a hoja
# plt.tight_layout()
# plt.show()


###########################################################
# 1x2 una fila dos columnas con desempaquetado automatico #
###########################################################
# # ax... es el indice de cada grafico
# fig, (ax_izq, ax_der) = plt.subplots(1,2)

# x = [1, 2, 3, 4]
# y1 = [1, 4, 9, 16]
# y2 = [16, 9, 4, 1]

# # grafico izquierda
# ax_izq.plot(x, y1, color='green', marker='s')
# ax_izq.set_title('Grafico Izquierda ax_izq')

# # grafico derecha
# ax_der.bar(x, y2, color='orange')
# ax_der.set_title('Grafico Derecha ax_der')

# #ajustar a hoja
# plt.tight_layout()
# plt.show()


###########################################################################
# Generar un espacio lineal equidistante (inicio, final, cant de valores) #
###########################################################################
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


##############################
# Dos curvas mismo grafico - #
##############################
# x = np.linspace(0, 10, 100)
# y = np.sin(x)

# fig, ax = plt.subplots()

# x_dif = x - 0.5
# ax.plot(x, y, label='Seno de X', color='#2ca02c', linewidth=2)
# ax.plot(x_dif, y, label='Offset', color='orange', linewidth=2)

# # A magenta, dashed line with a width of 2
# # ax.axvline(x=2.5, color='red', linestyle='--', linewidth=2)
# ax.axvline(x=2.5, color='magenta', linewidth=2)

# # personalizar ejes
# ax.set_title('Grafico Onda Senoidal', fontsize=14, fontweight='bold')
# ax.set_xlabel('Eje de Tiempo', fontsize=12)
# ax.set_ylabel('Amplitud', fontsize=12)
# ax.grid(True, linestyle='--', alpha=0.6)
# ax.legend(loc='upper right')

# plt.show()


##########################################################################
# Dos curvas mismo grafico con doble eje Y izq y der (ax2 = ax1.twinx()) #
##########################################################################
# # 1. Generate sample data
# x = np.linspace(0, 10, 100)
# y1 = np.sin(x)          # Data for the left Y-axis
# y2 = np.exp(x / 3)      # Data for the right Y-axis

# # 2. Initialize the figure and the primary (left) axes
# fig, ax1 = plt.subplots()

# # Plot data on the left axis
# color1 = 'tab:blue'
# ax1.set_xlabel('X Axis Label')
# ax1.set_ylabel('Left Y-Axis (Sine)', color=color1)
# ax1.plot(x, y1, color=color1, linewidth=2, label='Sine')
# ax1.tick_params(axis='y', labelcolor=color1)

# # 3. Create the secondary (right) axes sharing the same x-axis
# ax2 = ax1.twinx()

# # Plot data on the right axis
# color2 = 'tab:red'
# ax2.set_ylabel('Right Y-Axis (Exponential)', color=color2)
# ax2.plot(x, y2, color=color2, linewidth=2, linestyle='--', label='Exponential')
# ax2.tick_params(axis='y', labelcolor=color2)

# # 4. Refine layout and display
# fig.tight_layout()  # Prevents labels from getting clipped
# plt.show()


####################################################################
# Dos curvas mismo grafico con doble eje Y izq (ax2 = ax1.twinx()) #
####################################################################
# # 1. Generate mock data
# x = np.linspace(0, 10, 100)
# y1 = np.sin(x)            # Primary data (Scale 1)
# y2 = np.exp(x / 3) * 100  # Secondary data (Scale 2)

# # 2. Create the figure and primary axis (Left Axis #1)
# fig, ax1 = plt.subplots(figsize=(8, 5))
# fig.subplots_adjust(left=0.25)  # Make room on the left for the second axis

# # Plot primary data
# color1 = 'tab:blue'
# ax1.plot(x, y1, color=color1, linewidth=2, label="Sine")
# ax1.set_xlabel("X Axis")
# ax1.set_ylabel("Primary Y (Left 1)", color=color1)
# ax1.tick_params(axis='y', labelcolor=color1)

# # 3. Create the twin axis (Initially placed on the right side)
# ax2 = ax1.twinx()

# # 4. Move the twin axis to the left side and offset it
# ax2.spines["left"].set_position(("axes", -0.2)) # Push 20% further left of the plot
# ax2.spines["left"].set_visible(True)            # Ensure the spine line shows
# ax2.yaxis.set_label_position("left")            # Move text label to the left
# ax2.yaxis.set_ticks_position("left")            # Move tick marks to the left

# # Plot secondary data
# color2 = 'tab:red'
# ax2.plot(x, y2, color=color2, linewidth=2, linestyle="--", label="Exponential")
# ax2.set_ylabel("Secondary Y (Left 2)", color=color2)
# ax2.tick_params(axis='y', labelcolor=color2)

# # 5. Handle the right-hand side spine (Optional clean up)
# # twinx() automatically duplicates the right spine; hide it if you want it blank
# ax1.spines["right"].set_visible(False)
# ax2.spines["right"].set_visible(False)

# plt.title("Double Vertical Left Axes Example", pad=15)
# plt.show()


###################################
# Dos graficos comparten el eje Y #
###################################
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


########################
# Grafico de torta Pie #
########################
# datos
generos = ['Rock', 'Pop', 'Hip Hop', 'Electronica', 'Otros']
votos = [35, 25, 20, 15, 5]

# colores personalizados en hex o con nombres
colores = ['#ff9999', '#66b3ff', '#99ff99', '#ffcc99', '#c2c2f0']

# hacer explode para la primer rebanada
separacion = [0.1, 0, 0, 0, 0]

fig, ax = plt.subplots()

# crear el grafico de torta
ax.pie(
    votos,
    labels=generos,
    autopct='%1.1f%%',    # porcentaje con un decimal
    startangle=90,    # rota el inicio del grafico a 90grados (centro arriba)
    colors=colores,
    explode=separacion,
    shadow=True    # sombra para 3D
    )
ax.legend(
    labels=generos,
    title='Generos Musicales',
    loc='upper right',
    # bbox_to_anchor=(1, 0, 0.5, 0)    # mueve la leyenda fuera del circulo
    )

ax.set_title('Generos de Musica', fontsize=14, fontweight='bold')

plt.show()


    
    
