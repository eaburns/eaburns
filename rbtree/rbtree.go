// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.
/*
 A self-balancing binary tree.  The implementation of this red-black
 tree is directly from the 3rd edition of "Introduction to Algorithms"
 by Cormen, Leiserson, Rivest and Stein.
*/
package rbtree

/* In my opinion, some of this code is really terrible looking.  The
reason is that it is a direct translation from CLRS (see package
comment).  The disadvantage is that the code is difficult to read on
its own.  The advantages are: 1) well tested algorithm, 2) matches
the pseudo-code in CLRS and therefore the book documents the code
(variable names and everything). */

type color bool

type Key interface {
	// standard c-style compare: 0 is equal, <0 means reciever is
	// less, >0 means receiver is greater
	Compare(Key) int
}

type Node struct {
	parent *Node
	left   *Node
	right  *Node
	color  color
	Key    Key
	Value  interface{}
}

type RbTree struct {
	root *Node
	len  int
}

const (
	red   color = true
	black color = false
)

var nilNode Node

func init() {
	nilNode.color = black
	nilNode.parent = &nilNode
	nilNode.left = &nilNode
	nilNode.right = &nilNode
}

func newNode(k Key, v interface{}) *Node {
	return &Node{left: &nilNode, right: &nilNode, parent: &nilNode, Key: k, Value: v}
}

// Creates a new empty tree.
func New() *RbTree {
	return &RbTree{root: &nilNode}
}

// Retrieve the number of nodes in the tree in constant time.
func (t *RbTree) Len() int {
	return t.len
}

// Find the first value bound to the given key.  Returns nil if it is
// not found.  O(lg n)
func (t *RbTree) Find(k Key) *Node {
	x := t.root
	for x != &nilNode {
		switch cmp := k.Compare(x.Key); {
		case cmp == 0:
			return x
		case cmp < 0:
			x = x.left
		default:
			x = x.right
		}
	}
	return nil
}

// Test if the key is in the tree.  O(lg n)
func (t *RbTree) Member(k Key) bool {
	return t.Find(k) != nil
}

// Call the function on each key/value pair in order of the keys.  The
// behavior is not well defined if the tree is modified by the
// function f.  O(n)
func (t *RbTree) Do(f func(k Key, v interface{})) {
	inOrder(t.root, f)
}

func inOrder(n *Node, f func(k Key, v interface{})) {
	if n != &nilNode {
		inOrder(n.left, f)
		f(n.Key, n.Value)
		inOrder(n.right, f)
	}
}

// Inserts a binding of v to k in the tree. O(lg n)
//
// The return value is the node associated with the binding.
func (t *RbTree) Add(k Key, v interface{}) *Node {
	n := newNode(k, v)
	t.len += 1
	return rbInsert(false, t, n)
}

// Reinsert the given node back into the tree.  O(lg n)
func (t *RbTree) Reinsert(n *Node) {
	if n != nil {
		n.left = &nilNode
		n.right = &nilNode
		n.parent = &nilNode
		t.len += 1
		rbInsert(false, t, n)
	}
}

// Updates the position of the node in the tree.  This should be
// called whenever the key is changed and n is still in the tree.
// O(lg n)
func (t *RbTree) UpdateKey(n *Node) {
	if n != nil {
		t.RemoveNode(n)
		t.Reinsert(n)
	}
}

// Replaces a binding to k with the value v or inserts a new binging
// if there wasn't one previously.  O(lg n)
//
// The return value is the node associated with the binding.
func (t *RbTree) Replace(k Key, v interface{}) *Node {
	n := newNode(k, v)
	m := rbInsert(true, t, n)
	if m == n {
		t.len += 1
	}
	return m
}

func leftRotate(t *RbTree, x *Node) {
	y := x.right
	x.right = y.left
	if y.left != &nilNode {
		y.left.parent = x
	}
	y.parent = x.parent
	switch {
	case x.parent == &nilNode:
		t.root = y
	case x == x.parent.left:
		x.parent.left = y
	default:
		x.parent.right = y
	}
	y.left = x
	x.parent = y
}

func rightRotate(t *RbTree, x *Node) {
	y := x.left
	x.left = y.right
	if y.right != &nilNode {
		y.right.parent = x
	}
	y.parent = x.parent
	switch {
	case x.parent == &nilNode:
		t.root = y
	case x == x.parent.right:
		x.parent.right = y
	default:
		x.parent.left = y
	}
	y.right = x
	x.parent = y
}

// If replace is true then an equal element found in the tree will be
// replaced with the value from z instead of adding z.
func rbInsert(replace bool, t *RbTree, z *Node) *Node {
	y := &nilNode
	x := t.root
	for x != &nilNode {
		y = x
		switch cmp := z.Key.Compare(x.Key); {
		case replace && cmp == 0:
			x.Value = z.Value
			return x
		case cmp < 0:
			x = x.left
		default:
			x = x.right
		}
	}
	z.parent = y
	switch {
	case y == &nilNode:
		t.root = z
	case z.Key.Compare(y.Key) < 0:
		y.left = z
	default:
		y.right = z
	}
	z.color = red
	rbInsertFixup(t, z)
	return z
}

