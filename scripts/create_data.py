"""
Write program in python that creates timeline (data_x) and values (data_y), provided that
* there are 51 time points between 0 and 10 (interval is rougly 0.2) - each time point, except of 0 is somewhat randomized, let's say that sigma is 0.05 for gaussian distribution
* function(x) is a1e^(b1x) + a2e(^b2x)
* data_y[i] = function(data_x[i])
* save timeline into file that defines c++ const std::array<std::pair<double,double>, 51>
"""

"""Here's a Python program that generates the timeline (data_x) and values (data_y) based on the given conditions and saves the timeline as a C++ const double std::array<std::pair<double,double>, 51> in a file:
pythonCopy code"""

import numpy as np
import random

# Define the function parameters
a1 = 2.0
b1 = -2.0
a2 = 1.0
b2 = -0.2

# Define the number of time points and the interval
num_points = 51
interval = 10 / (num_points - 1)
sigma = 0.05

# Generate the timeline (data_x) with some randomization
data_x = [0.0]  # Start with 0.0
for i in range(1, num_points):
    data_x.append(i * interval + random.gauss(0, sigma))

# Calculate the values (data_y)
def function(x):
    value =  a1 * np.exp(b1 * x) + a2 * np.exp(b2 * x)
    #return value
    sigma = 0.1 * value
    return random.gauss(value, sigma)

data_y = [function(x) for x in data_x]

# Save the timeline as a C++ const double std::array<std::pair<double,double>, 51>
with open("timeline.cpp", "w") as file:
    i:int = 0
    file.write("const std::array<std::pair<double,double>, 51> timeline = { {\n")
    for x, y in zip(data_x, data_y):
        if (i%5 == 0 and i != 0):
            file.write("\n")
        file.write(f"    {{ {x:2.6f}, {y:2.6f} }},")
        i += 1
    file.write("\n} };\n")
    print("Timeline saved to 'timeline.cpp'")

with open("timeline.csv", "w") as file:
    file.write('"data_x", "data_y"\n')
    for x, y in zip(data_x, data_y):
        file.write(f"{x:2.6f},{y:2.6f}\n")
    print("Timeline saved to 'timeline.csv'")