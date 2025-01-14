import pandas as pd

# Path do terminal deve estar em "../Seletiva RoboCIn 2025 Embarcados/"
CSV_PATH = "test/calibration/data/filteredRobotLog.csv"
log = pd.read_csv(CSV_PATH)

# Transforma a coluna do data frame em list
w_list = log["local_w"].tolist()
theta_list = log["odometry_theta"].tolist()

# Encontra o indice da lista em que o theta != 0, ou seja ele não está mais parado
non_steady_index = 0
for i in range(len(theta_list)):
    if theta_list[i] != 0:
        non_steady_index = i
        break

# Escreve apenas as velocidades angulares de quando o robô está parado no steady_w.txt
W_PATH = "test/calibration/data/steady_w.txt"
with open(W_PATH, "w") as file:
    for i in range(non_steady_index):
        file.write(f"{w_list[i]}\n")
