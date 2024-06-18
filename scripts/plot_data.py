import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np

sns.set_theme()

# Read the CSV file
data = pd.read_csv('noisy_timeline.csv', quotechar='"', skipinitialspace=True)
#data = pd.read_csv('clean_timeline.csv', quotechar='"', skipinitialspace=True)

# Extract the data points
data_x = data['data_x']
data_y = data['data_y']

# Define the function
def fit_func(x):
    return 0.959354*np.exp(-0.195071*x) + 2.23905*np.exp(-1.89224*x)

def orig_func(x):
    return 1*np.exp(-0.2*x) + 2*np.exp(-2.0*x)

# Create a figure and axis
plt.rcParams["figure.figsize"] = (9.0, 6.0)
fig, ax = plt.subplots(figsize=(8, 6))

# Plot the data points
sns.scatterplot(x=data_x, y=data_y, ax=ax, label='Data Points')

# Plot the function
function_y = [fit_func(x) for x in data_x]
ax.plot(data_x, function_y, 'r-', label='Fit')

function_y = [orig_func(x) for x in data_x]
ax.plot(data_x, function_y, 'g-', label='Orig')

# Set labels and title
ax.set_xlabel('x')
ax.set_ylabel('y')
ax.set_title('Data Plot with Function')

# Set range
ax.set_xlim(0, 10)
ax.set_ylim(0, 3.5)

# Add a legend
ax.legend()

# Show the plot
fig.savefig("noisy_timeline.png")
#fig.savefig("clean_timeline.png")
