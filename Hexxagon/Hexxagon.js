/**
 * Created by dmitrij on 11.01.2017.
 */
process.stdin.resume();
process.stdin.setEncoding("utf-8");
var input_stdin_array =[];
var input_currentline = 0;

var NY = 6;
var NX = 7;
var counter = 0;


process.stdin.on("data", function (input) {
    input_stdin_array[input_currentline++] = input;

    if (input_currentline === 8){
        main();
    }
});

process.stdin.on("end", function () {
    process.stdout.write('The End');
    main();
});

var dir = [[-1,-1],[-1,+0], [-1, 1],
    [+0, -1], [+0, 1],
    [1, -1], [1, +0], [1, 1]];

var Moves = function(player){
    this.x = +0;
    this.y = +0;
    this.px = +0;
    this.py = +0;
    this.cost = -1000;
    this.isJump = false;
    this.player = player;
};

var Field = function () {
    this.myPlayer = +0;
    this.mas = [];
    this.nMuves = +0;
    this.maxCost = -1000;
    this.bestMove = null;
    //this.moves = [];
    this.checkers = [];
    this.recur = +0;
};


Field.prototype.scan = function () {

    var mas = [];
    for (y = 0 ; y < NY ; y++){
        mas[y] = input_stdin_array[y].split(' ');
    }
    this.nMoves = +input_stdin_array[6];
    this.myPlayer = +input_stdin_array[7];
    for (y = 0 ; y < NY ; y++){
        this.mas[y] = [];
        for (x = 0 ; x < NX ; x ++) {
            this.mas[y][x] = +mas[y][x];
            if (this.mas[y][x] === this.myPlayer) {
                this.checkers.push( {y: y, x: x});
            }
        }
    }
    this.bestMove = new Moves(this.myPlayer);
};

Field.prototype.clone = function(){
    var newField = new Field();
    for (y = 0 ; y < NY; y++){
        newField.mas[y] = [];
        for (x = 0; x < NX ; x++) {
            newField.mas[y][x] = this.mas[y][x];
        }
    }
    //newField.mas = Object.assign([],this.mas);
    newField.myPlayer = 3 - this.myPlayer;
    newField.nMuves = this.nMoves + 1;
    newField.maxCost = 0;
    newField.bestMove = new Moves(newField.myPlayer);
    //newField.moves = [];
    newField.checkers = [];
    newField.recur = this.recur + 1;

    return newField;
};

Field.prototype.injectField = function (m) {
    for (i = 0 ; i < 8 ; i ++) {
        dy = dir[i][0] + m.y;
        dx = dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] === this.myPlayer){ //it's new field
            this.mas[dy][dx] = m.player;
        }

        //count checkers
        for (y = 0 ; y < NY ; y++){
            for (x = 0 ; x < NX ; x ++) {
                if (this.mas[y][x] === this.myPlayer) {
                    this.checkers.push( {y: y, x: x});
                }
            }
        }
    }
};

Field.prototype.comparable = function() {
    var my = 0,
        rival = 0;
    for (y = 0 ; y < NY ; y++) {
        for (x = 0; x < NX; x++) {
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

Field.prototype.nextStep = function(m){


    if (this.nMoves === 100){
        m.cost = this.comparable() * 1000;
        return 0;
    }
    if ( counter > 1000000 || this.recur > 3) {
        m.cost = this.comparable();
        return 0;
    }

    var newField = this.clone();
    if (m.isJump) {
        newField.mas[m.py][m.px] = 0;
    }
    newField.mas[m.y][m.x] = this.myPlayer;
    newField.injectField(m);

    newField.calculateMoves();
    m.cost = -newField.bestMove.cost * this.recur;

    //delete newField;

};

Field.prototype.findMoves = function(m){
    var dx, dy;

    var nMov = 0;
    //moves
    for (i = 0; i < 7 ; i++){
        dy = dir[i][0] + m.py;
        dx = dir[i][1] + m.px;
        if ((dy < NY) && (dx < NX) && (dy >= 0) && (dx >= 0) && (this.mas[dy][dx] === 0)){

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
    for (i = 0; i < 7 ; i++){
        dy = dir[i][0]+dir[i][0] + m.py;
        dx = dir[i][1]+dir[i][1] + m.px;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] === 0){

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

Field.prototype.calculateMoves = function(){
    var field = this;
    var nMov = 0;
    var m = new Moves(this.myPlayer);

    this.checkers.forEach(function(v){
        m.px = v.x;
        m.py = v.y;
        nMov += field.findMoves(m);
    });
    if (nMov === 0 && this.nMoves < 100){
        this.bestMove.cost = -100000;
    }
};


function main() {
    var field = new Field();
    field.scan();
    field.calculateMoves();

    process.stdout.write(field.bestMove.py + ' ' + field.bestMove.px + '\n');
    process.stdout.write(field.bestMove.y + ' ' + field.bestMove.x + '\n');
    process.stdout.write(field.bestMove.cost +'\n');
    process.exit();
}