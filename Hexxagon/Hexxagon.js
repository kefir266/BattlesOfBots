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

var dir = [[-1,-1],[-1,0], [-1, 1],
    [0, -1], [0, 1],
    [1, -1], [1, 0], [1, 1]];

var Moves = function(){
    this.x = 0;
    this.y = 0;
    this.px = 0;
    this.py = 0;
    this.cost = 0;
    this.isJump = false;
};

var Field = function () {
    this.myPlayer = 0;
    this.mas = [];
    this.nMuves = 0;
    this.maxCost = -100000;
    this.bestMove = new Moves();
    this.moves = [];
    this.checkers = [];
};

Field.prototype.init = function () {
    for (y = 0 ; y < NY ; y++) {
        this.mas[y] = new Array(NX);
        for (x = 0 ; x < NX ; x++){
            this[y][x] = 0;
        }
    }
};

Field.prototype.scan = function () {

    for (y = 0 ; y < NY ; y++){
        this.mas[y] = input_stdin_array[y].trim(' ');
        for (x = 0 ; x < NX ; x ++) {
            if (this.mas[y][x] == this.myPlayer) {
                this.checkers.push( {y: y, x: x});
            }
        }
    }
    this.nMoves = input_stdin_array[6];
    this.myPlayer = input_stdin_array[7];



};

Field.prototype.nextStep = function(m){
    return 1;
};

Field.prototype.findMoves = function(m){
    var dx, dy;
    var mov;


    //moves
    for (i = 0; i < 7 ; i++){
        dy = dir[i][0] + m.y;
        dx = dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] == 0){
            move = new Moves();
            move.px = m.x;
            move.py = m.y;
            move.x = dx;
            move.y = dy;
            move.isJump = false;

            move.cost = this.nextStep(move);
            if (move.cost > this.maxCost) {
                this.maxCost = move.cost;
                this.bestMove = move;
            }
            this.moves.push(move);
        }
    }

    //jumps
    for (i = 0; i < 7 ; i++){
        dy = dir[i][0]+dir[i][0] + m.y;
        dx = dir[i][1]+dir[i][1] + m.x;
        if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && this.mas[dy][dx] == 0){
            move = new Moves();
            move.px = m.x;
            move.py = m.y;
            move.x = dx;
            move.y = dy;
            move.isJump = false;

            move.cost = this.nextStep(move);
            if (move.cost > this.maxCost) {
                this.maxCost = move.cost;
                this.bestMove = move;
            }
            this.moves.push(move);
        }
    }

    counter++;
};

Field.prototype.calculateMoves = function(m){
    var field = this;
    this.checkers.forEach(function(m){
        field.findMoves(m);
    })


};

Field.prototype.nextMove = function(){
    this.calculateMoves();
};



function main() {
    var field = new Field();
    field.scan();
    field.nextMove();

    process.stdout.write(field.bestMove.py + ' ' + field.bestMove.px + '\n');
    process.stdout.write(field.bestMove.y + ' ' + field.bestMove.x + '\n');

    process.exit();
}