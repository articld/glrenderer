import csv
import subprocess
import sys
import os
from datetime import datetime

# --- Configuration ---
INPUT_CSV = "final_3dmodels.csv"
RENDERER_EXE = "./cmake-build-release/glrenderer.exe"
RENDERED_CSV = "rendered_models.csv"
FAILED_CSV = "failed_renders.csv"

FIELDNAMES = [
    "3dmodel_id", "path", "meshes", "materials", "textures", "images",
    "image_height_max", "image_height_min", "image_width_max", "image_width_min",
    "vertices", "faces", "extent_x", "extent_y", "extent_z"
]


def init_output_csv(filepath: str) -> None:
    """Create the output CSV with headers if it doesn't already exist."""
    if not os.path.exists(filepath):
        with open(filepath, "w", newline="") as f:
            writer = csv.DictWriter(f, fieldnames=FIELDNAMES + ["timestamp"])
            writer.writeheader()


def append_row(filepath: str, row: dict, timestamp: str) -> None:
    """Append a single row to an output CSV."""
    with open(filepath, "a", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=FIELDNAMES + ["timestamp"])
        row_out = {k: row.get(k, "") for k in FIELDNAMES}
        row_out["timestamp"] = timestamp
        writer.writerow(row_out)


def run_renderer(model_path: str, extent_x: str, extent_y: str, extent_z: str) -> int:
    """
    Run glrenderer.exe with the given arguments.
    Returns the process exit code.
    """
    cmd = [RENDERER_EXE, model_path, extent_x, extent_y, extent_z]
    print(f"  Running: {' '.join(cmd)}")
    try:
        result = subprocess.run(cmd, check=False)
        return result.returncode
    except FileNotFoundError:
        print(f"  ERROR: '{RENDERER_EXE}' not found. Make sure it is in your PATH or the same directory.")
        sys.exit(1)


def process_csv(input_csv: str) -> None:
    if not os.path.exists(input_csv):
        print(f"ERROR: Input file '{input_csv}' not found.")
        sys.exit(1)

    init_output_csv(RENDERED_CSV)
    init_output_csv(FAILED_CSV)

    rendered_count = 0
    failed_count = 0

    with open(input_csv, newline="") as f:
        reader = csv.DictReader(f)

        # Validate required columns
        required = {"3dmodel_id", "path", "extent_x", "extent_y", "extent_z"}
        missing = required - set(reader.fieldnames or [])
        if missing:
            print(f"ERROR: Input CSV is missing required columns: {missing}")
            sys.exit(1)

        for row in reader:
            model_id = row["3dmodel_id"].strip()
            model_path = "G:/3dmodels/original/" + row["path"].strip()
            extent_x = row["extent_x"].strip()
            extent_y = row["extent_y"].strip()
            extent_z = row["extent_z"].strip()

            print(f"\nProcessing model: {model_id}")

            exit_code = run_renderer(model_path, extent_x, extent_y, extent_z)
            timestamp = datetime.now().isoformat(timespec="seconds")

            if exit_code == 0:
                print(f"  ✓ SUCCESS (exit code 0) → {RENDERED_CSV}")
                append_row(RENDERED_CSV, row, timestamp)
                rendered_count += 1
            else:
                print(f"  ✗ FAILED (exit code {exit_code}) → {FAILED_CSV}")
                append_row(FAILED_CSV, row, timestamp)
                failed_count += 1

    print(f"\n{'='*50}")
    print(f"Done. Processed {rendered_count + failed_count} model(s).")
    print(f"  Rendered successfully : {rendered_count}")
    print(f"  Failed                : {failed_count}")
    print(f"  Results written to    : '{RENDERED_CSV}' and '{FAILED_CSV}'")


if __name__ == "__main__":
    # Optionally accept a custom input CSV path as a command-line argument
    if len(sys.argv) > 1:
        INPUT_CSV = sys.argv[1]

    print(f"Batch renderer starting — input: '{INPUT_CSV}'")
    process_csv(INPUT_CSV)