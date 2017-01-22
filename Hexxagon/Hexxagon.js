/**
 * Created by dmitrij on 11.01.2017.
 */

///////////////////////////////////////FOR THE CONSOLE///////////
// process.stdin.resume();
// process.stdin.setEncoding("utf-8");
// //console.log('Hexxagon started');
// process.stdin.on("data", function (input) {
//     input_stdin_array[input_currentline++] = input;
//     if (input_currentline === 8){
//         console.log('Stdin ended');
//         main();
//     }
// });
//
// process.stdin.on("end", function () {
//     //process.stdout.write('The End\n');
//     console.log(input_stdin_array);
//     //main();
// });
// let input_currentline = 0;


///////////////////for the judgement system///////////////////
process.stdin.resume();
process.stdin.setEncoding("utf-8");
let stdin_input = "";
//process.stdout.write('Test');

process.stdin.on("data", function (input) {
    //console.log(input);
    stdin_input += input;
});

process.stdin.on("end", function () {
    input_stdin_array = stdin_input.split('\n');
    //process.stdout.write('Start');
    main();
});
///////////////////////////////////////////////////////////////

let input_stdin_array = [];
const NY = 6,
    NX = 7;
let counter = 0;
const dirEven = [[-1, -1], [-1, 0], [-1, 1],
        [0, -1], [0, 1],
        [1, 0]],
    dirOdd = [[-1, 0],
        [0, -1], [0, 1],
        [1, -1], [1, 0], [1, 1]],
    dirJumpEven = [[-2, -1], [-1, -2], [-2, 0], [-1, 2], [-2, 1],
        [1, -2], [1, -1], [2, 0], [1, 1], [1, 2]],
    dirJumpOdd = [[-1, -1], [-1, -2], [-2, 0], [-1, 2], [-1, 1],
        [2, -1], [2, -1], [2, 0], [2, 1], [1, 2]];

let Moves = function (player) {
    this.x = +0;
    this.y = +0;
    this.px = +0;
    this.py = +0;
    this.cost = -1000;
    this.isJump = false;
    this.player = player;
};

let Field = function () {
    this.myPlayer = +0;
    this.mas = [];
    this.nMoves = +0;
    this.maxCost = -1000;
    this.bestMove = null;
    //this.moves = [];
    this.checkers = [];
    this.recur = +0;
};


Field.prototype.scan = function (error) {

    let mas = [];
    for (let y = 0; y < NY; y++) {
        if (! input_stdin_array[y]) {
            error(input_stdin_array);
        }
        mas[y] = input_stdin_array[y].split(' ');
    }
    this.nMoves = +input_stdin_array[6];
    this.myPlayer = +input_stdin_array[7];
    for (let y = 0; y < NY; y++) {
        this.mas[y] = [];
        for (let x = 0; x < NX; x++) {
            this.mas[y][x] = +mas[y][x];
            if (this.mas[y][x] === this.myPlayer) {
                this.checkers.push({y: y, x: x});
            }
        }
    }
    this.bestMove = new Moves(this.myPlayer);
};

Field.prototype.clone = function () {
    let newField = new Field();
    for (let y = 0; y < NY; y++) {
        newField.mas[y] = [];
        for (let x = 0; x < NX; x++) {
            newField.mas[y][x] = this.mas[y][x];
        }
    }
    //newField.mas = Object.assign([],this.mas);
    newField.myPlayer = 3 - this.myPlayer;
    newField.nMoves = this.nMoves + 1;
    newField.maxCost = 0;
    newField.bestMove = new Moves(newField.myPlayer);
    //newField.moves = [];
    newField.checkers = [];
    newField.recur = this.recur + 1;

    return newField;
};

