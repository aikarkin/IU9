#!/usr/bin/python3
from argparse import ArgumentParser
from lib import parse_csv, get_commands, test_commands, get_mol_files, get_mol_dict


arg_parser = ArgumentParser()
arg_parser.add_argument('-m', '--mols-root', help='Directory containing molecules files')
arg_parser.add_argument('-l', '--lattices-root', help='Output directory, which will contain output lattices')
arg_parser.add_argument('-c', '--csv-file', help='CSV file containing tests')
arg_parser.add_argument('-o', '--output-file', help='File containing results of tests')
arg_parser.add_argument('-p', '--packing-script', help='Command which generate close-packed structure')
args = arg_parser.parse_args()

mols_root = '.'
lattices_root = '.'
output_file = ''

if args.mols_root:
    mols_root = args.mols_root
if args.lattices_root:
    lattices_root = args.lattices_root
if not args.csv_file:
    print('ERROR: Test file is not matched.')
    exit()
if not args.packing_script:
    print('ERROR: Packing script is not matched')
    exit()

packing_script = args.packing_script
csv_file = args.csv_file

if not args.output_file:
    csv_fn_split = csv_file.split('.')
    output_file = ''.join(csv_fn_split[:-1]) + '_res.' + csv_fn_split[-1]
else:
    output_file = args.output_file


TESTS_HEADERS = [
    "MOL_NUM",
    "APPOS_POINT",
    "BOX_SIZE",
    "TOTAL_DIST",
    "AVG_DIST",
    "INTER_NUM",
    "TOTAL_INTER",
    "AVG_INTER",
    "MAX_INTER",
    "EMPTY_CELLS_NUM",
    "ITER_NUM",
    "ES_ITER_NUM",
    "PS_ITER_NUM",
    "FP_PS_ITER_NUM"
];

mol_files = get_mol_files(mols_root)
mols_dic = get_mol_dict(mol_files)
for mn in mols_dic:
    print(mn)

(csv_headers, csv_records) = parse_csv(csv_file)

commands = get_commands(mols_dic, packing_script, lattices_root, csv_headers, csv_records)
test_commands(commands, TESTS_HEADERS, output_file)
