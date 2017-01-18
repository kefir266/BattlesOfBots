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
app.use('/public', express.static(__dirname + '/public'));
var httpServer = require('http').Server(app);
httpServer.listen(2000);

var io = require('socket.io') (httpServer, {});



module.exports = router;
app.listen(4000);

io.sockets.on('connection', function (socket) {
    console.log("connect");

    socket.on('data', function (data) {

        var inputStr = '',
            inputArray = [],
            pos = 0,
            json;
        if (data.reason === 'start') {
            console.log("start");
            var p = spawn('node', ['./judge.js'], {
                stdio: ['pipe', 'pipe', 'pipe'],
                cwd: undefined,
                env: process.env
            });

            p.stdout.on('data', function (data) {
                inputStr += data;
                pos = inputStr.indexOf('\n');

                if (pos > 0) {
                    inputArray = inputStr.split('\n');

                    for (i = 0 ; i < inputArray.length - 1 ; i++) {

                        json = JSON.parse('' + inputArray[i]);
                        console.log(json.field);
                        if (json.field) {
                            console.log(inputArray[i]);
                            io.emit('field', json);
                        } else {
                            io.emit('move', json);
                        }
                    }
                    inputStr = inputArray[inputArray.length - 1];
                }
            });
            p.stdout.on('end', function (data) {
                console.log('end!!!!!');
            });
            p.on('close', function (code) {
                io.emit('CLOSE');
            });

            p.stderr.on('data', function (data) {
                io.emit('stderr: ' + data);
            });

        }
    });
});