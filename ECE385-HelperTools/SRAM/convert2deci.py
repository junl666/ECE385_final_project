input_filename = '..//On-Chip Memory//sprite_bytes//bg.txt'  # Name of the file to read from
output_filename = 'bg.txt'  # Name of the file to write to

# Read from the input file and write to the output file
with open(input_filename, 'r') as infile, open(output_filename, 'w') as outfile:
    for line in infile:
        hex_value = line.strip()  # Remove newline characters
        dec_value = int(hex_value, 16)  # Convert hex to decimal
        outfile.write(str(dec_value) + '\n')  # Write the decimal value to the output file