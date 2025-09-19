# Process Synchronization

Concurrent access to shared data may result in data incosistency.

Example: Say you are working in a group projec of two. Imagine the two students are cooperating processes. Both does 50% of the assignment. If one of them doesnt do 50% then even your grade would be affected. Then saying, you spent 4hrs of your thinking on the introduction but then you go to the google doc and oyu realize that it is already there. This is problem with concurrent access, one person needs to know what the other person is doing. 

Say you copied your introduction and just overwrote the current introduction. That is data inconsistency because the other person wouldnt know.

# Race Condition
```
counter = 5 (shared)
counter++ (counter = 6) -> P1
counter-- (counter = 5) -> P2
```

They are running parallely

Whoever loses the race is the one that gets the final value.



# Producer Consumer Problem
Also known as bounded buffer problem


producer:
- buffer_size = 5
- in = 0
counter = 0 

[ 1 | | | | | ]

while (counter == buffer_size), it will be stuck here if it equals 1. 


# Critical Section


--- 

# TBD