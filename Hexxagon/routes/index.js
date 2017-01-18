var express = require('express');
var router = express.Router();
const spawn = require('child_process').spawn;
var app = express();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', { title: 'Express' });
});

app.get('/',function (req, res) {
    res.sendFile(__dirname + '/client/index.html');
});
app.use('/client', express.static(__dirname+'/client'))

app.get('/hexxagon',function (req, res) {


    // res.writeHead(200, { "Content-Type": "text/event-stream",
    //     "Cache-control": "no-cache" });

    res.sendFile(__dirname + '/index.html');

    // var p = spawn('node',['./judge.js'],{
    //      stdio: ['pipe', 'pipe', 'pipe'],
    //      cwd: undefined,
    //     env: process.env
    // });
    //
    // p.stdout.on('data', function (data) {
    //     res.write(data.toString());
    // });
    // p.on('close', function (code) {
    //     res.end('CLOSE');
    // });
    //
    // p.stderr.on('data', function (data) {
    //     res.end('stderr: ' + data);
    // });


});

var httpServer = require('http').Server(app);
httpServer.listen(2000);

var io = require('socket.io') (httpServer, {});



module.exports = router;
app.listen(4000);

io.sockets.on('conection', function () {
    console.log('socket connected');
})