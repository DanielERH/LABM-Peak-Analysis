# LABM Automated Peak Analysis Pipeline

Automated data processing pipeline for the **Large Angle Beamstrahlung Monitor (LABM)** — a beam diagnostic detector developed for the [Belle II experiment](https://www.belle2.org/) at the SuperKEKB accelerator in Tsukuba, Japan.

This script was written to replace manual, run-by-run analysis. It processes raw beam data across four optical channels, normalizes rates against accelerator current, tracks peak positions over time, and exports a full set of diagnostic PDF reports — all in a single command.

---

## What it does

The LABM detector scans a set of predefined motor positions (called "peaks") and records photon rates from multiple photomultiplier tubes (PMTs) at each position. Over a run, it cycles through these positions continuously, producing a large stream of structured data files.

This pipeline:

1. **Reads and validates** the configuration file — catches duplicate peak definitions before they corrupt results and exits with a clear message if any are found
2. **Ingests all data files** for a run automatically, looping through sequential files until none remain
3. **Filters non-physics data** — entries where the accelerator isn't in "Physics" mode or beam currents fall below threshold are excluded from averages
4. **Normalizes PMT rates** against the HER/LER beam current at each measurement point
5. **Tracks peak performance over time** — for each optical channel and scan area, it finds the peak with the highest rate per cycle
6. **Computes weighted beam statistics** — beam position averages (x, y), second moments, and cross-terms per PMT per cycle
7. **Exports PDF reports** — one set per optical channel per scan area, covering rate maps, motor position averages, and beam quality diagnostics over time

The four optical channels processed are:
- **Oho Up** (Motors 1 & 2)
- **Oho Down** (Motors 3 & 4)
- **Nikko Up** (Motors 9 & 10)
- **Nikko Down** (Motors 11 & 12)

---

## Requirements

- [ROOT](https://root.cern/) 6.14/04 or later (developed and tested on 6.14/04)
- A C++ compiler compatible with ROOT's `cling` interpreter
- Linux or macOS terminal environment

---

## How to run

```bash
root -l -b -q LABMAuto.C
```

- `-l` skips the ROOT splash screen
- `-b` runs in batch mode (no graphical windows)
- `-q` closes ROOT when the script finishes

---

## Required folder structure

Before running, your working directory should look like this:

```
HEADFOLDER/
├── LABMAuto.C
├── labmpeaks.conf
├── DATA/
│   ├── labmpeaks-run00000100_0000.dat
│   ├── labmpeaks-run00000100_0001.dat
│   └── ...
└── PDF/              ← output folder, must exist before running
```

The script reads data files sequentially starting from `_0000` and stops automatically when no further file is found.

---

## Configuration file

`labmpeaks.conf` defines the detector state and the motor positions to scan. It must be in the same directory as the script.

> **Important:** Do not repeat peak positions in the configuration file. If any two peaks share the same motor coordinates, the script will detect the conflict, print which peaks to fix, and exit before processing any data. This prevents silent errors in the output.

A template configuration file (`labmpeaks_template.conf`) is included in this repository to show the expected format.

---

## Output

All output is saved to the `PDF/` folder. Reports are named by run number and scan area, for example:

```
PDF/PeakRateOUMaxRun100Sec0.pdf
PDF/AverageMotor1Motor2PMT1Run100Sec0.pdf
```

Each PDF contains multi-page diagnostic plots including:
- PMT rate maps across motor position space
- Peak rate stability over scan cycles
- Beam position averages (Motor 1 & 2 weighted means)
- Second moment and cross-term evolution over time

---

## Context

This code was developed as part of the LABM detector operations at KEK during Belle II data-taking. The detector monitors beamstrahlung — synchrotron radiation produced when the electron and positron beams interact — and is used to characterize beam conditions and backgrounds in the interaction region.

The pipeline was designed to handle long runs with thousands of scan cycles and multiple simultaneous data files, with an emphasis on making the output reproducible and easy to review without manual intervention.

---

## Skills demonstrated

`C++` · `ROOT/CERN` · `Data pipeline design` · `Statistical data processing` · `Signal normalization` · `Automated file I/O` · `Multi-channel data handling` · `Scientific visualization` · `Experimental physics instrumentation`

---

## Author

**Daniel Enrique Ricalde Herrmann**  
PhD in Physics — Wayne State University (2024)  
daniel.ricalde.h@gmail.com
