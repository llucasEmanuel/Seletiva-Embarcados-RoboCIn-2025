import pandas as pd

# Path do terminal deve estar em "../Seletiva RoboCIn 2025 Embarcados/"
CSV_PATH = "test/calibration/data/filtered_robot_log.csv"
log = pd.read_csv(CSV_PATH)

# Transforma a coluna do data frame em list
w_list = log["local_w"].tolist()
theta_list = log["odometry_theta"].tolist()

# Considerando que os valores iniciais correspondem ao robô parado como no dataset fornecido
# Encontra o indice da lista em que o theta != 0, ou seja ele não está mais parado
non_steady_index = 0
for i in range(len(theta_list)):
    if theta_list[i] != 0:
        non_steady_index = i
        break

soma = 0 # DEBUG

# Abre o arquivo W_PATH e sobrescreve o anterior
W_PATH = "test/calibration/data/steady_w.txt"
with open(W_PATH, "w") as file:
    # Se houver velocidade angular com o robô parado
    if non_steady_index != 0:
        # Escreve apenas as velocidades angulares de quando o robô está parado no steady_w.txt
        for i in range(non_steady_index):
            file.write(f"{w_list[i]}\n")
            soma += w_list[i] # DEBUG
        print(f"Gyroscope offset data successfully written to '{W_PATH}'")

        print(float(soma) / float(non_steady_index)) # DEBUG (calcular offset)

    # Se não houver apenas printa a mensagem, pois já sobrescreveu os valores anteriores
    else:
        print("No gyroscope offset data available in the dataset")
