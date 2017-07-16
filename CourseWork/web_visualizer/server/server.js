
#!/usr/bin/node
var express = require('express')
    , app = express()
    , port = 3000;
var fs = require('fs');
var pmols = require('./pmols_utils.js');
var path = require('path');
var HashSet = require('hashset');
var sassMidleware = require('node-sass-middleware');

var VIEWS = __dirname + '/views/';


app.use(function (req, res, next) {
    res.setHeader('Access-Control-Allow-Origin', '*');
    res.setHeader('Access-Control-Allow-Methods', 'GET');
    res.setHeader('Access-Control-Allow-Headers', 'X-Requested-With,content-type');
    res.setHeader('Access-Control-Allow-Credentials', true);
    next();
});

app.use(
    sassMidleware({
        src: __dirname + '/public/sass',
        dest: __dirname + '/public/css',
        outputStyle: 'compressed',
        indentedSyntax: true,
        prefix: "/css",
        debug: true
    })
);

app.use('/', express.static(__dirname + '/public'));


app.get('/', function(req, res) {
    res.sendfile(path.join(VIEWS, "index.html"));
});

app.get('/viewer', function(req, res) {
    res.sendfile(path.join(VIEWS, "mols-viewer.html"));
});



app.get('/download/lattice/:id', function (req, res) {
    setTimeout(function () {
        res.send(JSON.stringify({res: 'download'}));
    }, 5000);
});


app.get('/get/lattice/:id', function (req, res) {
    setTimeout(function () {
        res.send(JSON.stringify({res: 'get'}));
    }, 5000);
});


app.listen(port, function () {
    console.log('Listening on port ', port);
});
