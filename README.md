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