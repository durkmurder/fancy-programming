package main

import (
	"fmt"
	"sync"
	"testing"
)

func TestConsensus(t *testing.T) {
	n := uint(100)
	c := NewCASConsensus[uint32](n)

	started := sync.WaitGroup{}
	started.Add(int(n))
	ready := sync.WaitGroup{}
	ready.Add(1)
	done := sync.WaitGroup{}
	done.Add(int(n))
	for i := range n {
		go func(threadID uint32) {
			defer done.Done()
			started.Done()
			ready.Wait()
			value := c.Decide(threadID, threadID)
			fmt.Printf("Thread ID: %d, Decided: %v\n", threadID, value)
		}(uint32(i))
	}

	started.Wait()
	ready.Done()
	done.Wait()
}
