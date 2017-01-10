package main

import (
	"fmt"
	//"time"
	"container/list"
)

//func ABS(a int) int {
//	return((a) < 0 ? -(a) : (a))
//}
var N = 10
var ch chan int
//var starttime int64
var counter int
type Dir struct {
	y,x int
}

var dir []Dir

type Field struct {
	mas       [10][10]int
	myPlayer  int
	firstMove *Move
	bestMove  *Move
	lastMove  *Move
	amazons[2][4] struct {
		x,y int
	}
}

var listFields *list.List

type Move struct {
	x, y     int
	ax, ay   int
	rx, ry   int
	cost     int
	nextLink *Move
	nAmazon  int
}

func (move *Move) add( nextLink *Move) {
	move.nextLink = nextLink
}

func (field *Field) init() {
	//field.mas = make([]([]int), N)
	//for i := range field.mas {
	//	field.mas[i] = make([]int,N)
	//}
	for y := 0 ; y < N ; y++ {
		for x := 0 ; x < N ; x++ {
			field.mas[y][x] = 0
		}
	}
	field.firstMove = nil
}

func (field *Field) scan() error {

	var err error = nil
	var i[2] int
	for y := 0 ; y < N ; y++ {
		for x := 0 ; x < N ; x++ {
			_, err = fmt.Scan(&field.mas[y][x])
			if field.mas[y][x] == 1 || field.mas[y][x] == 2 {
				field.amazons[field.mas[y][x] - 1][ i[field.mas[y][x] - 1] ].y = y
				field.amazons[field.mas[y][x] - 1][ i[field.mas[y][x] - 1] ].x = x
				i[field.mas[y][x]-1]++
			}
		}
	}
	_, err = fmt.Scan(&field.myPlayer)

	return err
}

func (field *Field) findAmazonArrow(py, px, y, x, player, nAmazon int) int {
	var cx, cy , num int
	for _,d := range dir {
		cx = x + d.x
		cy = y + d.y
		for ; (cx < N && cy < N && cx >= 0 && cy >= 0 && field.mas[cy][cx] == 0) ; {
			num++
			if field.firstMove == nil {
				field.firstMove = &Move{x: px, y: py, ax: x, ay: y, rx: cx, ry: cy, nAmazon: nAmazon}
				field.lastMove = field.firstMove
			} else {
				field.lastMove.nextLink = &Move{x: px, y: py, ax: x, ay: y, rx: cx, ry: cy, nAmazon: nAmazon}
				field.lastMove = field.lastMove.nextLink
			}

			cy += d.y
			cx += d.x
		}
	}
	return num
}

func (field *Field) findAmazonMoves(y, x, player, nAmazon int) int {
	var num int = 0
	field.mas[y][x] = 0

	var cx, cy int
	for _,d := range dir {
		cx = x + d.x
		cy = y + d.y
		for ; (cx < N && cy < N && cx >= 0 && cy >= 0 && field.mas[cy][cx] == 0) ; {

			num +=	field.findAmazonArrow(y, x, cy, cx, player, nAmazon)
			cy += d.y
			cx += d.x
		}
	}
	field.mas[y][x] = player
	return num
}

func (field *Field) copy() *Field  {

	var newField *Field
	type dataList interface {
		data() *Field
	}

	if (listFields.Len()  == 0) {
		newField = &Field{}
		newField.init()
	} else {
		var ee = listFields.Remove(listFields.Front())
		newField = ee.(*Field)
	}
	var y,x int



	for y = 0 ; y < N ; y++ {
		for x = 0 ;x < N ; x++ {
			newField.mas[y][x] = field.mas[y][x]
		}
	}
	var i = 0
	for ; i < 4 ; i++ {
		newField.amazons[0][i].y = field.amazons[0][i].y
		newField.amazons[0][i].x = field.amazons[0][i].x
	}
	return newField
}

func (field *Field) nextStep(recur, player int, move *Move) int  {

	field.mas[move.ay][move.ax] = 0
	field.mas[move.y][move.x] = player
	field.mas[move.ry][move.rx] = -1

	field.amazons[player - 1][move.nAmazon].y = move.y
	field.amazons[player - 1][move.nAmazon].x = move.x

	//field.nextMove(recur,player)
	move.cost = field.calcMoves(recur,player)

	field.amazons[player - 1][move.nAmazon].y = move.ay
	field.amazons[player - 1][move.nAmazon].x = move.ax

	field.mas[move.ay][move.ax] = player
	field.mas[move.y][move.x] = 0
	field.mas[move.ry][move.rx] = 0

	return move.cost
}

func (field *Field) passMoves(recur, player int, ch chan int) int {

	counter++
	if (recur > 3 || (counter > 1000000)) {
		//if recur == 1 {
		//	ch <- 0
		//	close(ch)
		//}
		return 0
	}

	var currentMove *Move = field.firstMove
	var nextField *Field
	nextField = field.copy()
	nextField.myPlayer = player

	var currentCost, bestCost int = 0,-10000

	for ; currentMove != nil ; {

		currentCost = nextField.nextStep(recur, player, currentMove)
		if currentCost > bestCost {
			bestCost = currentCost
			field.bestMove = currentMove
		}
		currentMove = currentMove.nextLink
	}
	//if recur == 1 {
	//	ch <- bestCost
	//	close(ch)
	//}

	listFields.PushBack(nextField)
	return bestCost
}

func (field *Field) calcMoves(recur,player int) int {
	var num int = 0

	//var chn int = 0
	var i int = 0;
	for ; i < 4 ; i++ {
		if recur > 0 {
			num = field.findAmazonMoves(field.amazons[player - 1][i].y, field.amazons[player - 1][i].x, player, i)
		} else {
			field.findAmazonMoves(field.amazons[player - 1][i].y, field.amazons[player - 1][i].x, player, i)
		}
				//if recur == 0 {
				//	chn++
				//	go field.passMoves(recur+1, 3 - player, ch)
				//} else {
					num -= field.passMoves(recur+1, 3 - player, nil)*2

				//}


	}
	var currentMove *Move = field.firstMove
	if recur > 0 {
		var delMove *Move

		for ; currentMove != nil; {

			delMove = currentMove
			currentMove = currentMove.nextLink
			delMove.nextLink = nil
			delMove = nil
		}
		currentMove = nil
		field.lastMove = field.firstMove
	}
//	if recur == 0 {
//		for i := range ch {
//			num -= i;
//			//chn--
//			if (chn == 0) {
//				break
//			}
//		}
//	} else if recur == 1 {
////		ch <- num
//
//	}

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

	field.calcMoves(recur, player)
	field.bestMove = field.getBestMove()
	return field.bestMove
}

func main() {

var field = Field{}
	dir = []Dir{{-1,-1},{-1,0},{-1,1},
	{0,-1},{0,1},
	{1,-1},{1,0},{1,1}}

	ch = make(chan int, (N * N))

	field.scan()

//	starttime = time.Now().UnixNano();
	counter = 0
	listFields = list.New()

	field.nextMove(0,field.myPlayer)

//	endtime := time.Now().UnixNano()

	fmt.Println(field.bestMove.y, field.bestMove.x)
	fmt.Println(field.bestMove.ay, field.bestMove.ax)
	fmt.Println(field.bestMove.ry, field.bestMove.rx)

	fmt.Println(field.bestMove.cost)
}
