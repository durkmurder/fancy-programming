package main

import "sync/atomic"

// Consensus describes a common interface for the consensus. Properties of consensus are:
// 1) consistent: all threads decide the same value
// 2) valid: the common decision value is some thread's input
// Usually a consensus object is one-shot if not stated otherwise.
type Consensus[T any] interface {
	// Decide accepts a thread ID and a value, produces a consistent value across all callers.
	// Decide has to be called by each thread at most once.
	Decide(threadID uint32, value T) T
}

// CASConsensus implements N-thread consensus.
// It cannot be reused after one round.
type CASConsensus[T any] struct {
	proposed []T
	r        atomic.Int32
}

func NewCASConsensus[T any](n uint) *CASConsensus[T] {
	c := &CASConsensus[T]{
		proposed: make([]T, n),
		r:        atomic.Int32{},
	}
	c.r.Store(-1)
	return c
}

func (c *CASConsensus[T]) Decide(threadID uint32, value T) T {
	c.proposed[threadID] = value
	if c.r.CompareAndSwap(-1, int32(threadID)) {
		return c.proposed[threadID]
	} else {
		return c.proposed[c.r.Load()]
	}
}
