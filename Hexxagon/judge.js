/**
 * Created by kefir on 1/15/17.
 */
var fs = require('fs'),
    Readable = require('stream').Readable,
    spawn = require('child_process').spawn;
var judge, callback;
/**
 * Module exports.
 */

module.exports = Judge;

const
    dirEven = [[-1, -1], [-1, 0], [-1, 1],
        [0, -1], [0, 1],
        [1, 0]],
    dirOdd = [[-1, 0],
        [0, -1], [0, 1],
        [1, -1], [1, 0], [1, 1]],
    dirJumpEven = [[-2, -1], [-1, -2], [-2, 0], [-1, 2], [-2, 1],
        [1, -2], [1, -1], [2, 0], [1, 1], [1, 2]],
    dirJumpOdd = [[-1, -1], [-1, -2], [-2, 0], [-1, 2], [-1, 1],
        [2, -1], [2, -1], [2, 0], [2, 1], [1, 2]];
const NX = 7,
    NY = 6;

//Callback for pipe
function readAnswer(data) {
    console.log('answer: '+data);
    judge.ansver += data;
    if (judge.ansver) {
        judge.ansverPull = judge.ansver.split('\n');

    }
    if (judge.ansverPull.length > 2) {
        judge.currentMove.scan(judge.field);
        judge.ansver = '';
    }
};

function Judge() {
    this.player = [
        {
            prog: 'node',
            arg: 'Hexxagon.js'
        },
        {
            prog: 'node',
            arg: 'Hexxagon.js'
        }
    ];
    this.ansver = '';
    this.ansverPull = [];
    this.limitMoves = 50;
    this.data = fs.readFileSync('./start_field').toString().split('\n');
    this.numMoves = 0;
    this.field = new Field(this);
    this.currentMove = new Move();
    this.moves = {};
    judge = this;
    this.ready = true;
    this.winner = 0;
    this.gameOver = false;
};

Judge.prototype.isReady = function () {
    return this.ready;
};

Judge.prototype.lock = function () {
    this.ready = false;
};

//Transport for bots by pipe
Judge.prototype.passToBot = function () {

    var data = this.data;
    ////interact with bot
    let p = spawn(this.player[this.currentMove.player - 1].prog,
        [this.player[this.currentMove.player - 1].arg],
        {
            stdio: ['pipe', 'pipe', 'pipe'],
            cwd: undefined,
            env: process.env
        });

    let streamIn = new Readable();
    streamIn._read = function () {
        if (data && data.length > 0) {
            streamIn.push(data.shift() + '\n');
        } else {
            streamIn.push(null);
        }
    };

    p.on('error', function (err) {
        throw err
    });

    p.stdout.on('data', readAnswer);
    p.stderr.on('data', (data) => {
        console.log(`stderr: ${data}`);
    });
    streamIn.pipe(p.stdin);
    p.unref();
};

Judge.prototype.start = function (p1, p2, startField) {

    this.ansver = '';
    this.ansverPull = [];
    this.data = fs.readFileSync('./start_field').toString().split('\n');
    this.numMoves = 0;
    this.field = new Field(this);
    this.currentMove = new Move();
    this.moves = {};
    this.ready = true;
    this.winner = 0;
    this.gameOver = false;

    this.field.scan();
    this.currentMove.player = this.field.myPlayer;
    this.currentMove.nMove = 1;
    this.moves ={};
};

//////////////////////////////////////////

let Move = function () {

    this.x = +0;
    this.y = +0;
    this.px = +0;
    this.py = +0;
    this.isJump = false;
    this.player = 1;
    this.nMove = +0;
};

let Field = function (judge) {
    this.judge = judge;
    this.myPlayer = +0;
    this.mas = [];
    this.nMoves = +0;
    this.checkers = [];
};

///First scan field form file
Field.prototype.scan = function () {
    let mas = [];

    for (let y = 0; y < NY; y++) {
        mas[y] = this.judge.data.shift().split(' ');
    }
    this.nMoves = +this.judge.data.shift();
    this.myPlayer = +this.judge.data.shift();
    for (let y = 0; y < NY; y++) {
        this.mas[y] = [];
        for (let x = 0; x < NX; x++) {
            this.mas[y][x] = +mas[y][x];
            // if (this.mas[y][x] === this.myPlayer) {
            //     this.checkers.push( {y: y, x: x});
            // }
        }
    }
};

//Async next step
Judge.prototype.next = function (cb) {
    if (this.ready) {
        this.ready = false;
        process.stdout.write(JSON.stringify({field: this.field.mas}) + '\n');
        console.log("NEXT " + this.numMoves);

        this.field.pass();
        if (cb) {
            callback = cb;
        }
    } else {
        console.log('LOCK');
    }
};



