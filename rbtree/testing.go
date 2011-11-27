// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.
/*
 * Utility functions for testing the RB tree
 */
package rbtree

import (
	"testing"
	"fmt"
)

func emptyNode() *Node {
	n := new(Node)
	n.left = &nilNode
	n.right = &nilNode
	n.parent = &nilNode
	return n
}


type intKey int

func (a intKey) Compare(bKey Key) int {
	b := bKey.(intKey)
	if int(a) > int(b) {
		return 1
	} else if int(a) < int(b) {
		return -1
	}
	return 0
}

type strKey string

func (a strKey) Compare(bKey Key) int {
	b := bKey.(strKey)
	if string(a) < string(b) {
		return -1
	} else if string(a) > string(b) {
		return 1
	}
	return 0
}

func ensureInvariants(t *testing.T, tree *RbTree) {
	if tree.root.color != black {
		t.Errorf("Root node is not colored black")
	}
	redOrBlack(t, tree.root)
	redsFollowedByBlacks(t, tree.root)
	sameBlackHeight(t, tree.root)
}

func redOrBlack(t *testing.T, n *Node) {
	if n == &nilNode {
		if n.color != black {
			t.Errorf("nilNode is not black")
		}
		return
	}
	if n.color != red && n.color != black {
		t.Errorf("A none is neither red or black")
	}
	redOrBlack(t, n.left)
	redOrBlack(t, n.right)
}

func redsFollowedByBlacks(t *testing.T, n *Node) {
	if n == &nilNode {
		return
	}
	if n.color == red {
		if n.left.color != black || n.right.color != black {
			t.Errorf("Red node not followed by black nodes")
		}
	}
	redsFollowedByBlacks(t, n.left)
	redsFollowedByBlacks(t, n.right)
}

func sameBlackHeight(t *testing.T, n *Node) int {
	if n == &nilNode {
		return 1
	}

	l := sameBlackHeight(t, n.left)
	r := sameBlackHeight(t, n.right)
	if l != r {
		t.Errorf("Unbalanced black-height")
	}
	blkHt := l
	if n.color == black {
		blkHt += 1
	}
	return blkHt
}

func dump(lvl int, n *Node) {
	for i := 0; i < lvl; i += 1 {
		fmt.Printf(" ")
	}
	if n == &nilNode {
		fmt.Printf("<nil>, black\n")
		return
	}
	color := "black"
	if n.color == red {
		color = "red"
	}
	fmt.Printf("%v, %s\n", n.Key, color)
	dump(lvl + 1, n.left)
	dump(lvl + 1, n.right)
}

func reflect(n *Node) *Node {
	if n == &nilNode {
		return n
	}
	m := newNode(n.Key, n.Value)
	m.color = n.color
	m.left = reflect(n.right)
	m.right = reflect(n.left)
	m.left.parent = m
	m.right.parent = m
	m.parent = &nilNode
	return m
}

func followPath(i int, s string, n *Node, reflect bool) *Node {
	if i == len(s) {
		return n
	}
	if s[i] == 'l' {
		if reflect {
			return followPath(i + 1, s, n.right, reflect)
		} else {
			return followPath(i + 1, s, n.left, reflect)
		}
	} else if s[i] == 'r' {
		if reflect {
			return followPath(i + 1, s, n.left, reflect)
		} else {
			return followPath(i + 1, s, n.right, reflect)
		}
	}
	return nil
}

func path(s string, tree *RbTree, reflect bool) *Node {
	return followPath(0, s, tree.root, reflect)
}

func countNodes(n *Node) int {
	if n == &nilNode {
		return 0
	}
	return 1 + countNodes(n.left) + countNodes(n.right)
}

func height(n *Node) int {
	if n == &nilNode {
		return 0
	}
	l := height(n.left)
	r := height(n.right)
	max := l
	if r > l {
		max = r
	}
	return 1 + max
}
