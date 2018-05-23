## @author kovlev
# Checks whether the .dll and .exe files are 32 or 64 bits in the given directory
# Also able to check single files
# Uses Python3

import os
import argparse
import struct


#The allowed file extensions
EXTENSIONS = [".exe", ".dll"]


def check_dir(directory):
	try:
		resultSet = set()
		
		#Getting every entry in directory
		for entry in os.listdir(directory):
			fullpath = os.path.join(directory, entry)
			#We are parsing only the files
			#NOTE no recursive foldertree traversing
			if os.path.isfile(fullpath):
				_, extension = os.path.splitext(entry)
				#Files are not handled differently by extension
				if extension in EXTENSIONS:
					arch = check_file(fullpath)
					resultSet.add(arch)
					print(entry + ":\t" + arch)
		
		if len(resultSet) > 1:
			print("\nWARNING! Multiple exec architecture definitions found in directory: " + str(resultSet))
		elif len(resultSet) == 0:
			print("ERROR: No executables or dlls in directory")
	except Exception as e:
		#Not sure if the code flow can reach this line
		#But better safe than sorry
		print("\nERROR: " + str(e))


def check_file(path):
	with open(path, "rb") as f:
		#Check MZ signature (magic) at the beginning of the file
		mz = f.read(2)
		if mz != b'MZ':
			return "Invalid MZ signature"
		
		#Gets PE header pointer
		f.seek(0x3c, 0)
		pe_pointer = f.read(2)
		pe_pos = struct.unpack('<H', pe_pointer)[0]
		
		f.seek(pe_pos, 0)
		pe = f.read(4)
		if pe != b'PE\x00\x00':
			return "Invalid PE signature"
		
		#NOTE they are little endian
		arch = struct.unpack("<H", f.read(2))[0]
		
		#Does not check a whole bunch of architectures
		if arch == 0x8664:
			return "64 bit"
		elif arch == 0x014c:
			return "32 bit"
		
	return "Unknown architecture"


def main():
	parser = argparse.ArgumentParser(description="Determinates whether the given executable(s) (in a folder on alone) is/are designed for 32 or 64 bit architecture")
	parser.add_argument("target", help="The directory to scan in or the executable file path")
	args = parser.parse_args()
	if args.target == None:
		print(parser.usage)
	else:
		if os.path.isfile(args.target):
			print(os.path.basename(args.target) + ":\t" + check_file(args.target))
		elif os.path.isdir(args.target):
			check_dir(args.target)
		else:
			print("ERROR: Invalid target")


#Check if we run this file explicitly
if __name__ == "__main__":
	main()
