var mols = {
    1: '../molecules/Structure3D_CID_962.sdf',
    2: '../molecules/Structure3D_CID_11.sdf',
    3: '../molecules/Structure3D_CID_6212.sdf',
    4: '../molecules/Structure3D_CID_2519.sdf'
}

var express = require('express')
  , app = express()
  , port = 3000

var exec = require('child_process').exec;

app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

app.get('/get/mol/:id', function (req, res) {
  var mol_id = req.params.id
  if (mol_id in mols) {
    //console.log(mols[mol_id]);
    var child = exec('./mol_info.py ' + mols[mol_id], function (error, stdout, stderr) {
      res.send(JSON.stringify({'result': 'success', 'data': stdout}));
    });
  }
  else {
    res.send(JSON.stringify({'result': 'error', 'message': 'there is not molecule with id: {}' % mol_id }));
  }
});

app.listen(port, function () {
  console.log('Listening on port ', port)
});