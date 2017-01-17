/**
 * Created by kefir on 1/15/17.
 */
var fs = require('fs');

var Readable = require('stream').Readable,
    streamIn = new Readable();
var data = fs.readFileSync('./start_field').toString().split('\n');
var sim = true;


streamIn._read = function () {
    if (data.length > 0) {
        console.log(data[0]);
        streamIn.push(data.shift() + '\n');
    } else {
        streamIn.push(null);
    }
}

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

Field.prototype.readAnswer = function (data) {
    ansver += data;
    if (ansver != '') {
        ansverPull = ansverPull.concat(ansver.split('\n', 1));
        ansver = '';
    }
    if (ansverPull.length > 1) {
        currentMove.scan(field);
        sim = false;
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

Move.prototype.scan = function (field) {

    var mxy = ansverPull.shift().split(' ');
    this.py = +mxy[0];
    this.px = +mxy[1];

    mxy = ansverPull.shift().split(' ');
    this.y = +mxy[0];
    this.x = +mxy[1];
    this.isJump = (Math.abs(this.px - this.x) === 2);
    console.log(this);
    this.pass(field);
    ansverPull = [];
};

Move.prototype.pass = function (field) {

    if (this.isJump) {
        field.mas[this.py][this.px] = 0;
    }
    field.mas[this.y][this.x] = this.player;
    field.injectField(this);
    field.nMoves++;
    field.myPlayer = 3 - field.myPlayer;
    currentMove.player = field.myPlayer;

    for (y = 0; y < NY; y++) {
        var str = '';
        for (x = 0; x < NX; x++) {
            if (x !== 0) {
                str += ' ';
            }
            str += field.mas[y][x];
        }
        data.push(str);
    }

    data.push('' + field.nMoves);
    data.push('' + field.myPlayer);
    data.push('');

    if (field.nMoves === 100) {
        field.finish();
    }

    var p = spawn('node', [player[1]], {
        stdio: ['pipe', 'pipe', 'pipe'],
        cwd: undefined,
        env: process.env
    });

    var streamIn = new Readable();
    streamIn._read = function () {
        if (data.length > 0) {
            console.log(data[0]);
            streamIn.push(data.shift() + '\n');
        } else {
            streamIn.push(null);
        }
    };
    p.on('error', function (err) {
        throw err
    });

    p.stdout.on('data', field.readAnswer);
    p.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
    });
    streamIn.pipe(p.stdin);
    p.unref();
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

Field.prototype.finish = function () {

};


function main() {

    field = new Field();

    field.scan();

    currentMove = new Move();
    currentMove.player = field.myPlayer;

    var p = spawn('node', [player[1]], {
        stdio: ['pipe', 'pipe', 'pipe'],
        cwd: undefined,
        env: process.env
    });
    p.on('error', function (err) {
        throw err
    });

    p.stdout.on('data', field.readAnswer);
    p.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
    });
    streamIn.pipe(p.stdin);
    p.unref();

};


main();