//Apply @move and pull @field to move's storage
Judge.prototype.pullField = function (field, move) {
    if (move.isJump) {
        field.mas[move.py][move.px] = 0;
    }
    field.mas[move.y][move.x] = move.player;
    field.injectField(move);

    this.moves[move.nMove] = {
        field: field,
        move: move
    };

    this.field.nMoves++;
    this.numMoves++;
    this.field.myPlayer = 3 - this.field.myPlayer;
    this.currentMove.nMove++;
    this.currentMove.player = this.field.myPlayer;
    this.ready = true;

    this.hasMoves(this.currentMove.player);

    console.log('callback');
    callback(this.field);
};

Field.prototype.findMoves = function (y, x) {
    let dir, dirJump, dx, dy;
    if ((x % 2) == 0) {
        dir = dirEven;
        dirJump = dirJumpEven;
    } else {
        dir = dirOdd;
        dirJump = dirJumpOdd;
    }
    for (let i = 0; i < dir.length; i++) {
        dy = dir[i][0] + y;
        dx = dir[i][1] + x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0
            && this.mas[dy][dx] === 0) {
            return true;
        }
    }
    for (let i = 0; i < dirJump.length; i++) {
        dy = dirJump[i][0] + y;
        dx = dirJump[i][1] + x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0
            && this.mas[dy][dx] === 0) {
            return true;
        }
    }
}

Judge.prototype.hasMoves = function (player) {
    if (this.numMoves < 100) {
        console.log("player "+player);
        let hasBlank = false;
        for (let y = 0; y < NY; y++) {
            for (let x = 0; x < NX; x++) {
                if (this.field.mas[y][x] === player && this.field.findMoves(y,x)) {
                    return true;
                } else if (this.field.mas[y][x] === 0) {
                    hasBlank = true;
                }
            }
        }
        if (hasBlank) {
            //@player hasn't moves
            this.winner = 3 - player;
            this.gameOver = true;
            return false;
        }
    }
    this.winner = this.whoWinner();
    this.gameOver = true;
    return false;
};


// Validate move
Move.prototype.isValid = function(field){

    if (field.mas[this.py][this.px] === this.player && field.mas[this.y][this.x] === 0) {
        let dir, dirJump, dx, dy;
        if ((this.px % 2) == 0) {
            dir = dirEven;
            dirJump = dirJumpEven;
        } else {
            dir = dirOdd;
            dirJump = dirJumpOdd;
        }
        for (let i = 0; i < 6; i++) {
            dy = dir[i][0] + this.py;
            dx = dir[i][1] + this.px;
            if (dy === this.y && dx === this.x) {
                return true;
            }
        }
        for (let i = 0; i < dirJump.length; i++) {
            dy = dirJump[i][0] + this.py;
            dx = dirJump[i][1] + this.px;
            if (dy === this.y && dx === this.x) {
                return true;
            }
        }
    }
    return false;
};

//Scan reply from bot
Move.prototype.scan = function (field) {

    let mxy = judge.ansverPull.shift().split(' ');
    this.py = +mxy[0];
    this.px = +mxy[1];

    mxy = judge.ansverPull.shift().split(' ');
    this.y = +mxy[0];
    this.x = +mxy[1];
    this.isJump = (Math.abs(this.px - this.x) === 2);
    if (this.isValid(field)) {
        console.log(JSON.stringify(this));
        judge.ansverPull = [];
        field.judge.pullField(field, this);
    } else {
        judge.winner = 3 - this.player;
        callback(this.field);
    }
};

//pass to input buffer field  to push in pipe
Field.prototype.pass = function () {

    for (let y = 0; y < NY; y++) {
        let str = '';
        for (let x = 0; x < NX; x++) {
            if (x !== 0) {
                str += ' ';
            }
            str += this.mas[y][x];
        }
        this.judge.data.push(str);
    }

    this.judge.data.push('' + this.nMoves);
    this.judge.data.push('' + this.myPlayer);
    this.judge.data.push('');

    this.judge.passToBot();
};

//inject neighbor checks after move
Field.prototype.injectField = function (m) {
    let dir, dx, dy;
    if ((m.px % 2) == 0) {
        dir = dirEven;
    } else {
        dir = dirOdd;
    }

    for (let i = 0; i < dir.length; i++) {
        dy = dir[i][0] + m.y;
        dx = dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0
            && this.mas[dy][dx] === (3 - this.myPlayer)) {
            this.mas[dy][dx] = m.player;
        }

        //count checkers
        // for (let y = 0 ; y < NY ; y++){
        //     for (let x = 0 ; x < NX ; x ++) {
        //         if (this.mas[y][x] === this.myPlayer) {
        //             this.checkers.push( {y: y, x: x});
        //         }
        //     }
        // }
    }
};

Judge.prototype.whoWinner = function () {
    let n =[0,0,0];
    for (let y = 0; y < NY; y++) {
        for (let x = 0; x < NX; x++) {
            n[this.field.mas[y][x]]++;
        }
    }
    if (n[1] > n[2]) return 1;
    else if (n[1] < n[2]) return 2;

    return 0;
};

Field.prototype.finish = function () {

};


