package main

import (
	"fmt"
	"sync"
	"testing"
)

const ops = 10

// TestLFUniversal emulates an execution where multiple goroutines try to modify seq object at the same time.
// A lock-free universal construction is used to produce a lineriazable execution.
func TestLFUniversal(t *testing.T) {
	universal := NewLFUniversal[uint32](0)

	started := sync.WaitGroup{}
	started.Add(nThreads)
	ready := sync.WaitGroup{}
	ready.Add(1)
	done := sync.WaitGroup{}
	done.Add(nThreads)
	for i := range nThreads {
		go func(threadID uint32) {
			defer done.Done()
			// signal that we have started
			started.Done()
			// wait for coordinated start
			ready.Wait()

			for range ops {
				_ = universal.Apply(threadID, func(u uint32) uint32 {
					return u + (threadID + 1)
				})
			}
		}(uint32(i))
	}

	// wait for all workers to start
	started.Wait()
	// signal about coordinated start
	done.Wait()
	fmt.Printf("lock-free accum value: %d\n", universal.Apply(0, func(u uint32) uint32 { return u }))
}

// TestWFUniversal emulates an execution where multiple goroutines try to modify seq object at the same time.
// A wait-free universal construction is used to produce a lineriazable execution.
func TestWFUniversal(t *testing.T) {
	universal := NewWFUniversal[uint32](0)

	started := sync.WaitGroup{}
	started.Add(nThreads)
	ready := sync.WaitGroup{}
	ready.Add(1)
	done := sync.WaitGroup{}
	done.Add(nThreads)
	for i := range nThreads {
		go func(threadID uint32) {
			defer done.Done()
			// signal that we have started
			started.Done()
			// wait for coordinated start
			ready.Wait()

			for range ops {
				_ = universal.Apply(threadID, func(u uint32) uint32 {
					return u + (threadID + 1)
				})
			}
		}(uint32(i))
	}

	// wait for all workers to start
	started.Wait()
	// signal about coordinated start
	ready.Done()
	done.Wait()
	fmt.Printf("wait-free accum value: %d\n", universal.Apply(0, func(u uint32) uint32 { return u }))
}