Field.prototype.injectField = function (m) {
    let dir, dx, dy;
    if ((m.px % 2) == 0) {
        dir = dirEven;
    } else {
        dir = dirOdd;
    }
    for (let i = 0; i < 5; i++) {
        dy = dir[i][0] + m.y;
        dx = dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] === this.myPlayer) { //it's new field
            this.mas[dy][dx] = m.player;
        }

        //count checkers
        for (let y = 0; y < NY; y++) {
            for (let x = 0; x < NX; x++) {
                if (this.mas[y][x] === this.myPlayer) {
                    this.checkers.push({y: y, x: x});
                }
            }
        }
    }
};

Field.prototype.comparable = function () {
    let my = 0,
        rival = 0;
    for (let y = 0; y < NY; y++) {
        for (let x = 0; x < NX; x++) {
            switch (this.mas[y][x]) {
                case 0:
                    break;
                case this.myPlayer:
                    my++;
                    break;
                default:
                    rival++;
            }
        }
    }
    return +(my - rival);
};

Field.prototype.nextStep = function (m) {


    if (this.nMoves === 100) {
        m.cost = this.comparable() * 1000;
        return 0;
    }
    if (counter > 2000000 || this.recur > 1) {
        m.cost = this.comparable();
        return 0;
    }

    let newField = this.clone();
    if (m.isJump) {
        newField.mas[m.py][m.px] = 0;
    }
    newField.mas[m.y][m.x] = this.myPlayer;
    newField.injectField(m);

    newField.calculateMoves();
    m.cost = -newField.bestMove.cost * this.recur;

    //delete newField;

};

Field.prototype.findMoves = function (m) {
    let dx, dy;
    let dir, dirJump;

    if ((m.px % 2) == 0) {
        dir = dirEven;
        dirJump = dirJumpEven;
    } else {
        dir = dirOdd;
        dirJump = dirJumpOdd;
    }

    let nMov = 0;
    //moves
    for (let i = 0; i < 6; i++) {
        dy = dir[i][0] + m.py;
        dx = dir[i][1] + m.px;
        if ((dy < NY) && (dx < NX) && (dy >= 0) && (dx >= 0) && (this.mas[dy][dx] === 0)) {

            m.x = dx;
            m.y = dy;
            m.isJump = false;

            this.nextStep(m);
            if (m.cost > this.maxCost) {
                this.maxCost = m.cost;
                this.bestMove.py = m.py;
                this.bestMove.px = m.px;
                this.bestMove.y = m.y;
                this.bestMove.x = m.x;
                this.bestMove.cost = m.cost;
            }
            nMov++;
            //this.moves.push(move);
        }
    }

    //jumps
    for (let i = 0; i < 8; i++) {
        dy = dirJump[i][0] + m.py;
        dx = dirJump[i][1] + m.px;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] === 0) {

            m.x = dx;
            m.y = dy;
            m.isJump = true;

            this.nextStep(m);
            if (m.cost > this.maxCost) {
                this.maxCost = m.cost;
                this.bestMove.py = m.py;
                this.bestMove.px = m.px;
                this.bestMove.y = m.y;
                this.bestMove.x = m.x;
                this.bestMove.cost = m.cost;
            }
            nMov++;
            //this.moves.push(move);
        }
    }

    counter++;

    return nMov;
};

Field.prototype.calculateMoves = function () {
    let field = this;
    let nMov = 0;
    let m = new Moves(this.myPlayer);

    this.checkers.forEach(function (v) {
        m.px = v.x;
        m.py = v.y;
        nMov += field.findMoves(m);
    });
    if (nMov === 0 && this.nMoves < 100) {
        this.bestMove.cost = -100000;
    }
};

function errorScanField(input) {
    console.log(input);
}

function main() {
    //console.log('START');
    let field = new Field();
    field.scan(errorScanField);
    field.calculateMoves();

    process.stdout.write(field.bestMove.py + ' ' + field.bestMove.px + '\n');
    process.stdout.write(field.bestMove.y + ' ' + field.bestMove.x + '\n');
    // process.stdout.write(field.bestMove.cost +'\n');
    // process.stdout.write('0 0\n');
    // process.stdout.write('0 1\n');
    //console.log('OUTPUT');
    process.exit();
}