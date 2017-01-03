var path = require('path');
var fs = require('fs');
var fn_lobj = new Map();
var child_process = require('child_process');

class Lattice {
    constructor(length, width, height, molecule_name, file_ext) {
        this.l = parseInt(length);
        this.w = parseInt(width);
        this.h = parseInt(height);
        this.mol_name = molecule_name;
        var f_ext = file_ext;
    };

    equals(other) {
        console.log('equals');
        console.log(this);
        console.log(other);
        console.log();
        return this.l = other.l && this.w == other.w && this.h == other.h && this.mol_file == other.mol_file;
    }

    static parse(str) {
        var f_obj = path.parse(str);
        this.f_ext = f_obj.ext;
        // var l, w, h, m_name;

        var sep_idx = f_obj.name.lastIndexOf('_');
        this.mol_name = f_obj.name.substring(0, sep_idx);
        var size_str = f_obj.name.substring(sep_idx + 1, f_obj.name.length);
        var lens_ss = size_str.split('x');
        this.l = parseInt(lens_ss[0]);
        this.w = parseInt(lens_ss[1]);
        this.h = parseInt(lens_ss[2]);

        return new Lattice(this.l, this.w, this.h, this.mol_name, this.f_ext);
    }
}

function updateLattices(root, lattices) {
    var files = fs.readdirSync(root);
    for (var i in files) {
        var fn = root + files[i];
        if(!lattices.has(fn)) {
            var cur_lattice = Lattice.parse(fn);
            lattices.set(fn, cur_lattice);
        }
    }
    return lattices;
}

function exec(cmd, kargs, on_exec, on_error) {
  var cmd_args = cmd;
  for(var arg in kargs) {
      cmd_args += ` ${arg} ${kargs[arg]}`;
  }

  console.log(cmd_args);

  child_process.exec(cmd_args, function (error, stdout, stderr) {
      if(error) {
          console.error(error);
          on_error(error);
          return;
      }

      on_exec(stdout, stderr);
  });
}

Map.prototype.indexOf = function(lattice) {
    console.log('indexOf');
    for(var [key, val] of this) {
        if(lattice.equals(val)) {
            console.log('equals!!');
            return key;
        }
    }
    return undefined;
};

exports.fn_lobj = fn_lobj;

exports.Lattice = Lattice;
exports.updateLattices = updateLattices;
exports.exec = exec;