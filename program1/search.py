import sys

# test data
seq_filename = str(sys.argv[1])
sequence = []

# main
seq_file = open(seq_filename, "r")
lines = seq_file.readlines()
seq_file.close()

for seq in lines:
    print(seq)
