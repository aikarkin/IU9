var express = require('express')
    , app = express()
    , port = 3000;
var fs = require('fs');
var pmols = require('./pmols_utils.js');
var path = require('path');
var HashSet = require('hashset');

var LATTICES_ROOT = '../lattices/';
var PACK_SCRIPT = '../../bin/./mols_packer';
var CACHE = new HashSet();
var MOLECULES = {
    1: path.parse('../molecules/Structure3D_CID_962.sdf'),
    2: path.parse('../molecules/Structure3D_CID_11.sdf'),
    3: path.parse('../molecules/Structure3D_CID_6212.sdf'),
    4: path.parse('../molecules/Structure3D_CID_2519.sdf')
};

pmols.updateCache(CACHE, LATTICES_ROOT);


/*  onSuccess(filePath, debugInfo)
    onError(errType, errMessage)  */
function getLattice(latticeParams, onSuccess, onError) {
    var filePath = latticeParams.outDir + latticeParams.outFile;

    if (CACHE.contains(latticeParams.outFile)) {
        onSuccess(filePath, 'cache');
        return;
    }

    pmols.exec(PACK_SCRIPT, latticeParams,
        (debug, err) => {
            if (typeof (err) === 'string' && err != '') {
                onError('runtime error', err);
                return;
            }
            CACHE.add(latticeParams.outFile);
            onSuccess(filePath, debug);
        },
        (err) => {
            onError('execution error', err);
            return;
        }
    );
}


app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});


app.get('/download/lattice/:id', function (req, res) {
    latticeParams = pmols.latticeParamsFromRequest(req, MOLECULES, LATTICES_ROOT);
    this.res = res;

    getLattice(
        latticeParams,
        (filePath, debug) => {
            if(debug == 'cache') {
                console.log('Lattice configuration has taken from cache');
            }
            else {
                console.log(`${PACK_SCRIPT} has successfully executed`);
                console.log(`debug info: \n${debug}`);
            }

            res.download(path.resolve(filePath), latticeParams.outFile);
        },
        (errType, errMessage) => {
            if(errType == 'execution error') {
                console.log(console.log(`${PACK_SCRIPT} execution failed.\nError(s):${errMessage}`));
            }
            else if(errType == 'runtime error') {
                console.log(console.log(`${PACK_SCRIPT} has executed with errors:\n${errMessage}`));
            }

            res.send(`Error: ${errType}.\n\t${errMessage}`);
        }
    );
});


app.get('/get/lattice/:id', function (req, res) {
    latticeParams = pmols.latticeParamsFromRequest(req, MOLECULES, LATTICES_ROOT);
    this.res = res;

    getLattice(
        latticeParams,
        (filePath, debug) => {
            if(debug == 'cache') {
                console.log('Lattice configuration has taken from cache');
            }
            else {
                console.log(`${PACK_SCRIPT} has successfully executed`);
                console.log(`debug info: \n${debug}`);
            }

            data = fs.readFileSync(filePath, 'utf8');
            res.send(JSON.stringify({
                'result': 'success',
                'debug': debug,
                'data': data
            }));
        },
        (errType, errMessage) => {
            if(errType == 'execution error') {
                console.log(console.log(`${PACK_SCRIPT} execution failed.\nError(s):${errMessage}`));
            }
            else if(errType == 'runtime error') {
                console.log(console.log(`${PACK_SCRIPT} has executed with errors:\n${errMessage}`));
            }

            res.send(JSON.stringify({
                'result': 'error',
                'type': errType,
                'discription': errMessage
            }));
        }
    );
});


app.listen(port, function () {
    console.log('Listening on port ', port);
});