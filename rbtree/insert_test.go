// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.

package rbtree

import "testing"

func n(i int, c color, l *Node, r *Node) *Node {
	n := newNode(intKey(i), i)
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

func TestInsertIntoEmpty(t *testing.T) {
	tree := New()
	n := newNode(intKey(1), 1)
	rbInsert(false, tree, n)
	if tree.root != n {
		t.Errorf("root is not the inserted node")
	}
	if n.left != &nilNode {
		t.Errorf("Newly inserted node has a left child")
	}
	if n.right != &nilNode {
		t.Errorf("Newly inserted node has a right child")
	}
	ensureInvariants(t, tree)
}

// Figure 13.4 from CLRS ed 3 which seems to test all of the first 3
// cases in rbInsertFixup
func TestInsertCasesLeft(t *testing.T) {
	tree := New()
	tree.root =
		n(11, black,
                  n(2, red,
                    n(1, black, &nilNode, &nilNode),
                    n(7, black,
                      n(5, red,
                        n(4, red, &nilNode, &nilNode),
                        &nilNode),
                      n(8, red, &nilNode, &nilNode))),
                  n(14, black,
                    &nilNode,
                    n(15, red, &nilNode, &nilNode)))
	z := tree.root.left.right.left.left
	if z.Value.(int) != 4 {
		t.Fatalf("Node 'z' is not value 4")
	}
	rbInsertFixup(tree, z)
	if tree.root.Value.(int) != 7 {
		t.Errorf("Root value is not 7 after fixup")
	}
	if tree.root.left.Value.(int) != 2 {
		t.Errorf("Root left is not 2 after fixup")
	}
	if tree.root.right.Value.(int) != 11 {
		t.Errorf("Root right is not 11 after fixup")
	}
	if tree.root.left.left.Value.(int) != 1 {
		t.Errorf("2 left is not 1 after fixup")
	}
	if tree.root.left.right.Value.(int) != 5 {
		t.Errorf("2 right is not 5 after fixup")
	}
	if tree.root.left.right.left.Value.(int) != 4 {
		t.Errorf("5 left is not 4 after fixup")
	}
	if tree.root.right.left.Value.(int) != 8 {
		t.Errorf("11 left is not 8 after fixup")
	}
	if tree.root.right.right.Value.(int) != 14 {
		t.Errorf("11 right is not 14 after fixup")
	}
	if tree.root.right.right.right.Value.(int) != 15 {
		t.Errorf("15 right is not 15 after fixup")
	}
	ensureInvariants(t, tree)
}

// A reflection of Figure 13.4 from CLRS ed 3 over the y-axis which
// should test all of the final 3 cases in rbInsertFixup
func TestInsertCasesRight(t *testing.T) {
	tree := New()
	tree.root =
		n(11, black,
		  n(2, black,
                    n(1, red, &nilNode, &nilNode),
                    &nilNode),
	          n(16, red,
                    n(13, black,
                      n(12, red, &nilNode, &nilNode),
                      n(14, red,
                        &nilNode,
                        n(15, red, &nilNode, &nilNode))),
                    n(17, black, &nilNode, &nilNode)))
	z := tree.root.right.left.right.right
	if z.Value.(int) != 15 {
		t.Fatalf("z value is not 15")
	}
	rbInsertFixup(tree, z)

	if tree.root.Value.(int) != 13 {
		t.Errorf("Root is not 13")
	}
	if tree.root.left.Value.(int) != 11 {
		t.Errorf("13 left is not 11")
	}
	if tree.root.right.Value.(int) != 16 {
		t.Errorf("13 right is not 16")
	}
	if tree.root.left.left.Value.(int) != 2 {
		t.Errorf("11 left is not 2")
	}
	if tree.root.left.right.Value.(int) != 12 {
		t.Errorf("11 right is not 12")
	}
	if tree.root.left.left.left.Value.(int) != 1 {
		t.Errorf("2 left is not 1")
	}
	if tree.root.right.left.Value.(int) != 14 {
		t.Errorf("16 left is not 14")
	}
	if tree.root.right.left.right.Value.(int) != 15 {
		t.Errorf("14 right is not 15")
	}
	if tree.root.right.right.Value.(int) != 17 {
		t.Errorf("16 right is not 17")
	}
	ensureInvariants(t, tree)
}
