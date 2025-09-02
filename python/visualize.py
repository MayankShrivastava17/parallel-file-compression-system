#!/usr/bin/env python3
import argparse, csv
import matplotlib.pyplot as plt

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", required=True)
    args = ap.parse_args()

    files, ratios, secs = [], [], []
    with open(args.csv) as f:
        reader = csv.DictReader(f)
        for row in reader:
            files.append(row["input_path"])
            inb = max(1, int(row["in_bytes"]))
            outb = int(row["out_bytes"])
            ratios.append(outb / inb)
            secs.append(float(row["seconds"]))

    # Chart 1: compression ratio
    plt.figure()
    plt.bar(range(len(files)), ratios)
    plt.xticks(range(len(files)), [f.split('/')[-1] for f in files], rotation=45, ha='right')
    plt.ylabel("Compression ratio (out/in, lower is better)")
    plt.title("Compression Ratio per File")
    plt.tight_layout()
    plt.show()

    # Chart 2: runtime
    plt.figure()
    plt.bar(range(len(files)), secs)
    plt.xticks(range(len(files)), [f.split('/')[-1] for f in files], rotation=45, ha='right')
    plt.ylabel("Seconds")
    plt.title("Compression Time per File")
    plt.tight_layout()
    plt.show()

if __name__ == "__main__":
    main()
