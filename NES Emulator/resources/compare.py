import re

orig = open("nestestdump.log", "r")
dump = open("dump.log", "r")

line = 1
while True:
	orig_line = orig.readline()
	dump_line = dump.readline()

	if orig_line == "" or dump_line == "":
		break

	orig_line = orig_line.split(":")[-1]
	dump_line = dump_line.split(")")[0]

	orig_line = orig_line[:-1]
	dump_line = dump_line[1:5]

	if int(orig_line) != int(dump_line):
		print("Difference in line " + str(line) + ": " + orig_line + " --- " + dump_line)
		break

	line += 1
