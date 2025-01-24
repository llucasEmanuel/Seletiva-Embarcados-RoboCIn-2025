import pandas as pd

# Path do terminal deve estar em "../Seletiva RoboCIn 2025 Embarcados/"
CSV_PATH = "test/calibration/data/filtered_robot_log.csv"
log = pd.read_csv(CSV_PATH)

# Log em que só existem registros com o theta = 0
null_log = log[log["odometry_theta"] == 0]

# Faz uma lista com os valores da velocidade angular
steady_w = null_log["local_w"].to_list()

soma = 0 # DEBUG

# Abre o arquivo W_PATH e sobrescreve o anterior
W_PATH = "lib/MPU6050/steady_w.txt"
with open(W_PATH, "w") as file:
    # Se houver velocidade angular com o robô parado
    if len(steady_w) != 0:
        # Escreve apenas as velocidades angulares de quando o robô está parado no steady_w.txt
        for w in steady_w:
            file.write(f"{w}\n")
            soma += w # DEBUG
        print(f"Gyroscope offset data successfully written to '{W_PATH}'")

        print(float(soma) / float(len(steady_w))) # DEBUG (calcular offset)

    # Se não houver apenas printa a mensagem, pois já sobrescreveu os valores anteriores
    else:
        print("No gyroscope offset data available in the dataset")
