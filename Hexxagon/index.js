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

    res.sendFile(__dirname + '/index.html');
});

app.use('/public', express.static(__dirname + '/public'));
var httpServer = require('http').Server(app);
httpServer.listen(2000);

var io = require('socket.io') (httpServer, {});

module.exports = router;
app.listen(4000);
var Judge = require('./judge.js');
var judge = new Judge();

function passField(field) {
    json = {
        field: (field) ? field.mas : null,
        winner: judge.winner,
        gameOver: judge.gameOver
    };
    console.log(json);
    console.log(judge.ready);
    io.emit('field', json);
    json = {
        move: judge.currentMove
    };
    io.emit('move', json);

}

io.sockets.on('connection', function (socket) {
    console.log("connect");

    socket.on('data', function (data) {

        var inputStr = '',
            inputArray = [],
            pos = 0,
            json;
        if (data.reason === 'start') {
            judge.start();
            console.log("start");

            json = {
                field: judge.field.mas,
                winner: judge.winner
            };
            io.emit('field', json);

        }
        if (data.reason === 'next') {
            console.log('NEXT');
            judge.next(passField);

        }
    });
});