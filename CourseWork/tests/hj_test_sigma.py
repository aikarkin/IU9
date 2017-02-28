#!/usr/bin/python3
from subprocess import check_call, CalledProcessError
from random import choice
from datetime import datetime
import os


root = '../resources/'

sigma0 = 2.0
sigma_ = [];
for i in range(21):
	sigma_.append(sigma0 + 0.1*i)
	
sigma_.extend([3.150, 3.14, 3.12, 3.11, 3.09, 3.05, 3.0, 2.95, 2.9413, 2.8, 2.7, 2.6,
         2.5, 2.3, 2.0, 1.2, 0.31506, 0.31566, 0.031506, 3.15061, 3.1589, 3.166, 3.1668])


dx = 1.2
da = 90


def ad_to_lst(dic):
    args = []
    for key in dic:
        # print('insert {} to args'.format("--{0}={1}".format(key, dic[key])))
        args.append("--{0}={1}".format(key, dic[key]))
    return args


def generate_cmd(sigm):
    ad = {
        "config": root + "packing_conf.json",
        "cell_length": 3.0,
        "lj_sigma": sigm,
        "lj_epsilon": 0.6368,
        "trans_x": dx,
        "trans_y": dx,
        "trans_z": dx,
        "rot_x": da,
        "rot_y": da,
        "rot_z": da
    }
    cmd = ["./mols_packer"]
    cmd.extend(ad_to_lst(ad))
    
    return cmd


cmds = []


def lst_to_str(lst):
    str = ""
    for i in range(len(lst)):
        str += lst[i] + " "
    return str


print("preparing commands...")
for i in range(len(sigma_)):
    cmd = generate_cmd(sigma_[i])
    cmds.append(cmd)
print("commands prepared")


print("runing commands...")
log_root = "../logs/"

for i in range(len(cmds)):
    dt_now = str(datetime.now())
    log_dir = log_root + dt_now + "/"
    os.makedirs(log_dir)
    log_file = log_dir + dt_now + ".log"
    err_file = log_dir + "err__" + dt_now + ".log"
    
    cmds[i].extend(['--log_dir=' + log_dir])
    cmds[i].extend(['--output=' + log_dir + 'Structure3D_CID_962_10x10x10__' + dt_now + '.mol2'])
    
    lf = open(log_file, 'w')
    ef = open(err_file, 'w')
    
    print(dt_now + ":")
    print("\tloging to {0}".format(log_file))
    print("\texec \"{0}\"".format(lst_to_str(cmds[i])))
    try:
        check_call(cmds[i], stdout=lf, stderr=ef, timeout=120)
    except CalledProcessError as cpe:
        print("Process error. See logs at" + err_file)
        lf.write(cpe.message);
    lf.close()
    ef.close()
    print('————————————————————————————————————————————————————————————————————')

