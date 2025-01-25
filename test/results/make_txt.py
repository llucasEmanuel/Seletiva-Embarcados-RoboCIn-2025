import pandas as pd

# Path do terminal deve estar em "../Seletiva RoboCIn 2025 Embarcados/"
CSV_PATH = "test/calibration/data/filtered_robot_log.csv"
log = pd.read_csv(CSV_PATH)

# Faz uma lista com os valores da velocidade angular
w_list = log["local_w"].to_list()

# Abre o arquivo W_PATH e sobrescreve o anterior
W_PATH = "test/results/w_input.txt"
with open(W_PATH, "w") as file:
    # Se houver velocidade angular com o robô parado
    if len(w_list) != 0:
        # Escreve apenas as velocidades angulares de quando o robô está parado no steady_w.txt
        for w in w_list:
            file.write(f"{w}\n")
        print(f"Gyroscope data successfully written to '{W_PATH}'")

    # Se não houver apenas printa a mensagem, pois já sobrescreveu os valores anteriores
    else:
        print("No gyroscope data available in the dataset")
