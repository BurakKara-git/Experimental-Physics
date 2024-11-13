import csv
import os

# Define the directory containing the text files
dir_path = os.getcwd()

# Define a list of filenames to process
file_list = [
    'u1-0.46_u3-0.72_t-180.txt',
    'u1-0.52_u3-0.72_t-180.txt',
    'u1-0.57_u3-0.72_t-180.txt',
    'u1-0.60_u3-1.33_t-180.txt',
    'u1-0.60_u3-1.33_t-185.txt',
    'u1-0.60_u3-1.33_t-195.txt',
    'u1-0.60_u3-1.33_t-202.txt',
    'u1-0.65_u3-0.28_t-180.txt',
    'u1-0.65_u3-0.72_t-180.txt',
    'u1-0.65_u3-1.90_t-180.txt',
]

# Process each file
for filename in file_list:
    # Open the text file for reading
    with open(os.path.join(dir_path, filename), 'r') as txtfile:
        
        # Skip the first 6 rows
        for i in range(6):
            next(txtfile)
        
        # Read the remaining rows and extract the fourth and fifth columns
        data = []
        for line in txtfile:
            # Split the line into columns
            columns = line.strip().split('\t')
            
            # Extract the fourth and fifth columns
            col4 = columns[3]
            col5 = columns[4]
            
            # Append the columns to the data list
            data.append([col4, col5])
            
    # Create a new CSV file for writing
    output_file = os.path.splitext(filename)[0] + '.csv'
    with open(output_file, 'w', newline='') as csvfile:
        
        # Create a CSV writer object
        writer = csv.writer(csvfile)
        
        # Write the header row
        writer.writerow(['I', 'U'])
        
        # Write the data rows
        writer.writerows(data)