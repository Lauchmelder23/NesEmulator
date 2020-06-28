import re

orig = open("nestestdump.log", "r")
dump = open("dump.log", "r")

line = 1
while True:
	orig_line = orig.readline()
	dump_line = dump.readline()

	if orig_line == "" or dump_line == "":
		break

	orig_line = orig_line.translate(str.maketrans('', '', ' $\t'))
	dump_line = dump_line.translate(str.maketrans('', '', ' $\t'))

	dump_line = dump_line.split(")")[1]

	orig_line = orig_line[:4]
	dump_line = dump_line[:4]

	if orig_line != dump_line:
		print("Difference in line " + str(line) + ": " + orig_line + " --- " + dump_line)
		break

	line += 1
