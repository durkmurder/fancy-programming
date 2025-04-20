package main

import (
	"fmt"
	"sync"
	"testing"
)

const ops = 10

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
			started.Done()
			ready.Wait()
			for range ops {
				_ = universal.Apply(threadID, func(u uint32) uint32 {
					return u + (threadID + 1)
				})
			}

			//fmt.Printf("Thread ID: %d, Decided: %v\n", threadID, response)
		}(uint32(i))
	}

	started.Wait()
	ready.Done()
	done.Wait()
	fmt.Printf("lock-free accum value: %d\n", universal.Apply(0, func(u uint32) uint32 { return u }))
}

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
			started.Done()
			ready.Wait()
			for range ops {
				_ = universal.Apply(threadID, func(u uint32) uint32 {
					return u + (threadID + 1)
				})
			}
			//fmt.Printf("Thread ID: %d, Decided: %v\n", threadID, response)
		}(uint32(i))
	}

	started.Wait()
	ready.Done()
	done.Wait()
	fmt.Printf("wait-free accum value: %d\n", universal.Apply(0, func(u uint32) uint32 { return u }))
}
