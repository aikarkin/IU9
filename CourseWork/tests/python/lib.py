from subprocess import Popen, PIPE, TimeoutExpired
from os import walk
from pybel import readfile

MOL_EXTENSIONS = ['sdf', 'mol2', 'pdb', 'alc', 'csf', 'cbin', 'casci', 'cdx', 'cer', 'c3d', 'cml', 'bsd', 'cup', 'gen', 'mol', 'jsd', 'cub', 'smiles', 'smi', 'ds', 'el']


def parse_csv(in_file):
    csv_data = ""
    csv_headers = []
    csv_records = []

    with open(in_file, 'r') as csv_f:
        csv_data = csv_f.read()
    csv_f.close()

    if csv_data == "":
        print("ERROR: Cannot parse CSV file: given file is empty")
    else:
        csv_lines = csv_data.split('\n')
        csv_headers = csv_lines[0].split(',')

        for i in range(1, len(csv_lines)):
            csv_records.append(csv_lines[i].split(','))

    csv_records.remove([''])

    return (csv_headers, csv_records)


def save_to_csv(out_file, csv_headers, csv_records):
    csv_f = open(out_file, 'w')
    headers_line = ""
    rec_count = 0

    for header in csv_headers:
        headers_line += '\"{}\",'.format(header)
    
    csv_f.write(headers_line + '\n')

    for i in range(0, len(csv_records)):
        if type(csv_records[i]) == str:
            csv_f.write('{},'.format(csv_records[i])) 
            rec_count = 1
        elif type(csv_records[i]) == list:
            for j in range(0, len(csv_records[i])):
                csv_f.write('{},'.format(csv_records[i][j])) 
            rec_count = len(csv_records[i])
        for j in range(rec_count, len(csv_headers)):
            csv_f.write(',')
        csv_f.write('\n')


def get_commands(mol_files, script_name, root, csv_headers, csv_records):
    if(csv_headers[0][0] == '\"'):
        for i in range(0, len(csv_headers)):
            csv_headers[i] = csv_headers[i][1:-1]
            # print (csv_headers[i])

    commands = []

    for i in range(0, len(csv_records)):
        cmd = ['./{0}'.format(script_name)]
        substance = ''
        for j in range(0, len(csv_records[i])):          
            param = csv_headers[j]
            val = csv_records[i][j]
            if param == "" or val == "":
                continue

            cmd.append('--{}={}'.format(param, val))
            if param == 'substance':
                substance = val
                cmd.append('--{}={}'.format('input', mol_files[val]))
        if substance != '':    
            cmd.append('--format={}'.format('mol2'))
            cmd.append('--output={}{}__{}.{}'.format(root, substance, i + 1, 'mol2'))
        commands.append(cmd)
    
    return commands


def run_command(cmd, timeout):
    try:
        proc = Popen(cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE)
        proc.wait(timeout=timeout)
        out, err = proc.communicate()
        ret_code = proc.returncode
    except TimeoutExpired:
        out = ""
        err = "timeout"
        ret_code = -1
    return (out, err, ret_code)


def lst_to_line(lst):
    cmd = ''

    for el in lst:
        cmd += el + ' '
    
    return cmd


def test_commands(commands, csv_headers, csv_out_file):
    results = []
    i = 0

    for cmd in commands:
        print('running command {}'.format(lst_to_line(cmd)))
        (cmd_res, err, ret_code) = run_command(cmd, 180)
        if err != b'' or ret_code != 0:
            print('command has executed with errors. See output file: {}'.format(csv_out_file))
            results.append('Command executed with errors. Error code: {}. Error message: {} '.format(ret_code, err))
        else:
            print('command has executed successfuly')
            results.append(cmd_res.decode('utf-8').rstrip().split(','))
        print()
        if i % 10 == 0:
            save_to_csv(csv_out_file, csv_headers, results)
        i = i + 1

    save_to_csv(csv_out_file, csv_headers, results)


def get_mol_files(root):
    mol_files = []
    for (dirpath, dirnames, filenames) in walk(root):
        for fn in filenames:
            if fn.split('.')[-1] in MOL_EXTENSIONS:
                mol_files.append('{}/{}'.format(dirpath, fn))
    return mol_files


def get_mol_dict(mol_files):
    mol_dict = {}
    for mfp in mol_files:
        mf = readfile(mfp.split('.')[-1], mfp)
        mol = mf.__next__()
        if mol.formula and mol.formula != '':
            mol_dict[mol.formula] = mfp
    return mol_dict
        
