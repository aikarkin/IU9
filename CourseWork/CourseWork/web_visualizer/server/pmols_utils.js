var path = require('path');
var fs = require('fs');
var child_process = require('child_process');

function isPositiveInteger(n) {
    if (n == undefined)
        return false;
    return 0 === n % (!isNaN(parseFloat(n)) && 0 <= ~~n);
}

function isNumeric(n) {
    if (n == undefined)
        return false;
    return !isNaN(parseFloat(n)) && isFinite(n);
}

function latticeParamsFromRequest(request, idToMolFile, latticeRoot) {
    var queryParams = request.query;

    if (!isNumeric(queryParams['length']) ||
        !isNumeric(queryParams['width']) ||
        !isNumeric(queryParams['height'])) {
        console.error('Error: invalid request params; sizes of box is not a numbers');
        return;
    }

    var molId = request.params.id;

    if (!isPositiveInteger(molId) || !idToMolFile.hasOwnProperty(molId)) {
        console.error('Error: invalid request params - molecule with such id is not exists');
        return;
    }

    var latticeParams = {};

    latticeParams.length = parseFloat(queryParams['length']).toFixed(3);
    latticeParams.width = parseFloat(queryParams['width']).toFixed(3);
    latticeParams.height = parseFloat(queryParams['height']).toFixed(3);

    latticeParams.format = queryParams['format'] || 'json';

    var molFile = idToMolFile[molId];
    latticeParams.molFile = molFile.dir + '/' + molFile.base;
    latticeParams.outFile = `${molFile.name}_${latticeParams.length}x${latticeParams.width}x${latticeParams.height}.${latticeParams.format}`;
    latticeParams.outDir = latticeRoot;

    return latticeParams;
}


function updateCache(cache, rootDir) {
    for(var elem of cache.toArray())
        cache.remove(elem);

    var files = fs.readdirSync(rootDir);
    for (var f of files) {
        cache.add(f);
    }
}

function reqParamsToCmdArgs(reqParams) {
    var kargs = {
        '-L': reqParams.length,
        '-W': reqParams.width,
        '-H': reqParams.height,
        '-I': reqParams.molFile,
        '-F': reqParams.format,
        '-O': reqParams.outDir + reqParams.outFile,
        '-c': 'default.json'
    }
    return kargs;
}

function exec(cmd, latticeParams, on_exec, on_error) {
    var cmdArgs = cmd;
    var kArgs = reqParamsToCmdArgs(latticeParams);

    for (var arg in kArgs) {
        cmdArgs += ` ${arg} ${kArgs[arg]}`;
    }

    child_process.exec(cmdArgs, function (error, stdout, stderr) {
        if (error) {
            on_error(error);
            return;
        }

        on_exec(stdout, stderr);
    });
}

exports.exec = exec;
exports.updateCache = updateCache;
exports.latticeParamsFromRequest = latticeParamsFromRequest;
