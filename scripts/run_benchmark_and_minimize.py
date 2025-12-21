import subprocess
import sys
from pathlib import Path

def main():
    if len(sys.argv) != 6:
        print(
            "Usage: run_benchmark_and_minimize.py "
            "<benchmark_exe> <json_output> <dates.json path> <date(yyyy-mm-dd)> <minimize_exe>"
        )
        sys.exit(1)

    benchmark_exe = Path(sys.argv[1])
    json_out      = Path(sys.argv[2])
    dates_json    = Path(sys.argv[3])
    date          = str(sys.argv[4])
    minimize_exe  = Path(sys.argv[5])

    json_out.parent.mkdir(parents=True, exist_ok=True)

    # run benchmark
    subprocess.check_call([
        str(benchmark_exe),
        "--benchmark_format=json",
        f"--benchmark_out={json_out}",
    ])

    # minimize json
    subprocess.check_call([
        str(minimize_exe),
        str(json_out),
        str(dates_json),
        str(date)
    ])

if __name__ == "__main__":
    main()
