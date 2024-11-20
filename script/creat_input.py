import random

# Define file names
id_file_name = "../example_input/ids.txt"
id_value_file_name = "../example_input/id_values.txt"

# Number of entries to generate
num_entries = 1000

# Generate and write to the files
with open(id_file_name, "w") as id_file, open(id_value_file_name, "w") as id_value_file:
    for i in range(1, num_entries + 1):
        # Generate ID and value
        id_str = f"id{i}"
        value = random.randint(6, 100)  # Ensure value > 5

        # Write to id file
        id_file.write(f"{id_str}\n")

        # Write to id:value file
        id_value_file.write(f"{id_str}:{value}\n")

print(f"Files '{id_file_name}' and '{id_value_file_name}' created successfully!")
