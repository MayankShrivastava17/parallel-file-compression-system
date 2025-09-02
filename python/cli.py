#!/usr/bin/env python3
import argparse, csv, subprocess, sys, time, os
from pathlib import Path

def run(cmd):
    p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    out, err = p.communicate()
    if p.returncode != 0:
        print(err, file=sys.stderr)
        sys.exit(p.returncode)
    return out

def main():
    ap = argparse.ArgumentParser(description="Python CLI wrapper for parallel_compressor")
    sub = ap.add_subparsers(dest="mode", required=True)

    ap_c = sub.add_parser("compress")
    ap_c.add_argument("--algo", choices=["huffman","lz77"], required=True)
    ap_c.add_argument("--out-dir", default="../out")
    ap_c.add_argument("files", nargs='+')

    ap_d = sub.add_parser("decompress")
    ap_d.add_argument("--out-dir", default="../out")
    ap_d.add_argument("files", nargs='+')

    ap_v = sub.add_parser("visualize")
    ap_v.add_argument("--csv", required=True, help="CSV exported from compressor stdout")

    args = ap.parse_args()
    exe = str((Path(__file__).resolve().parents[1] / "build" / "parallel_compressor"))

    if args.mode in ("compress","decompress"):
        cmd = [exe, args.mode]
        if args.mode == "compress":
            cmd += [args.algo, args.out_dir] + args.files
        else:
            cmd += [args.out_dir] + args.files
        out = run(cmd)
        # Save CSV for visualization
        ts = int(time.time())
        csv_path = Path(args.out_dir) / f"metrics_{ts}.csv"
        os.makedirs(args.out_dir, exist_ok=True)
        with open(csv_path, "w") as f:
            f.write(out)
        print(f"[OK] metrics saved to {csv_path}")
        print(out)
    elif args.mode == "visualize":
        print("Use visualize.py to plot charts from the CSV.")
    else:
        ap.print_help()

if __name__ == "__main__":
    main()
