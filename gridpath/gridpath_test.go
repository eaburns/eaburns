package gridpath

import (
	"bufio"
	"fmt"
	"os"
	"testing"
)

func TestLoad(t *testing.T) {
	_, err := loadGridMap("1.map")
	if err != nil {
		t.Error(err)
	}
}

func TestSearch(t *testing.T) {
	m, err := loadGridMap("1.map")
	if err != nil {
		t.Error(err)
	}

	const expectedCost = 15.071067811865476
	_, cost := Astar(m, m.start, m.goal)
	if cost != expectedCost {
		t.Errorf("Expected cost %g, got %g", expectedCost, cost)
	}
}

// A gridmap is a simple grid pathfinding instance.
type gridmap struct {
	w, h        int
	blkd        []bool
	start, goal Loc
}

func (m gridmap) Width() int {
	return m.w
}

func (m gridmap) Height() int {
	return m.h
}

func (m gridmap) Blocked(x, y int) bool {
	return m.blkd[x*m.h+y]
}

// loadGridMap loads a grid map from a file.
func loadGridMap(path string) (gridmap, error) {
	f, err := os.Open(path)
	if err != nil {
		return gridmap{}, err
	}
	defer f.Close()
	return readGridMap(bufio.NewReader(f))
}

// readGridMap reads a grid map from a file.
func readGridMap(in *bufio.Reader) (m gridmap, err error) {
	_, err = fmt.Fscanf(in, "type octile\nheight %d\n width %d\nmap", &m.h, &m.w)
	if err != nil {
		return
	}
	m.blkd = make([]bool, m.w*m.h)

	b, err := in.ReadByte()
	if err != nil {
		return
	}
	if b != '\n' {
		err = fmt.Errorf("Expected a newline after Board:, got '%c'", b)
		return
	}
	for y := 0; y < m.h; y++ {
		for x := 0; x < m.w; x++ {
			b, err = in.ReadByte()
			if err != nil {
				return
			}
			if b != '.' {
				m.blkd[x*m.h+y] = true
			}
		}
		b, err = in.ReadByte()
		if err != nil {
			return
		}
		if b != '\n' {
			err = fmt.Errorf("Expected a newline after line %d, got '%c'", y, b)
			return
		}
	}
	_, err = fmt.Fscanf(in, "%d %d %d %d", &m.start.X, &m.start.Y, &m.goal.X, &m.goal.Y)
	return
}
