from subprocess import call
from os.path import isfile, exists
from os import remove
from time import sleep

def parse_args(conf_file):
	f = open(conf_file, 'r')
	args = []
	out_file = {}
	for line in f:
		key_val = line.split(":")
		if key_val[0] == "output":
			out_file = key_val[1].replace('\n', '')
		arg = "--{0}={1}".format(key_val[0], key_val[1])
		arg=arg.replace('\n', '')
		args.append(arg)
	return (args, out_file)


(args, out_file) = parse_args('../resources/lattice.conf')

cmd_args = ['./mols_packer']
cmd_args.extend(args)

#  check whether output file exists, and deleting it, if exists
if exists(out_file) and isfile(out_file):
	remove(out_file);

call(cmd_args)
call(['jmol', out_file])
