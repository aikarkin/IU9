var express = require('express')
  , app = express()
  , port = 3000;
var fs = require('fs');
var pmols = require('./pmols_utils.js');
var path = require('path');

var MOLECULES = {
    1: '../molecules/Structure3D_CID_962.sdf',
    2: '../molecules/Structure3D_CID_11.sdf',
    3: '../molecules/Structure3D_CID_6212.sdf',
    4: '../molecules/Structure3D_CID_2519.sdf'
}

pmols.updateLattices('../lattices/', pmols.fn_lobj);

function try_get_and_send(resp, lattice, debug) {
  var lkey = pmols.fn_lobj.indexOf(lattice);
  console.log(lkey)
  if(lkey == undefined)
    return undefined;
  data = fs.readFileSync(lkey, 'utf8');
  resp.send(JSON.stringify({'result': 'success', 'data': data, 'debug': debug}));
  return lkey;
}

app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

app.get('/get/mol/:id', function (req, res) {
  var mol_id = req.params.id;
  if (mol_id in MOLECULES) {
    var lparams = {
      '-L': req.param('L'),
      '-W': req.param('W'),
      '-H': req.param('H'),
      '-f': 'json',
      '-i': MOLECULES[mol_id],
      '-d': '../lattices/'
    };

    var mol_name = path.parse(MOLECULES[mol_id]).name;
    req_lattice = new pmols.Lattice(lparams['-L'], lparams['-W'], lparams['-H'], mol_name, lparams['-f']);
    sent_res = try_get_and_send(res, req_lattice, 'cache');
    if(sent_res == undefined) {
      pmols.exec('./mols_packer', lparams,
        (stdout, stderr) => {
          pmols.updateLattices('../lattices/', pmols.fn_lobj);
          sent_res = try_get_and_send(res, req_lattice, stdout);
          if(sent_res == undefined) {
            res.send(JSON.stringify({'result': 'runtime error', 'errors': stderr, 'debug': stdout}));
            return;
          }
        },
        (err) => { res.send(JSON.stringify({'result': 'execution error', 'error': err}));  });
    }
  }
  else {
    res.send(JSON.stringify({'result': 'error', 'message': 'there is no molecule with given id: {}' % mol_id }));
  }
});

app.listen(port, function () {
  console.log('Listening on port ', port)
});