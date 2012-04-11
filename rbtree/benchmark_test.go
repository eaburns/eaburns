// Copyright 2011 Ethan Burns
// Use of this source code is governed by a BSD-style
// license that can be foudn in the LICENSE file.

package rbtree

import (
	"math/rand"
	"testing"
	"time"
)

func randomInts(b *testing.B) []int {
	rand.Seed(int64(time.Now().Nanosecond()))
	ints := make([]int, b.N)
	for i := 0; i < b.N; i += 1 {
		k := rand.Int()
		ints[i] = k
	}
	return ints
}

func BenchmarkAdd(b *testing.B) {
	b.StopTimer()
	tree := New()
	ints := randomInts(b)
	b.StartTimer()
	for i := 0; i < len(ints); i += 1 {
		tree.Add(intKey(ints[i]), ints[i])
	}
}

func BenchmarkRemove(b *testing.B) {
	b.StopTimer()
	tree := New()
	ints := randomInts(b)
	for i := 0; i < len(ints); i += 1 {
		tree.Add(intKey(ints[i]), ints[i])
	}
	b.StartTimer()
	for i := 0; i < len(ints); i += 1 {
		tree.Remove(intKey(ints[i]))
	}
}

func BenchmarkRemoveNode(b *testing.B) {
	b.StopTimer()
	tree := New()
	nodes := make([]*Node, b.N)
	for i := 0; i < len(nodes); i += 1 {
		k := rand.Int()
		nodes[i] = tree.Add(intKey(k), k)
	}
	b.StartTimer()
	for i := 0; i < len(nodes); i += 1 {
		tree.RemoveNode(nodes[i])
	}
}

// The following tests match the benchmarks in GoLLRB for comparison.

func BenchmarkInsert(b *testing.B) {
	tree := New()
	for i := 0; i < b.N; i++ {
		tree.Replace(intKey(b.N-i), b.N-i)
	}
}

func BenchmarkDelete(b *testing.B) {
	b.StopTimer()
	tree := New()
	for i := 0; i < b.N; i++ {
		tree.Replace(intKey(b.N-i), b.N-i)
	}
	b.StartTimer()
	for i := 0; i < b.N; i++ {
		tree.Remove(intKey(i))
	}
}
