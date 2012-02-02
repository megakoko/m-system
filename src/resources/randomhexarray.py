# Script for generating random 2-byte hex numbers.
# Required length of the array should be passed as the argument
# to this script.

from sys import argv
from random import choice

if len(argv) > 1:
	# Codes for characters '0'...'9', 'A'...'F'.
	integers = range(ord('0'), ord('9')+1) + range(ord('A'), ord('F')+1)

	# Length of the hex array
	length = 0;
	try:
		length = int(argv[1])
	except:
		print 'Wrong array length:', argv[1]
		exit(1)
	
	# Print array 4 items per row.
	for i in range(0, length):
		print '0x' + chr(choice(integers)) + chr(choice(integers)) + ',',
		if i % 4 == 3:
			print ''

