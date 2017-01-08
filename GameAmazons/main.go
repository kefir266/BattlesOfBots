package main

import (
	"fmt"
	"time"
)

//func ABS(a int) int {
//	return((a) < 0 ? -(a) : (a))
//}
var N = 10

var onlySpace bool

type Dir struct {
	y,x int
}

var dir []Dir

type Field struct {
	mas       [][]int
	myPlayer  int
	firstMove *Move
	bestMove  *Move
	lastMove  *Move
}

type Move struct {
	x, y     int
	ax, ay   int
	rx, ry   int
	cost     int
	nextLink *Move
}

func (move *Move) add( nextLink *Move) {
	move.nextLink = nextLink
}

func (field *Field) init() {
	field.mas = make([]([]int), N)
	for i := range field.mas {
		field.mas[i] = make([]int,N)
	}
}

func (field *Field) scan() error {

	var err error = nil
	for y := 0 ; y < N ; y++ {
		for x := 0 ; x < N ; x++ {
			_, err = fmt.Scan(&field.mas[y][x])
		}
	}
	_, err = fmt.Scan(&field.myPlayer)

	return err
}

func (field *Field) findAmazonArrow(py,px,y,x,player int) {
	var cx, cy int
	for _,d := range dir {
		cx = x + d.x
		cy = y + d.y
		for ; (cx < N && cy < N && cx >= 0 && cy >= 0 && field.mas[cy][cx] == 0) ; {
			if field.firstMove == nil {
				field.firstMove = &Move{x: px, y: py, ax: x, ay: y, rx: cx, ry: cy}
				field.lastMove = field.firstMove
			} else {
				field.lastMove.nextLink = &Move{x: px, y: py, ax: x, ay: y, rx: cx, ry: cy}
				field.lastMove = field.lastMove.nextLink
			}

			cy += d.y
			cx += d.x
		}
	}

}

func (field *Field) findAmazonMoves(y, x, player int) int {
	var num int = 0
	field.mas[y][x] = 0

	var cx, cy int
	for _,d := range dir {
		cx = x + d.x
		cy = y + d.y
		for ; (cx < N && cy < N && cx >= 0 && cy >= 0 && field.mas[cy][cx] == 0) ; {

			field.findAmazonArrow(y, x, cy, cx, player)
			cy += d.y
			cx += d.x
		}
	}
	field.mas[y][x] = player
	return num
}

func (field *Field) calcMoves(player int) int {
	var num int = 0
	for  y := 0 ; y < N ; y++ {
		for x := 0 ; x < N ; x++ {
			if field.mas[y][x] == player {
				num += field.findAmazonMoves(y, x, player)
			}
		}
	}
	return num
}

func (field *Field) getBestMove() *Move{
	var maxCost int = field.firstMove.cost
	var bestMove *Move = field.firstMove
	var currentMove *Move = field.firstMove
	for ; currentMove != nil ; {
		if currentMove.cost > maxCost {
			maxCost = currentMove.cost
			bestMove = currentMove
		}
		currentMove = currentMove.nextLink
	}
	return bestMove
}

func (field *Field) nextMove(recur, player int) *Move{

	field.calcMoves(player)
	field.bestMove = field.getBestMove()
	return field.bestMove
}

func main() {

var field = Field{}
	dir = []Dir{{-1,-1},{-1,0},{-1,1},
	{0,-1},{0,1},
	{1,-1},{1,0},{1,1}}

	field.init()
	field.scan()

	starttime := time.Now().UnixNano();


	field.nextMove(0,field.myPlayer)

	endtime := time.Now().UnixNano()

	fmt.Println(field.bestMove.y, field.bestMove.x)
	fmt.Println(field.bestMove.ay, field.bestMove.ax)
	fmt.Println(field.bestMove.ry, field.bestMove.rx)

	fmt.Println((endtime - starttime))
}
