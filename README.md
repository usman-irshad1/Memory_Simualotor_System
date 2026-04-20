<![CDATA[<h1 align="center">🧠 Virtual Memory Management Simulator</h1>

<p align="center">
  <b>A full-stack simulation of the OS virtual memory subsystem — TLB, Page Table, RAM, and Disk — with FIFO, LRU, and OPT page-replacement algorithms.</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-blue?logo=cplusplus" />
  <img src="https://img.shields.io/badge/Build-MSVC%20%2F%20Visual%20Studio-purple?logo=visualstudio" />
  <img src="https://img.shields.io/badge/Platform-Windows-0078D6?logo=windows" />
  <img src="https://img.shields.io/badge/License-MIT-green" />
</p>

---

##  Table of Contents

- [Overview](#-overview)
- [Architecture](#-architecture)
- [Page Replacement Algorithms](#-page-replacement-algorithms)
- [Project Structure](#-project-structure)
- [Getting Started](#-getting-started)
  - [Prerequisites](#prerequisites)
  - [Build & Run](#build--run)
- [Configuration](#-configuration)
- [Input Format](#-input-format)
- [Sample Output](#-sample-output)
- [Performance Metrics](#-performance-metrics)
- [How It Works](#-how-it-works)
- [Dependencies](#-dependencies)
- [Contributing](#-contributing)
- [License](#-license)

---

##  Overview

This project simulates the **complete virtual-to-physical memory translation pipeline** found in modern operating systems. It models:

| Component | What it simulates |
|---|---|
| **TLB (Translation Lookaside Buffer)** | A small, fast cache for recent VPN→Frame translations |
| **Page Table** | The full VPN→Physical Frame mapping with dirty/valid bits |
| **RAM** | A byte-addressable physical memory array |
| **Hard Disk** | Backing store for dirty page write-backs |

The simulator reads a stream of hexadecimal virtual addresses, translates each through the TLB → Page Table → RAM hierarchy, applies a chosen replacement policy when memory is full, and reports detailed performance statistics at the end.

---

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     Virtual Address                     │
│              ┌──────────────┬──────────┐                │
│              │     VPN      │  Offset  │                │
│              └──────┬───────┴─────┬────┘                │
│                     │             │                      │
│              ┌──────▼──────┐      │                      │
│              │     TLB     │      │                      │
│              │  (4 entries)│      │                      │
│              └──┬─────┬───┘      │                      │
│            Hit  │     │ Miss     │                      │
│              ┌──▼─────▼───┐      │                      │
│              │ Page Table  │      │                      │
│              │ (FIFO/LRU/ │      │                      │
│              │    OPT)     │      │                      │
│              └──────┬──────┘      │                      │
│                     │ Frame #     │                      │
│              ┌──────▼─────────────▼───┐                  │
│              │        RAM             │                  │
│              │  memory[frame][offset] │                  │
│              └──────────┬─────────────┘                  │
│                         │ Dirty eviction                 │
│              ┌──────────▼─────────────┐                  │
│              │     HardDisk.txt       │                  │
│              │    (Backing Store)     │                  │
│              └────────────────────────┘                  │
└─────────────────────────────────────────────────────────┘
```

---

##  Page Replacement Algorithms

The simulator implements **three** classic page-replacement strategies, each executed independently on the same address trace for fair comparison:

### 1. FIFO (First-In, First-Out)
> Evicts the **oldest** page in memory — the one that was loaded first.

- Simple queue-based implementation (`std::list`)
- Replacement triggered when `frame_count >= max_frames`
- Logs every eviction to `FIFO_logging.txt`

### 2. LRU (Least Recently Used)
> Evicts the page that hasn't been accessed for the **longest time**.

- O(1) lookup and update via `unordered_map` + `std::list` (doubly-linked list as a recency queue)
- On each access, the page is moved to the front of the LRU queue
- Logs evictions to `log.txt`

### 3. OPT (Optimal / Bélády's Algorithm)
> Evicts the page that will **not be used for the longest time** in the future.

- Pre-computes a **lookahead buffer** of all future VPN references
- On eviction, scans forward from the current index to find the page with the farthest (or no) future use
- Theoretical lower bound — used as a benchmark for FIFO and LRU
- Logs evictions to `OPT_logging`

---

##  Project Structure

```
OS_project/
├── OS_project.cpp              # Entry point — runs all three algorithms sequentially
├── Virtual_Memory_manager.h    # Manager interface (Walker, Runner, SPRINTER)
├── Virtual_Memory_manager.cpp  # Orchestration logic for FIFO, LRU, OPT simulations
│
├── TLB_table.h                 # TLB class with FIFO & LRU insertion/lookup
├── TLB_table.cpp               # TLB implementation + performance metrics output
│
├── Page_frame.h                # Page_table class with all three replacement policies
├── Page_frame.cpp              # Page table logic, eviction handling, disk write-back
│
├── Ram_memory.h                # Ram_memory array + Page_frame_node struct
├── Ram_memory.cpp              # Physical memory read/write operations
│
├── FileConfiguration.h         # Configuration loader (JSON) + address parser
├── FileConfiguration.cpp       # Parses Parameters.json & addresses.txt
│
├── Parameters.json             # Simulation parameters (sizes, latency)
├── addresses.txt               # Input: hex virtual addresses (one per line)
│
├── Doxyfile                    # Doxygen config for auto-generated documentation
├── OS_project.slnx             # Visual Studio solution
├── OS_project.vcxproj          # Visual Studio project file
└── packages.config             # NuGet dependency manifest (jsoncpp)
```

### Output Files (generated at runtime)

| File | Description |
|---|---|
| `log.txt` | LRU eviction trace |
| `FIFO_logging.txt` | FIFO eviction trace |
| `OPT_logging` | OPT eviction trace |
| `HardDisk.txt` | Simulated disk — receives dirty page write-backs |

---

##  Configuration

All simulation parameters are defined in **`Parameters.json`**:

```json
{
    "physical_size": 40960,
    "page_size": 4096,
    "TLB_size": 4,
    "virtual_size": 18446744073709551615,
    "latency": 10
}
```

| Parameter | Description | Default |
|---|---|---|
| `physical_size` | Total physical memory in bytes | `40960` (40 KB → 10 frames) |
| `page_size` | Size of each page/frame in bytes | `4096` (4 KB) |
| `TLB_size` | Number of TLB entries | `4` |
| `virtual_size` | Upper bound of virtual address space | `2^64 - 1` (full 64-bit) |
| `latency` | RAM access latency in nanoseconds | `10` ns |

> **Tip:** Tweak `physical_size` and `TLB_size` to observe how hit rates and EAT change under memory pressure.

---

## Input Format

The simulator reads virtual addresses from **`addresses.txt`**. Each line should contain a single hexadecimal address:

```
0x00003a
0x000041
0x000058
0x0000FF
...
```

Every 5th address is treated as a **write** operation (`W`); all others are **reads** (`R`).

---


## Performance Metrics

At the end of each algorithm run, the simulator computes:

| Metric | Formula / Description |
|---|---|
| **TLB Hit Rate** | `TLB_hits / total_requests × 100%` |
| **RAM Reads** | Total page-ins from disk to RAM |
| **RAM Writes** | Total in-RAM write operations |
| **Dirty Evictions** | Pages written back to disk on eviction |
| **Effective Access Time (EAT)** | Weighted sum considering TLB (1 ns), page table walk (10 ns), RAM access (latency), writes (2× latency), and disk write-back (1000 ns) |

### EAT Breakdown

```
EAT = (requests × 1ns)                          // TLB lookup
    + (TLB_misses × 10ns)                        // Page table walk penalty
    + (RAM_reads × latency)                      // Reading from RAM
    + (RAM_writes × latency × 2)                 // Writing to RAM (2x penalty)
    + (dirty_evictions × 1000ns)                 // Disk write-back
    ──────────────────────────────────
                total_requests
```

---

## How It Works

1. **Initialization** — `FileConfiguration` parses `Parameters.json` for memory sizes and latency, then loads all hex addresses from `addresses.txt`.

2. **Address Decomposition** — Each virtual address is split into a **Virtual Page Number (VPN)** and **Offset** using bitwise operations based on the page size.

3. **TLB Lookup** — The VPN is searched in the TLB cache.
   - **Hit →** The physical frame is retrieved directly; the page is promoted in the LRU queue (for LRU/OPT modes).
   - **Miss →** Fall through to the Page Table.

4. **Page Table Lookup** — The VPN is searched in the page table.
   - **Found →** The frame is returned and the TLB is updated.
   - **Not Found (Page Fault) →** A free frame is allocated, or eviction occurs using the active policy (FIFO / LRU / OPT).

5. **Eviction** — When physical memory is full:
   - The victim page is selected according to the active algorithm.
   - If the victim's **dirty bit** is set, its frame contents are flushed to `HardDisk.txt`.
   - The victim is removed from the Page Table and TLB.
   - The new page is loaded into the freed frame.

6. **Shutdown Sync** — On destruction, `Page_table` flushes all remaining dirty pages to `HardDisk.txt`, simulating a clean system shutdown.

---


