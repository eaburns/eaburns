// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.

package rbtree

import (
	"container/list"
	"math/rand"
	"testing"
	"time"
)

// Number of nodes added to the tree.
const nAdds = 100000

// Insert then remove a bunch of random numbers... not a great way to
// test.
func TestAddRemove(t *testing.T) {
	rand.Seed(int64(time.Now().Nanosecond()))
	tree := New()
	lst := list.New()
	for i := 0; i < nAdds; i += 1 {
		k := rand.Int()
		lst.PushFront(k)
		tree.Add(intKey(k), k)
	}
	if countNodes(tree.root) != tree.Len() {
		t.Errorf("Number of nodes in the tree doesn't match the Len")
	}
	ensureInvariants(t, tree)
	for e := lst.Front(); e != nil; e = e.Next() {
		k := e.Value.(int)
		n := tree.Remove(intKey(k))
		if n == nil {
			t.Errorf("%d was not found in the tree", k)
		}
	}
	ensureInvariants(t, tree)
	if countNodes(tree.root) != tree.Len() {
		t.Errorf("Number of nodes in the tree doesn't match the Len")
	}
	if tree.Len() != 0 {
		t.Errorf("Not all nodes were removed")
	}
}

func randomTree() *RbTree {
	rand.Seed(int64(time.Now().Nanosecond()))
	tree := New()
	lst := list.New()
	for i := 0; i < nAdds; i += 1 {
		k := rand.Int()
		lst.PushFront(k)
		tree.Add(intKey(k), k)
	}
	return tree
}

func TestDo(t *testing.T) {
	tree := randomTree()
	lastKey := -1
	count := 0
	inOrder := func(k Key, v interface{}) {
		i := int(k.(intKey))
		if int(k.(intKey)) < lastKey {
			t.Errorf("Out of order! %d came before %d\n",
				lastKey, i)
		}
		lastKey = i
		count += 1
	}
	tree.Do(inOrder)
	if count != tree.Len() {
		t.Errorf("Didn't encounter every element\n")
	}
}

func TestMaximumAndMinimum(t *testing.T) {
	tree := randomTree()
	max := -1
	min := -1
	iter := func(k Key, v interface{}) {
		i := int(k.(intKey))
		if min == -1 || i < min {
			min = i
		}
		if i > max {
			max = i
		}
	}
	tree.Do(iter)
	if min != tree.Minimum().Value.(int) {
		t.Errorf("Minimum is %d, Minimum() reported %d instead\n",
			min, tree.Minimum())
	}
	if max != tree.Maximum().Value.(int) {
		t.Errorf("Maximum is %d, Maximum() reported %d instead\n",
			max, tree.Maximum().Value.(int))
	}
}
