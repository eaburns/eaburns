// An implementation of grid-based pathfinding
package gridpath

import (
	"container/heap"
	"fmt"
	"math"
	"time"
)

// A GridMap represents a grid-based map of blocked
// and unblocked cells.
type GridMap interface {
	Blocked(int, int) bool
	Width() int
	Height() int
}

// A Loc is a location in a grid.
type Loc struct {
	X, Y int
}

// Astar returns the shortest path and ints cost between
// start and goal in the given grid map.
func Astar(m GridMap, start, goal Loc) ([]Loc, float64) {
	stride := m.Height()
	goali := goal.X*stride + goal.Y
	open := make(openList, 0, m.Width()*m.Height())
	closed := makeClosedList(m)

	starti := start.X*stride + start.Y
	closed[starti].g = 0
	closed[starti].f = 0
	heap.Push(&open, &closed[starti])

	expd, gend, begin := 0, 0, time.Now()

	for len(open) > 0 {
		n := open[0]
		heap.Pop(&open)
		if n.ind == goali {
			break
		}

		expd++

		x, y := n.ind/stride, n.ind%stride
		for _, mv := range moves {
			if !mv.ok(m, x, y) {
				continue
			}
			gend++
			kidx, kidy := x+mv.dx, y+mv.dy
			kidi := kidx*stride + kidy
			kid := &closed[kidi]
			cost := n.g + mv.cost
			if kid.g >= 0 && kid.g < cost {
				continue
			}
			kid.parent = n.ind
			kid.g = cost
			if kid.h < 0 {
				kid.h = octiledist(kidx, kidy, goal.X, goal.Y)
			}
			kid.f = kid.g + kid.h
			if kid.pqindex >= 0 {
				heap.Remove(&open, kid.pqindex)
			}
			heap.Push(&open, kid)
		}
	}

	fmt.Println("path cost", closed[goali].g)
	fmt.Println("expanded", expd)
	fmt.Println("generated", gend)
	fmt.Println("seconds", time.Since(begin))

	return makePath(m, closed, goali), closed[goali].g
}

// octiledist returns the 8-way heuristic cost-to-go estimate
// between x0,y0 and x1,y1.
func octiledist(x0, y0, x1, y1 int) float64 {
	dx := x0 - x1
	if dx < 0 {
		dx = -dx
	}
	dy := y0 - y1
	if dy < 0 {
		dy = -dy
	}
	diag, straight := dx, dy
	if straight < diag {
		diag, straight = straight, diag
	}
	return float64(straight-diag) + float64(diag)*math.Sqrt(2)
}

// A move is a legal move within the grid specified
// by the delta from the current location and the
// cost.
type move struct {
	dx, dy int
	other  []Loc // locations that must also be free
	cost   float64
}

var (
	// moves is the array of all valid moves.
	moves = [...]move{
		{1, 0, []Loc{}, 1},
		{-1, 0, []Loc{}, 1},
		{0, 1, []Loc{}, 1},
		{0, -1, []Loc{}, 1},
		{1, 1, []Loc{{1, 0}, {0, 1}}, math.Sqrt(2)},
		{1, -1, []Loc{{1, 0}, {0, -1}}, math.Sqrt(2)},
		{-1, 1, []Loc{{-1, 0}, {0, 1}}, math.Sqrt(2)},
		{-1, -1, []Loc{{-1, 0}, {0, -1}}, math.Sqrt(2)},
	}
)

// ok returns true if the given move is allowed in the grid.
func (mv move) ok(m GridMap, x, y int) bool {
	for _, d := range mv.other {
		i, j := x+d.X, y+d.Y
		if !clear(m, i, j) {
			return false
		}
	}
	return clear(m, x+mv.dx, y+mv.dy)
}

// clear returns true if the given coordinate is both on the map
// and not blocked.
func clear(m GridMap, x, y int) bool {
	return x >= 0 && x < m.Width() && y >= 0 && y < m.Height() && !m.Blocked(x, y)
}

// makePath returns the path from the goal to the first node
// without any parent index (the start node).
func makePath(m GridMap, nodes []node, goali int) (path []Loc) {
	if nodes[goali].parent < 0 {
		return
	}

	var rev []Loc
	stride := m.Height()
	for i := goali; nodes[i].parent >= 0; i = nodes[i].parent {
		x, y := i/stride, i%stride
		rev = append(rev, Loc{x, y})
	}
	for _, loc := range rev {
		path = append(path, loc)
	}
	return
}

// node is an A* node structure.
type node struct {
	ind     int     // The closed list index of this node.
	parent  int     // The index of the parent node, -1 means no parent.
	pqindex int     // The priority queue index of the node, -1 indicates not in the queue.
	g       float64 // The cost from the start node to this node, -1 indicates unreached.
	h       float64 // Cached heuristic value, -1 means that it was yet to be computed.
	f       float64 // f = g + h
}

// openList is an open list of node pointers for use with A*.
type openList []*node

// Len returns the length of the openList.
func (o openList) Len() int {
	return len(o)
}

// Less orders the nodes in the open list in increasing
// order of f, breaking ties in favor of nodes with greater
// g values (and thus lower heuristic estimates).
func (o openList) Less(i, j int) bool {
	a, b := o[i], o[j]
	if a.f == b.f {
		return a.g > b.g
	}
	return a.f < b.f
}

// Swap swaps two elements in the open list, tracking their
// indices in the heap.
func (o openList) Swap(i, j int) {
	o[i], o[j] = o[j], o[i]
	o[i].pqindex = i
	o[j].pqindex = j
}

// Push pushes a new element to the back of the slice.
func (o *openList) Push(n interface{}) {
	*o = append(*o, n.(*node))
	(*o)[len(*o)-1].pqindex = len(*o) - 1
}

// Pop pops the last element off of the slice.
func (o *openList) Pop() interface{} {
	opn := *o
	it := opn[len(opn)-1]
	it.pqindex = -1
	*o = opn[:len(opn)-1]
	return it
}

// closedList creates a closed list for the given map.
func makeClosedList(m GridMap) []node {
	sz := m.Width() * m.Height()
	closed := make([]node, sz)
	for i := range closed {
		closed[i].ind = i
		closed[i].parent = -1
		closed[i].g = -1
		closed[i].pqindex = -1
	}
	return closed
}
