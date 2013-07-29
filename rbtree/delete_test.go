// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package rbtree

import "testing"

func s(s string, c color, l *Node, r *Node) *Node {
	n := newNode(strKey(s), s)
	n.color = c
	n.left = l
	if l != &nilNode {
		l.parent = n
	}
	n.right = r
	if r != &nilNode {
		r.parent = n
	}
	return n
}

func blk(d int) *Node {
	if d == 1 {
		return &nilNode
	}
	return s("blk", black, blk(d-1), blk(d-1))
}

func TestDeleteSingleton(t *testing.T) {
	tree := New()
	n := newNode(intKey(1), 1)
	rbInsert(false, tree, n)
	if tree.root != n {
		t.Fatalf("Node was never inserted")
	}
	rbDelete(tree, n)
	if tree.root == n {
		t.Fatalf("Node was never removed")
	}
	if tree.root != &nilNode {
		t.Fatalf("Root is not nilNode")
	}
}

func runTestCase4(t *testing.T, c_prime color, right bool) {
	tree := New()
	c := black // can't test red because this is the root
	c_blk_depth := 1
	if c_prime == red {
		c_blk_depth = 2
	}
	tree.root = s("b", c,
		s("a", black, blk(1), blk(1)),
		s("d", black, s("c", c_prime,
			blk(c_blk_depth), blk(c_blk_depth)),
			s("e", red, blk(2), blk(2))))
	if right {
		tree.root = reflect(tree.root)
	}
	rbDeleteFixup(tree, tree.root.left)
	if path("", tree, right).Value.(string) != "d" {
		t.Errorf("Root is not d")
	}
	if path("", tree, right).color != c {
		t.Errorf("Root is not d")
	}
	if path("l", tree, right).Value.(string) != "b" {
		t.Errorf("b is not left of d")
	}
	if path("r", tree, right).Value.(string) != "e" {
		t.Errorf("e is not right of d")
	}
	if path("ll", tree, right).Value.(string) != "a" {
		t.Errorf("a is not left of b")
	}
	if path("lr", tree, right).Value.(string) != "c" {
		t.Errorf("c is not right of b")
	}
	if path("lr", tree, right).color != c_prime {
		t.Errorf("color of c changed")
	}

	ensureInvariants(t, tree)
}

func TestDeleteCase4(t *testing.T) {
	runTestCase4(t, black, false)
	runTestCase4(t, red, false)
	runTestCase4(t, black, false)
	runTestCase4(t, red, false)
}

func runTestCase3(t *testing.T, right bool) {
	tree := New()
	tree.root = s("b", black,
		s("a", black, blk(1), blk(1)),
		s("d", black, s("c", red, blk(2), blk(2)),
			s("e", black, blk(1), blk(1))))
	if right {
		tree.root = reflect(tree.root)
	}
	x := path("l", tree, right)
	if x.Value.(string) != "a" {
		t.Errorf("node is not a")
	}
	rbDeleteFixup(tree, x)
	if path("", tree, right).Value.(string) != "c" {
		t.Errorf("Root is not c")
	}
	if path("l", tree, right).Value.(string) != "b" {
		t.Errorf("c left is not b")
	}
	if path("r", tree, right).Value.(string) != "d" {
		t.Errorf("c right is not d")
	}
	if path("rr", tree, right).Value.(string) != "e" {
		t.Errorf("d right is not e")
	}
	if path("ll", tree, right).Value.(string) != "a" {
		t.Errorf("b right is not a")
	}
	ensureInvariants(t, tree)
}

func TestDeleteCase3(t *testing.T) {
	runTestCase3(t, false)
	runTestCase3(t, true)
}
