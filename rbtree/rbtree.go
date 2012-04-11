// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.

// A self-balancing binary tree that can be used as an ordered map from
// keys to values.
//
// The implementation of this red-black tree is directly from the 3rd
// edition of "Introduction to Algorithms" by Cormen, Leiserson, Rivest
// and Stein.
package rbtree

/* In my opinion, some of this code is really terrible looking.  The
reason is that it is a direct translation from CLRS (see package
comment).  The disadvantage is that the code is difficult to read on
its own.  The advantages are: 1) well tested algorithm, 2) matches
the pseudo-code in CLRS and therefore the book documents the code
(variable names and everything). */

type color bool

// Key is the interface for the keys associated with data in the tree.
type Key interface {
	// Compare checks the ordering of two keys.  It returns
	// <0 if the receiver is less than the argument, >0 if the
	// receiver is greater and 0 if they are equal.
	Compare(Key) int
}

// Node represents a node in the tree.  It can be used as an opaque
// handle to the tree node holding the association between a Key
// and its data.
type Node struct {
	parent *Node
	left   *Node
	right  *Node
	color  color
	Key    Key
	Value  interface{}
}

// RbTree is a red-black tree that can map Keys to values of type
// interface{}.
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

// New returns a new empty tree.
func New() *RbTree {
	return &RbTree{root: &nilNode}
}

// Len returns the number of key/value mappings in the tree.  This operation
// is constant in the number of Nodes in the tree.
func (t *RbTree) Len() int {
	return t.len
}

// Find returns a pointer to a node that associates the matching Key
// to a value or nil if there is no mapping for the given Key in the tree.
// This operation is O(lg n) in the number of Nodes in the tree.
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

// Member tests if there is a value bound to the given Key in the tree.
func (t *RbTree) Member(k Key) bool {
	return t.Find(k) != nil
}

// Do calls the function on each Key/value pair in the tree.  The function
// is called on the bindings in the ordering defined over the Keys. The
// behavior of Do is undefined defined if the tree is modified by the
// function f.  The operation is O(n) in the number of Nodes in the tree.
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

// Add inserts a new mapping into the tree from the given
// key to the given value and returns a pointer to the Node
// associated with this binding.  This operation is O(lg n) in
// the number of Nodes in the tree.
func (t *RbTree) Add(k Key, v interface{}) *Node {
	n := newNode(k, v)
	t.len += 1
	return rbInsert(false, t, n)
}

// Reinsert re-inserts the given Node into the tree.  The Node must
// represent a node that has been removed from the tree.  This
// operation is O(lg n) in the number of Nodes in the tree.
func (t *RbTree) Reinsert(n *Node) {
	if n != nil {
		n.left = &nilNode
		n.right = &nilNode
		n.parent = &nilNode
		t.len += 1
		rbInsert(false, t, n)
	}
}

// Update updates the position of the given Node in the tree.
// This should be called whenever the key is changed and the
// Node is still in the tree.  This operation is O(lg n) in the number
// of Nodes in the tree.
func (t *RbTree) UpdateKey(n *Node) {
	if n != nil {
		t.RemoveNode(n)
		t.Reinsert(n)
	}
}

// Replace looks for a binding for the given Key and returns a pointer
// to the node for the replaced or new binding.  If a binding is
// found then the value to which it refers is replaced with the new
// value.  If there is no value previously bound to the Key then
// a new binding is added.  This operation is O(lg n) in the number
// of Nodes in the tree.
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

// Minimum returns a pointer to the node in the tree that holds the
// minimum key according to the comparison function over Keys.
// This operation is O(lg n) in the number of Nodes in the tree.
func (t *RbTree) Minimum() *Node {
	return treeMinimum(t.root)
}

func treeMinimum(x *Node) *Node {
	for ; x.left != &nilNode; x = x.left {
	}
	return x
}

// Maximum returns a pointer to the node in the tree that holds the
// maximum key according to the comparison function over Keys.
// This operation is O(lg n) in the number of Nodes in the tree.
func (t *RbTree) Maximum() *Node {
	return treeMaximum(t.root)
}

func treeMaximum(x *Node) *Node {
	for ; x.right != &nilNode; x = x.right {
	}
	return x
}

// Remove attempts to remove a binding from Key.  If there is no such
// binding then nil is returned otherwise a pointer to the Node for the
// removed binding is returned.  This operation is O(lg n) in the number
// of Nodes in the tree.
func (t *RbTree) Remove(k Key) *Node {
	n := t.Find(k)
	if n != nil {
		t.len -= 1
		rbDelete(t, n)
	}
	return n
}

// RemoveNode removes the given Node from the tree.  This operation
// is O(lg n) in the number of Nodes in the tree but can often be much
// faster than calling Remove on the Key as this method elides the
// loopkup step to find the Node.
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

// Copy returns a copy of the given tree.
func (t *RbTree) Copy() *RbTree {
	return &RbTree{root: copy(t.root), len: t.len}
}
