# CSF Assignment #3 (Matthew Flynn and Andrew Suh)

## **Part B (cache performance)**

To determine the best cache configuration, we conducted a series of tests for each parameter. We tried to isolate one variable to see the correlation between varying one configuration and the affect on performance.  

<br/>

---

<br/>

**Eviction**: To determine wether lru or fifo was better, we recorded the cache simulation output with varying eviction styles while keeping all other configurations the same. It possible to see that lru has a better hit rate for stores. This will be true most cases, because evicting the least recently used block will help the program keep blocks that are more likely to be accessed in the future, which will increase the hit rate. 

| # sets | # blocks | # bytes | WA/NWA | WT/WB | eviction | total loads | total stores | load hits | load misses | store hits | store misses | total cycles |
|-|-|-|-|-|-|-|-|-|-|-|-|-|
| 256 | 16 | 64 | WA | WT | lru | ﻿318197 | 197486 | 317198 | 999 | 195109 | 2377 | 25663541 |
| 256 | 16 | 64 | WA | WT | fifo | ﻿318197 | 197486 | 317193 | 1004 | 195109 | 2377 | 25671546 |

<br/>

---

<br/>

**Write Policy**: Here, we determined the performance difference between write-through and write-back policies. Write-through will always write to the cache and memory on a store. Write-back will write the cache only on a store, and write to memory only when a block is evicted. Therefore we know that the hit and miss rates will be the same, however, the total cycles will differ significantly because write-back will minimize the number of times written to memory. Write-back will have faster performance. 

| # sets | # blocks | # bytes | WA/NWA | WT/WB | eviction | total loads | total stores | load hits | load misses | store hits | store misses | total cycles |
|-|-|-|-|-|-|-|-|-|-|-|-|-|
| 256 | 16 | 64 | WA | WT | lru | ﻿318197 | 197486 | 317198 | 999 | 195109 | 2377 | 25663541 |
| 256 | 16 | 64 | WA | WB | lru | ﻿318197 | 197486 | 317198 | 999 | 195109 | 2377 | 6016518 |

<br/>

---

<br/>

**Write Policy on Miss**: The difference between write-allocate and no-write-allocate is a bit more complicated. A write-allocate cache will bring the relevant memory block into the cache before a store proceeds, while a no-write-allocate cache will not modify the cache. The performance difference here depends on the type of data coming into the cache. If the written data is likely to be accessed by the program in the near future, write-allocate is more advantageous because it will already have the block in the cache for easy access. No-write-allocate will be better if the written data is not to be accessed in the near future because it does not modify the cache or read from memory to put the block in the cache. However, we also need to conisder that no-write-allocate cannot be paired with write-back. Becuase write-back gives such a significant cycle count advantage, we have concluded that write-allocate in combination with write-back is the best combination. 

| # sets | # blocks | # bytes | WA/NWA | WT/WB | eviction | total loads | total stores | load hits | load misses | store hits | store misses | total cycles |
|-|-|-|-|-|-|-|-|-|-|-|-|-|
| 256 | 16 | 64 | WA | WB | lru | ﻿318197 | 197486 | 317198 | 999 | 195109 | 2377 | 6016518 |
| 256 | 16 | 64 | NWA | WT | lru | ﻿318197 | 197486 | 315894 | 2303 | 173709 | 23777 | 23925324 |
| 256 | 16 | 64 | WA | WT | lru | ﻿318197 | 197486 | 317198 | 999 | 195109 | 2377 | 25663541 |

<br/>

---

<br/>


**Cache Configuration**: Lasatly, to figure out the best cache configuration, we conducted a series of tests with different cache set configurations, while maintaining the overall cache size. They all ran with 64 byte blocks, write-allocate, write-back, and lru eviction policy. As the data shows, as the number of sets go down and number of blocks go up, the hit rate and the total cycles decreases, signaling higher perforamnce. We can think of this as a result of higher associativity, which correlates to a higher hit rate, and hence better performance. However, we know that higher associativity doesn't come without drawbacks. Higher associativity also leads to a higher delay during cycles. Therefore the best place to be is with the highst associativity as possible, but before the point of significant diminishing returns due to hardware delay. 


|  | 1 set / 4096 blocks | 8 sets / 512 blocks | 64 sets / 64 blocks | 512 sets / 8 blocks | 4096 sets / 1 block |
|-|-|-|-|-|-|
| total loads | 318197 | 318197 | 318197 | 318197 | 318197 |
| total stores | 197486 | 197486 | 197486 | 197486 | 197486 |
| load hits | 317204 | 317204 | 317199 | 317197 | 316465 |
| load misses | 993 | 993 | 998 | 1000 | 1732 |
| store hits | 195112 | 195112 | 195112 | 195105 | 195043 |
| store misses | 2374 | 2374 | 2374 | 2381 | 2443 |
| total cycles | 5902883 | 5902883 | 5944497 | 6107753 | 8129565 |

<br/>

**BEST CONFIGURATION**: With the research above, we conclude that the best cache configuration is a combination of write-allocate, write-back, lru, with a good balance of associativity (8 sets / 512 blocks).

## Contributions

Matthew's Contributions
* command line argument validation
* address decomposing
* load, loadMiss, loadHit execution for lru
* parts of store, storeMiss, storeHit execution for lru
* CacheBlock class implementation

Andrew's Contributions
* basic program control flow
* reading trace files
* Cache class structure
* parts of store, storeMiss, storeHit execution for lru
* cycle count
* load and store hit and miss excecution for fifo
* printing results
