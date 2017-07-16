from subprocess import call


for i in range(7):
    eps = 0.01 * (10**i)
    print('eps: {}'.format(eps))
    call(['./mols_packer', '--config=../resources/packing_conf.json', '--lj_epsilon={}'.format(eps), '--output=../logs/lattice_15x15x15__{}.mol2'.format(i)])
