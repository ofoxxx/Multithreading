# Thread pool
Classic implementation of thread pool and wait-free variant.

# Preamble
There are [reasons](https://stackoverflow.com/questions/26944538/lock-free-thread-pool) why to prefer thread pool realisation based on condition_variable and mutex.<br>
But let's try to implement lock/wait-free and compare.

# Synchronized
Classic implementation of thread pool. It's optimal, does not make busy-loops, robust and debug friendly. 

# Wait-free
Is baised on simplified FIFO queue(linked list) with CAS push_back/pop_front methods. This thread pool dynamicaly creates threads under load (does not precreate threads nor keeps sleeping workers) but it's requere managing thread

Conslusion
==========
Test shows no significant performance boost of wait-free over synchronized thread pool.
