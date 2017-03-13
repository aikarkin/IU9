#!/usr/bin/python3
from subprocess import call
from random import choice
from datetime import datetime
import os


root = '../resources/'
cell_len_ = [2.0, 2.5, 2.8, 3.0, 3.4, 3.6, 4.0, 5.0]
sigma_ = [3.150, 3.14, 3.12, 3.11, 3.09, 3.05, 3.0, 2.95, 2.9413, 2.8, 2.7, 2.6,
         2.5, 2.3, 2.0, 1.2, 0.31506, 0.31566, 0.031506, 3.15061, 3.1589, 3.166, 3.1668]
eps_ = [1.0, 6.368, 63.68, 0.6364, 0.6368, 0.680946, 0.8822, 0.7749, 0.65, 636.8, 6368, 63680]
dx_ = [1.2, 2.0, 1.0, 0.8, 0.5, 0.4]
da_ = [90, 45, 30, 15]
len_arr_ = [10, 15, 20, 25]

hash = set()


def ad_to_lst(dic):
    args = []
    for key in dic:
        # print('insert {} to args'.format("--{0}={1}".format(key, dic[key])))
        args.append("--{0}={1}".format(key, dic[key]))
    return args


def generate_new_cmd():
    if len(hash) >= len(cell_len_)* len(sigma_) * len(eps_) * len(dx_) * len(da_):
        return ""
    args_vec = (choice(cell_len_), choice(sigma_), choice(eps_), choice(dx_), choice(da_))

    while args_vec in hash:
        args_vec = (choice(cell_len_), choice(sigma_), choice(eps_), choice(dx_), choice(da_))

    hash.add(args_vec)

    ad = {
        "config": root + "default.json",
        "cell_length": args_vec[0],
        "lj_sigma": args_vec[1],
        "lj_epsilon": args_vec[2],
        "trans_x": args_vec[3],
        "trans_y": args_vec[3],
        "trans_z": args_vec[3],
        "rot_x": args_vec[4],
        "rot_y": args_vec[4],
        "rot_z": args_vec[4]
    }
    cmd = ["./mols_packer"]
    cmd.extend(ad_to_lst(ad))
    return cmd

cmds = []

cmd = generate_new_cmd()
max_it = 200
i = 0

def lst_to_str(lst):
    str = ""
    for i in range(len(lst)):
        str += lst[i] + " "
    return str


print("preparing commands...")
while cmd != "" and i < max_it:
    cmds.append(cmd)
    cmd = generate_new_cmd()
    i += 1
print("commands prepared")

print("runing commands...")
log_root = "../logs/"
for i in range(len(cmds)):
    dt_now = str(datetime.now())
    log_dir = log_root + dt_now + "/"
    os.makedirs(log_dir)
    log_file = log_dir + dt_now + ".log"
    
    cmds[i].extend(['--log_dir=' + log_dir])
    cmds[i].extend(['--output=' + log_dir + 'Structure3D_CID_962_10x10x10__' + dt_now + '.mol2'])
    
    f = open(log_file, 'w')
    
    print(dt_now + ":")
    print("\tloging to {0}".format(log_file))
    print("\texec \"{0}\"".format(lst_to_str(cmds[i])))
    call(cmds[i], stdout=f)
    f.close()
    print('————————————————————————————————————————————————————————————————————')


