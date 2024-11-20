import numpy as np
import matplotlib.pyplot as plt
import random
plt.rc('font',family='Book Antiqua')
plt.rcParams["axes.labelweight"] = "bold"  # Make axis labels bold
plt.rcParams["axes.titleweight"] = "bold"  # Make title bold
# Function equivalent to gen_num_under_v(int v)
def gen_num_under_v(v):
    if v <= 1:
        return v
    return random.randint(1, v - 1)

# Function equivalent to gen_share(int v, int ShareNum)
def gen_share(v, ShareNum):
    share_num = gen_num_under_v(ShareNum)
    shares = []
    for i in range(share_num, 1, -1):
        s = gen_num_under_v(v - i)
        shares.append(s)
        v -= s
    shares.append(v)
    return shares

# Function to perform the modified splitting of intersection sum
def split_intersection_sum_uniform(intersection_sum):
    split_values = []
    while intersection_sum > np.exp(1):  # Continue until intersection_sum <= exp(1)
        sampled_value = random.randint(6, min(100, intersection_sum))  # Uniform sampling from [6, 100]
        splits = gen_share(sampled_value, 5)  # Split the sampled value
        split_values.extend(splits)  # Add the splits to the result
        intersection_sum -= sampled_value  # Reduce the intersection sum
    split_values.extend(gen_share(int(intersection_sum), 5))  # Split the remaining sum
    return split_values

# Step 1: Generate original values (Gaussian distribution)
origin_values = np.random.normal(loc=50, scale=10, size=1000)  # Mean=50, StdDev=10
origin_values = np.clip(origin_values, 6, 100)  # Ensure values are in the range [6, 100]

# Step 2: Simulate splitting each original value
split_values_origin = []
for value in origin_values:
    splits = gen_share(int(value), 5)  # Split each original value
    split_values_origin.extend(splits)

# Step 3: Simulate intersection values as a subset of original values
intersection_indices = np.random.choice(len(origin_values), size=300, replace=False)  # Random 300 IDs for intersection
intersection_values = origin_values[intersection_indices]

# Step 4: Calculate intersection sum
intersection_sum = int(np.sum(intersection_values))

# Step 5: Generate split values for intersection sum using the updated algorithm
split_values_intersection_sum_updated = split_intersection_sum_uniform(intersection_sum)

# Plotting all distributions
plt.figure(figsize=(14, 8))

# Plot original value distribution
plt.hist(origin_values, bins=30, alpha=0.7, label="Original Value Distribution (Gaussian)", density=True)

# Plot split value distribution from original values
plt.hist(split_values_origin, bins=30, alpha=0.7, label="Split Values from Original Values", density=True)

# Plot split values from intersection sum using the updated algorithm
plt.hist(split_values_intersection_sum_updated, bins=30, alpha=0.7, label="Split Values from Intersection Sum (Updated)", density=True)

# Labels and legend
plt.title("Comparison of Gaussian Value, Split Values, and Updated Intersection-Split Values", fontsize=16,fontweight='bold')
plt.xlabel("Value", fontsize=16,fontweight='bold')
plt.ylabel("Density", fontsize=16,fontweight='bold')
plt.legend(fontsize=12)
plt.grid(True)

plt.show()
