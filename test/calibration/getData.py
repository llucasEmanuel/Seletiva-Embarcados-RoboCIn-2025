import pandas as pd

# Path do terminal deve estar em "../Seletiva RoboCIn 2025 Embarcados/"
CSV_PATH = "test/calibration/data/filteredRobotLog.csv"
log = pd.read_csv(CSV_PATH)

# Transforma a coluna do data frame em list
w_list = log["local_w"].tolist()
theta_list = log["odometry_theta"].tolist()

W_PATH = "test/calibration/data/local_w.txt"
with open(W_PATH, "w") as file:
    for w in w_list:
        file.write(f"{w}\n")

THETA_PATH = "test/calibration/data/odometry_theta.txt"
with open(THETA_PATH, "w") as file:
    for theta in theta_list:
        file.write(f"{theta}\n")