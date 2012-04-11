// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.

package rbtree

import "testing"

// Example from CLRS ed 3 Figure 13.2
func TestLeftRotateRoot(t *testing.T) {
	tree := New()
	y := emptyNode()
	x := emptyNode()
	alph := emptyNode()
	beta := emptyNode()
	gamma := emptyNode()
	tree.root = x
	x.left = alph
	alph.parent = x
	x.right = y
	y.parent = x
	y.left = beta
	beta.parent = y
	y.right = gamma
	gamma.parent = y

	leftRotate(tree, x)
	if tree.root != y {
		t.Fatalf("y did not become the root")
	}
	if y.left != x {
		t.Fatalf("y left is not x")
	}
	if y.right != gamma {
		t.Fatalf("y right is not gamma")
	}
	if x.left != alph {
		t.Fatalf("x left is not alpha")
	}
	if x.right != beta {
		t.Fatalf("x right is not beta")
	}
}

// Example from CLRS ed 3 Figure 13.2
func TestRightRotateRoot(t *testing.T) {
	tree := New()
	y := emptyNode()
	x := emptyNode()
	alph := emptyNode()
	beta := emptyNode()
	gamma := emptyNode()
	tree.root = y
	y.left = x
	x.parent = y
	y.right = gamma
	gamma.parent = y
	x.left = alph
	alph.parent = x
	x.right = beta
	beta.parent = x

	rightRotate(tree, y)
	if tree.root != x {
		t.Fatalf("x did not become the root")
	}
	if x.left != alph {
		t.Fatalf("x left is not alpha")
	}
	if x.right != y {
		t.Fatalf("x right is not y")
	}
	if y.left != beta {
		t.Fatalf("y left is not beta")
	}
	if y.right != gamma {
		t.Fatalf("y right is not gamma")
	}
}


// Example from CLRS ed 3 Figure 13.2
func TestLeftRotateInternal(t *testing.T) {
	tree := New()
	top := emptyNode()
	y := emptyNode()
	x := emptyNode()
	alph := emptyNode()
	beta := emptyNode()
	gamma := emptyNode()
	tree.root = top
	top.left = x
	x.parent = top
	x.left = alph
	alph.parent = x
	x.right = y
	y.parent = x
	y.left = beta
	beta.parent = y
	y.right = gamma
	gamma.parent = y

	leftRotate(tree, x)
	if tree.root != top {
		t.Fatalf("tree root changed")
	}
	if top.left != y {
		t.Fatalf("top left did not become y")
	}
	if y.left != x {
		t.Fatalf("y left is not x")
	}
	if y.right != gamma {
		t.Fatalf("y right is not gamma")
	}
	if x.left != alph {
		t.Fatalf("x left is not alpha")
	}
	if x.right != beta {
		t.Fatalf("x right is not beta")
	}
}

// Example from CLRS ed 3 Figure 13.2
func TestRightRotateInternal(t *testing.T) {
	tree := New()
	top := emptyNode()
	y := emptyNode()
	x := emptyNode()
	alph := emptyNode()
	beta := emptyNode()
	gamma := emptyNode()
	tree.root = top
	top.left = y
	y.parent = top
	y.left = x
	x.parent = y
	y.right = gamma
	gamma.parent = y
	x.left = alph
	alph.parent = x
	x.right = beta
	beta.parent = x

	rightRotate(tree, y)
	if tree.root != top {
		t.Fatalf("tree root changed")
	}
	if top.left != x {
		t.Fatalf("top left did not become x")
	}
	if x.left != alph {
		t.Fatalf("x left is not alpha")
	}
	if x.right != y {
		t.Fatalf("x right is not y")
	}
	if y.left != beta {
		t.Fatalf("y left is not beta")
	}
	if y.right != gamma {
		t.Fatalf("y right is not gamma")
	}
}
