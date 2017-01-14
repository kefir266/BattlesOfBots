package main

import (
	"fmt"
	//"time"
	"container/list"
)

var NY = 6;

var NX = 7;

var counter = 0;

type Dir struct {
	y, x int
}

var dir, dirJump []Dir

type Checker struct {
	y, x int
}

var moves *list.List
var fields *list.List

type Move struct {
	x, y    int
	px, py  int
	cost    int
	isJump  bool
	player  int
}

type Field struct {
	mas      [6][7]int
	myPlayer int
	nMoves   int
	maxCost  int
	bestMove *Move
	checkers *list.List
	recur    int
}

func newMove(player int) *Move  {
	var m *Move
	if moves.Len() > 0 {
		var ee = moves.Remove(moves.Front())
		m = ee.(*Move)
		m.player = player
		m.cost  = 0
	} else {
		m = &Move{player: player}
	}
	return m
}

func newField(player int) *Field {
	var newField *Field
	if fields.Len() > 0 {
		var ee = fields.Remove(fields.Front())
		newField = ee.(*Field)

	} else {
		newField = &Field{}
	}
	newField.maxCost = -90000000
	return newField
}

func (field *Field) newBestMove(m *Move)  {
	field.bestMove = newMove(m.player)
	field.bestMove.px = m.px
	field.bestMove.py = m.py
	field.bestMove.x = m.x
	field.bestMove.y = m.y
	field.bestMove.cost = m.cost
	field.bestMove.isJump = m.isJump
}

func (field *Field) scan() error {

	var err error = nil
	for y := 0; y < NY; y++ {
		for x := 0; x < NX; x++ {
			_, err = fmt.Scan(&field.mas[y][x])


		}
	}


	_, err = fmt.Scan(&field.nMoves)
	_, err = fmt.Scan(&field.myPlayer)
	field.bestMove.player = field.myPlayer

	field.checkers = list.New()
	for y := 0; y < NY; y++ {
		for x := 0; x < NX; x++ {
			if field.mas[y][x] == field.myPlayer {
				field.checkers.PushBack(&Checker{y, x})
			}
		}
	}

	return err
}

func (field *Field) clone() *Field {
	var x, y int
	var player int = 3 - field.myPlayer
	var newField = newField(player)
	for y = 0; y < NY; y++ {
		for x = 0; x < NX; x++ {
			newField.mas[y][x] = field.mas[y][x]
		}
	}
	//newField.mas = Object.assign([],this.mas);
	newField.myPlayer = player
	newField.nMoves = field.nMoves + 1
	newField.maxCost = -9000000
	newField.recur = field.recur + 1
	newField.checkers = list.New()

	return newField;
}

func (field *Field) injectField(m *Move) {
	var dy, dx, y, x int

	for _,d := range dir {
		dy = d.y + m.y
		dx = d.x + m.x
		if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && field.mas[dy][dx] == field.myPlayer) {
			//it's new field
			field.mas[dy][dx] = m.player
		}

		//count checkers
		for y = 0; y < NY; y++ {
			for x = 0; x < NX; x ++ {
				if (field.mas[y][x] == field.myPlayer) {
					field.checkers.PushBack(&Checker{y, x})
				}
			}
		}
	}
}

func (field *Field) comparable() int {
	var my, rival int
	for y := 0; y < NY; y++ {
		for x := 0; x < NX; x++ {
			switch (field.mas[y][x]) {
			case 0:
				break;
			case field.myPlayer:
				my++
				break;
			default:
				rival++
			}
		}
	}

	fmt.Println(my - rival)
	return (my - rival)*10
}

func (field *Field) nextStep(m *Move) {
	if field.nMoves == 100 {
		m.cost = field.comparable() * 1000
		return ;
	}
	if ( counter > 20000000 || field.recur > 1) {
		m.cost = field.comparable()
		return ;
	}

	var newField *Field = field.clone()
	if (m.isJump) {
		newField.mas[m.py][m.px] = 0
	}
	newField.mas[m.y][m.x] = field.myPlayer
	newField.injectField(m)

	m.cost = field.comparable()
	newField.calculateMoves()
	m.cost = -newField.bestMove.cost * field.recur
	fields.PushBack(newField)

}

func (field *Field) findMoves(m *Move) int {
	var dx, dy int

	var nMov int = 0
	//moves
	for _,d := range dir {
		dy = d.y + m.py
		dx = d.x + m.px
		if (dy < NY) && (dx < NX) && (dy >= 0) && (dx >= 0) && (field.mas[dy][dx] == 0) {

			m.x = dx
			m.y = dy
			m.isJump = false

			field.nextStep(m)

			if m.cost > field.maxCost {
				field.maxCost = m.cost
				field.newBestMove(m)
			}
			if field.recur == 0   {
			}
			if field.bestMove == nil {
				field.newBestMove(m)
			}
			nMov++
			//this.moves.push(move);
		}
	}

	//jumps
	for _,d := range dirJump {

		dy = d.y + m.py
		dx = d.x + m.px
		if (dy < NY && dx < NX && dy >= 0 && dx >= 0 && field.mas[dy][dx] == 0) {

			m.x = dx
			m.y = dy
			m.isJump = true

			field.nextStep(m)
			if (m.cost > field.maxCost) {
				field.maxCost = m.cost
				field.newBestMove(m)
			}
			if field.bestMove == nil {
				field.newBestMove(m)
			}
			if field.recur == 0   {
			}
			nMov++
			//this.moves.push(move);
		}
	}

	counter++
	return nMov
}

func (field *Field) calculateMoves() {
	//var field = this;
	var nMov int = 0
	var m *Move
	m = newMove(field.myPlayer)
	var v *list.Element
	for v = field.checkers.Front(); v != nil; v = v.Next() {
		m.px = v.Value.(*Checker).x
		m.py = v.Value.(*Checker).y
		nMov += field.findMoves(m)
	}
	if (nMov == 0 && field.nMoves < 100) {
		field.bestMove.cost = -100000;
	}
	moves.PushBack(m)
};

func main() {

	var field = Field{maxCost: -9000000, bestMove:&Move{}}
	dir = []Dir{{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1}}
	dirJump = []Dir{{-1, -2}, {-2, 0}, {-1, 2},
		{1, -2}, {2, 0}, {1, 2}}
	moves = list.New()
	fields = list.New()

	field.scan()
	field.calculateMoves()

	fmt.Println(field.bestMove.py, field.bestMove.px)
	fmt.Println(field.bestMove.y, field.bestMove.x)

	fmt.Println(field.bestMove.cost)
}
