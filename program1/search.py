import sys

# test data
seq_filename = str(sys.argv[1])
sequence = []

# main
with open(seq_filename, "r") as seq_file:
    line = seq_file.readline
    print line
