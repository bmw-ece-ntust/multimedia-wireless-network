import pandas as pd
import matplotlib.pyplot as plt

# Read the data from the text file
data = pd.read_csv("qos_A3_Tino.csv")

# Plotting
plt.figure(figsize=(10, 6))

# Plot each AC separately
for ac in ["VO", "VI", "BE", "BK"]:
    plt.plot(data["OfferedTraffic"], data[ac], label=ac)

plt.title("Throughput per AC (3 streams each) vs Offered traffic per AC (3 streams each)")
plt.xlabel("Offered traffic per AC (3 streams each) (Mb/s)")
plt.ylabel("Throughput per AC (3 streams each) (Mb/s)")
plt.grid(True)
plt.legend()
plt.show()

