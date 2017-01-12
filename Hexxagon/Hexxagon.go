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
	y,x int
}

var dir []Dir

type Chechker struct {
	x, y int
}

type Move struct {
	x, y    int
	px, py  int
	cost    int
	nAmazon int
	isJump  bool
	player  int
}

type Field struct {
	mas      [10][10]int
	myPlayer int
	nMuves   int
	maxCost  int
	bestMove *Move
	checkers *list.List
	recur    int
}

func (field *Field) scan() error {

	var err error = nil
	for y := 0 ; y < NY ; y++ {
		for x := 0 ; x < NX ; x++ {
			_, err = fmt.Scan(&field.mas[y][x])

		}
	}

	_, err = fmt.Scan(&field.nMuves)
	_, err = fmt.Scan(&field.myPlayer)

	field.checkers = list.New()
	for y := 0 ; y < NY ; y++ {
		for x := 0 ; x < NX ; x++ {
			if field.mas[y][x] == field.myPlayer {
				field.checkers.PushBack(Chechker{y,x})
			}
		}
	}

	return err
}

func main() {

	var field = Field{}
	dir = []Dir{{-1, -1}, {-1, 0}, {-1, 1},
		{0, -1}, {0, 1},
		{1, -1}, {1, 0}, {1, 1}}

	field.scan()

	fmt.Println(field.bestMove.py, field.bestMove.px)
	fmt.Println(field.bestMove.y, field.bestMove.x)

	fmt.Println(field.bestMove.cost)
}
