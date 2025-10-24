# Evara Clouds Embedded Projects

A collection of **bare-metal embedded C examples** demonstrating:
- Real-time cooperative scheduling
- Deterministic memory management (no heap)
- Static pool allocators
- UART-based logging (no file I/O)

## Structure
- `firmware/` – MCU-ready code (no stdlib dependencies)
- `examples/` – PC-hosted simulations of embedded concepts
- `tools/` – Host-side scripts and analyzers
- `tests/` – Unit tests for core modules
- `docs/` – Architecture, memory maps, and design notes

## Build
To build the firmware simulation on host:
```bash
make

# OS junk
.DS_Store
Thumbs.db
