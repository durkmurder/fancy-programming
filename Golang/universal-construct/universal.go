package main

import (
	"slices"
)

const nThreads = 5

// Node is a node in a linked list which holds a sequential number,
// consensus object and invocation that has to be applied at each step.
type Node[SeqObject any] struct {
	invocation func(SeqObject) SeqObject
	decideNext Consensus[*Node[SeqObject]]
	next       *Node[SeqObject]
	seq        uint32
}

// NewNode initializes new node with some invocation
func NewNode[SeqObject any](invocation func(SeqObject) SeqObject) *Node[SeqObject] {
	return &Node[SeqObject]{
		invocation: invocation,
		decideNext: NewCASConsensus[*Node[SeqObject]](nThreads),
		next:       nil,
		seq:        0,
	}
}

// MaxNode returns node with the highest sequence number.
func MaxNode[SeqObject any](nodes []*Node[SeqObject]) *Node[SeqObject] {
	return slices.MaxFunc(nodes, func(a, b *Node[SeqObject]) int {
		return int(a.seq) - int(b.seq)
	})
}

// LFUniversal allows to perform a lineriazable execution over a sequential object with a lock-free algorithm.
// All operations needs to be deterministic and order independent since there is no way to ensure global order.
type LFUniversal[SeqObject any] struct {
	head    []*Node[SeqObject]
	tail    *Node[SeqObject]
	initial SeqObject
}

// NewLFUniversal accepts an initial state and creates a universal executor.
func NewLFUniversal[SeqObject any](initial SeqObject) *LFUniversal[SeqObject] {
	tail := NewNode(func(object SeqObject) SeqObject {
		return object
	})
	tail.seq = 1
	head := make([]*Node[SeqObject], nThreads)
	for i := range nThreads {
		head[i] = tail
	}
	return &LFUniversal[SeqObject]{
		head:    head,
		tail:    tail,
		initial: initial,
	}
}

// Apply applies given invocation to the current object state.
// calling Apply yields next results:
// Apply(0, func(u uint32) uint32 { return u + 1 } == 1
// Apply(1, func(u uint32) uint32 { return u + 2 } == 3
// Apply(1, func(u uint32) uint32 { return u + 3 } == 6
// Apply(2, func(u uint32) uint32 { return u } == 6
// It guarantees that execution will be lineriazable, meaning it respects the runtime and program order.
// All operations will be applied atomically to the seq object.
// If two calls to Apply are happening at the same time, there is no particular order defined.
func (u *LFUniversal[SeqObject]) Apply(threadID uint32, invocation func(SeqObject) SeqObject) SeqObject {
	prefer := NewNode[SeqObject](invocation)
	for prefer.seq == 0 {
		before := MaxNode(u.head)
		after := before.decideNext.Decide(threadID, prefer)
		before.next = after
		after.seq = before.seq + 1
		u.head[threadID] = after
	}

	cpy := u.initial
	current := u.tail.next
	for current != prefer {
		cpy = current.invocation(cpy)
		current = current.next
	}
	return invocation(cpy)
}

// WFUniversal allows to perform a lineriazable execution over a sequential object with a wait-free algorithm, meaning
// that each call to Apply finishes in finite number of steps.
// All operations needs to be deterministic and order independent since there is no way to ensure global order.
type WFUniversal[SeqObject any] struct {
	head     []*Node[SeqObject]
	announce []*Node[SeqObject]
	tail     *Node[SeqObject]
	initial  SeqObject
}

// NewWFUniversal accepts an initial state and creates a universal executor.
func NewWFUniversal[SeqObject any](initial SeqObject) *WFUniversal[SeqObject] {
	tail := NewNode(func(object SeqObject) SeqObject {
		return object
	})
	tail.seq = 1
	head := make([]*Node[SeqObject], nThreads)
	announce := make([]*Node[SeqObject], nThreads)
	for i := range nThreads {
		head[i] = tail
		announce[i] = tail
	}
	return &WFUniversal[SeqObject]{
		head:     head,
		announce: announce,
		tail:     tail,
		initial:  initial,
	}
}

// Apply applies given invocation to the current object state.
// calling Apply yields next results:
// Apply(0, func(u uint32) uint32 { return u + 1 } == 1
// Apply(1, func(u uint32) uint32 { return u + 2 } == 3
// Apply(1, func(u uint32) uint32 { return u + 3 } == 6
// Apply(2, func(u uint32) uint32 { return u } == 6
// It guarantees that execution will be lineriazable, meaning it respects the runtime and program order.
// All operations will be applied atomically to the seq object.
// If two calls to Apply are happening at the same time, there is no particular order defined.
func (u *WFUniversal[SeqObject]) Apply(threadID uint32, invocation func(SeqObject) SeqObject) SeqObject {
	u.announce[threadID] = NewNode[SeqObject](invocation)
	u.head[threadID] = MaxNode(u.head)
	for u.announce[threadID].seq == 0 {
		before := u.head[threadID]
		help := u.announce[(before.seq+1)%nThreads]
		prefer := u.announce[threadID]
		if help.seq == 0 {
			prefer = help
		}
		after := before.decideNext.Decide(threadID, prefer)
		before.next = after
		after.seq = before.seq + 1
		u.head[threadID] = after
	}
	u.head[threadID] = u.announce[threadID]
	cpy := u.initial
	current := u.tail.next
	for current != u.announce[threadID] {
		cpy = current.invocation(cpy)
		current = current.next
	}
	return invocation(cpy)
}
