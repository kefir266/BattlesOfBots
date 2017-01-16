/**
 * Created by kefir on 1/15/17.
 */
var fs = require('fs');
// var suspend = require('suspend'),
//     resume = suspend.resume;

var Readable = require('stream').Readable,
    streamIn = new Readable();
var data = fs.readFileSync('./start_field').toString().split('\n');

//step = stepsGenerator();

streamIn._read = function () {
    if (data.length > 0) {
        streamIn.push(data.shift()+'\n');
    } else {
        //streamIn.push("\x04");
        streamIn.push(null);
    }
    //console.log(field);
    //currentMove.pass(field);
    //step.next();
    //console.log(data);
};

//////////////////////////////////////////

var NX = 7,
    NY = 6,
    limitMoves = 50,
    player = [
        'Hexxagon.js',
        'Hexxagon.js'
    ],
    ansver = '',
    ansverPull = [];

var numMoves = 1,
    currentMove;

var spawn = require('child_process').spawn;
//     p = [];
// p[0] = spawn('node', [player[0]], {
//     stdio: ['pipe', 'pipe', null]
// });
// p[1] = spawn('node', [player[1]], {
//     stdio: ['pipe', 'pipe', null]
// });
//
// p[0].stdout.on('data', readAnswer);
// p[1].stdout.on('data', readAnswer);


var dir = [[-1, -1], [-1, +0], [-1, +1],
    [+0, -1], [+0, 1],
    [+1, -1], [+1, +0], [+1, +1]];
var dirJump = [[-1, -2], [-2, +0], [-1, 2],
    [+1, -2], [-2, +0], [+1, 2]];



var Move = function (player) {
    this.x = +0;
    this.y = +0;
    this.px = +0;
    this.py = +0;
    this.isJump = false;
    this.player = player;
};

var Field = function () {
    this.myPlayer = +0;
    this.mas = [];
    this.nMoves = +0;
    this.checkers = [];
};

Field.prototype.readAnswer = function(data) {
    ansver += data;
    //currentMove.pass(field);
    if (ansver != '') {
        console.log('read Ansver: '+ansver);

        ansverPull = ansverPull.concat(ansver.split('\n',1));
        ansver = '';

    }
    if (ansverPull.length > 1) {
        console.log('Go to NEXT');
        console.log(ansverPull);
        //step.next();
        //resume();

    }
};

Field.prototype.scan = function () {
    var mas = [];

    for (y = 0; y < NY; y++) {
        mas[y] = data[y].split(' ');
    }
    this.nMoves = +data[6];
    this.myPlayer = +data[7];
    for (y = 0; y < NY; y++) {
        this.mas[y] = [];
        for (x = 0; x < NX; x++) {
            this.mas[y][x] = +mas[y][x];
            // if (this.mas[y][x] === this.myPlayer) {
            //     this.checkers.push( {y: y, x: x});
            // }
        }
    }
};

Move.prototype.scan = function () {

    for ( ; ansverPull.length === 0 ;) {
         //setTimeout(function () {

            if (ansver != '') {
                console.log('Ansver: '+ansver);
                ansverPull = ansver.split('\n');
                ansver = '';
                break;
            }
         //}, 10);

    }

    var mxy = ansverPull.shift().split(' ');
    console.log('mxy: ' + mxy);
    this.py = +mxy[0];
    this.px = +mxy[1];
    if (ansverPull.length === 0) {
        setTimeout(function () {
            ansverPull = ansver.split('\n');
        }, 100);
    }
    mxy = ansverPull.shift().split(' ');
    this.y = +mxy[0];
    this.x = +mxy[1];
    this.isJump = (Math.abs(this.px - this.x) === 2);
    console.log(this);
};

Move.prototype.pass = function (field) {

    streamIn.resume();
    for (y = 0; y < NY; y++) {
        var str = '';
        for (x = 0; x < NX; x++) {
            str += field.mas[y][x] + ' ';
        }
        console.log(str);
        streamIn.push(str + '\n');
    }

    streamIn.push('' + field.nMoves);
    streamIn.push('' + field.myPlayer);
    streamIn.push("\x04");
    console.log(field);
    this.scan();
};

Field.prototype.injectField = function (m) {
    for (i = 0; i < 8; i++) {
        dy = dir[i][0] + m.y;
        dx = dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] === this.myPlayer) { //it's new field
            this.mas[dy][dx] = m.player;
        }

        //count checkers
        // for (y = 0 ; y < NY ; y++){
        //     for (x = 0 ; x < NX ; x ++) {
        //         if (this.mas[y][x] === this.myPlayer) {
        //             this.checkers.push( {y: y, x: x});
        //         }
        //     }
        // }
    }
};

Field.prototype.nextStep = function (m) {

    if (m.isJump) {
        this.mas[m.py][m.px] = 0;
    }
    this.mas[m.y][m.x] = this.myPlayer;
    this.injectField(m);
    //this.calculateMoves();

    this.myPlayer = 3 - this.myPlayer;
    this.nMoves++;
};

// function* stepsGenerator () {
//     for (i = 0; i <= limitMoves; i++) {
//         var p = spawn('node', [player[1]], {
//             stdio: ['pipe', 'pipe', null]
//         });
//
//         p.stdout.on('data', readAnswer);
//         streamIn.pipe(p.stdin);
//         console.log('yield!!! ' + i);
//
//         //p[field.myPlayer-1].unref();
//         setTimeout(function () {
//             p.kill();
//         }, 2000);
//         yield currentMove.pass(field);
//         console.log('SCAN');
//         currentMove.scan();
//
//     }
// };

function main() {

    field = new Field();

    field.scan();

    currentMove = new Move();
    currentMove.player = field.myPlayer;

    //for (; numMoves <= limitMoves; numMoves++) {
        //step.next();

        //move.pass(field);
        //setTimeout( function () {

        //}, 1000);

    var p = spawn('node', [player[1]], {
        stdio: ['pipe', 'pipe', 'pipe'],
        cwd: undefined,
        env: process.env
    });
    p.on('error', function( err ){ throw err });

    // suspend(function*() {
        p.stdout.on('data', field.readAnswer);
    p.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
    });
        streamIn.pipe(p.stdin);
    p.unref();
        console.log('============suspend=============');
    setTimeout(function () {
        console.log('Game over! ');
        p.kill();
    }, 5000);
    // })();
    // p.stdout.on('data', readAnswer);
    // streamIn.pipe(p.stdin);

    //p[field.myPlayer-1].unref();
    // setTimeout(function () {
    //     p.kill();
    // }, 2000);

        // setTimeout(function () {
        //     console.log('Game over! ');
        //     p[field.myPlayer].kill();
        // }, 5000);
    //}

};



main();

setTimeout(function () {

}, 5000);