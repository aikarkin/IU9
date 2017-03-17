#!/usr/bin/python3
from lib import save_to_csv
from random import choice, uniform

CSV_HEADERS = [
    'substance',
    'length',
    'width',
    'height',
    'expansivity',
    'compression',
    'step_coefficient',
    'rot_eps',
    'trans_eps',
    'lj_epsilon'
]

LJ_EPS_VALS = {
    "H2O": 775,
    "H3N": 358,
    "CHCl3": 355,
    "CH4O": 417,
    "C2H5Cl": 423
}

STEP_COEFFICIENT = 0.1
ROT_EPS = 0.1
TRANS_EPS = 0.001

def get_rand_substance(sub_eps_dic):
    key = choice(list(sub_eps_dic.keys()))
    return (key, sub_eps_dic[key])

def get_compr_vals(start, end, step, precision=2):
    compr = []
    count = int((end - start) / step) + 1
    for i in range(0, count): 
        compr.append(round(start + i*step, precision))
    return compr


def get_param_vec(param_dic, csv_headers):
    param_vec = []
    for i in range(0, len(csv_headers)):
        header = csv_headers[i]
        if header not in param_dic:
            print('ERROR: key {} is not exists in given dictionary'.format(header))
            return
        param_vec.append(param_dic[header])
    return param_vec


def get_param_default_param_dic():
    params = {}
    params['substance'] = 'H2O'
    params['lj_epsilon'] = LJ_EPS_VALS['H2O']
    params['length'] = 20
    params['width'] = 13
    params['height'] = 24
    params['expansivity'] = 1.0
    params['step_coefficient'] = STEP_COEFFICIENT
    params['rot_eps'] = ROT_EPS
    params['trans_eps'] = TRANS_EPS
    return params

compr_vals = get_compr_vals(0.6, 0.8, 0.002, 3)
csv_rows = []

for i in range(0, len(compr_vals)):
    params = get_param_default_param_dic()
    params['compression'] = compr_vals[i]
    csv_rows.append(get_param_vec(params, CSV_HEADERS))

save_to_csv('../data/csv/compr_H2O_fixed_size.2.csv', CSV_HEADERS, csv_rows)
