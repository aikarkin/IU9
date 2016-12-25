#!/usr/bin/python3.5
import openbabel as ob
import json
import sys

obconversion = ob.OBConversion()
obconversion.SetInFormat("sdf")
obmol = ob.OBMol()

notatend = obconversion.ReadFile(obmol, sys.argv[1])

table = ob.OBElementTable()

res_obj = {}

if notatend:
    res_obj['title'] = obmol.GetTitle()
    res_obj['formula'] = obmol.GetFormula()

    atoms = {}
    bonds = []

    for b in ob.OBMolBondIter(obmol):
        ba_i = b.GetBeginAtom().GetIdx()
        ea_i = b.GetEndAtom().GetIdx()
        cur_b = (str(ba_i), str(ea_i))
        bonds.append(cur_b)

    for a in ob.OBMolAtomIter(obmol):
        atomic_num = a.GetAtomicNum()
        ca_obj = {}
        ca_obj['atomic_num'] = atomic_num
        ca_obj['symbol'] = table.GetSymbol(atomic_num)
        ca_obj['position'] = (a.GetX(), a.GetY(), a.GetZ())
        ca_obj['vdw_radius'] = table.GetVdwRad(atomic_num)
        ca_obj['color'] = table.GetRGB(atomic_num)
        atoms[a.GetIdx()] = ca_obj
    
    res_obj['atoms'] = atoms
    res_obj['bonds'] = bonds

    res_json = json.dumps(res_obj)
    print(res_json)

else:
    print('error: could not read sdf file', file=stderr)