func rbInsertFixup(t *RbTree, z *Node) {
	for z.parent.color == red {
		if z.parent == z.parent.parent.left {
			y := z.parent.parent.right
			if y.color == red {
				z.parent.color = black
				y.color = black
				z.parent.parent.color = red
				z = z.parent.parent
			} else {
				if z == z.parent.right {
					z = z.parent
					leftRotate(t, z)
				}
				z.parent.color = black
				z.parent.parent.color = red
				rightRotate(t, z.parent.parent)
			}
		} else {
			y := z.parent.parent.left
			if y.color == red {
				z.parent.color = black
				y.color = black
				z.parent.parent.color = red
				z = z.parent.parent
			} else {
				if z == z.parent.left {
					z = z.parent
					rightRotate(t, z)
				}
				z.parent.color = black
				z.parent.parent.color = red
				leftRotate(t, z.parent.parent)
			}
		}
	}
	t.root.color = black
}

// The minimum key in the tree. O(lg n)
func (t *RbTree) Minimum() *Node {
	return treeMinimum(t.root)
}

func treeMinimum(x *Node) *Node {
	for ; x.left != &nilNode; x = x.left {
	}
	return x
}

// The maximum key in the tree. O(lg n)
func (t *RbTree) Maximum() *Node {
	return treeMaximum(t.root)
}

func treeMaximum(x *Node) *Node {
	for ; x.right != &nilNode; x = x.right {
	}
	return x
}

// Remove a node bound to the given key from the tree. O(lg n)
func (t *RbTree) Remove(k Key) *Node {
	n := t.Find(k)
	if n != nil {
		t.len -= 1
		rbDelete(t, n)
	}
	return n
}

// Remove the given node from the tree. O(lg n) but often much faster
// than calling Remove on the key and value.
func (t *RbTree) RemoveNode(n *Node) {
	if n != nil {
		t.len -= 1
		rbDelete(t, n)
	}
}

func rbDelete(t *RbTree, z *Node) {
	var x *Node
	y := z
	yOriginalColor := y.color
	switch {
	case z.left == &nilNode:
		x = z.right
		rbTransplant(t, z, z.right)
	case z.right == &nilNode:
		x = z.left
		rbTransplant(t, z, z.left)
	default:
		y = treeMinimum(z.right)
		yOriginalColor = y.color
		x = y.right
		if y.parent == z {
			x.parent = y
		} else {
			rbTransplant(t, y, y.right)
			y.right = z.right
			y.right.parent = y
		}
		rbTransplant(t, z, y)
		y.left = z.left
		y.left.parent = y
		y.color = z.color
	}
	if yOriginalColor == black {
		rbDeleteFixup(t, x)
	}
}

func rbTransplant(t *RbTree, u *Node, v *Node) {
	switch {
	case u.parent == &nilNode:
		t.root = v
	case u == u.parent.left:
		u.parent.left = v
	default:
		u.parent.right = v
	}
	v.parent = u.parent
}

func rbDeleteFixup(t *RbTree, x *Node) {
	for x != t.root && x.color == black {
		if x == x.parent.left {
			w := x.parent.right
			if w.color == red {
				w.color = black
				x.parent.color = red
				leftRotate(t, x.parent)
				w = x.parent.right
			}
			if w.left.color == black && w.right.color == black {
				w.color = red
				x = x.parent
			} else {
				if w.right.color == black {
					w.left.color = black
					w.color = red
					rightRotate(t, w)
					w = x.parent.right
				}
				w.color = x.parent.color
				x.parent.color = black
				w.right.color = black
				leftRotate(t, x.parent)
				x = t.root
			}
		} else {
			w := x.parent.left
			if w.color == red {
				w.color = black
				x.parent.color = red
				rightRotate(t, x.parent)
				w = x.parent.left
			}
			if w.right.color == black && w.left.color == black {
				w.color = red
				x = x.parent
			} else {
				if w.left.color == black {
					w.right.color = black
					w.color = red
					leftRotate(t, w)
					w = x.parent.left
				}
				w.color = x.parent.color
				x.parent.color = black
				w.left.color = black
				rightRotate(t, x.parent)
				x = t.root
			}
		}
	}
	x.color = black
}

func copy(n *Node) *Node {
	if n == &nilNode {
		return n
	}
	m := newNode(n.Key, n.Value)
	l := copy(n.left)
	l.parent = m
	r := copy(n.right)
	r.parent = m
	return m
}

// Return a copy of the given tree.
func (t *RbTree) Copy() *RbTree {
	return &RbTree{root: copy(t.root), len: t.len}
}
