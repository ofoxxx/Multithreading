# Thread pool
Classic implementation of thread pool.<br>
Not wait-free [by reasons](https://stackoverflow.com/questions/26944538/lock-free-thread-pool).
Wait-free queue may be used internally but it gives no significant performance boost
