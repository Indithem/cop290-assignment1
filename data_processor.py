import pandas
import datetime
from datetime import date
from dateutil.relativedelta import relativedelta
from jugaad_data.nse import stock_df
import sys
import timeit
import os
import matplotlib.pyplot as plt
import numpy as np


class processor:
    """
    Downloads data as required for the assignment.

    BENCHMARK_REPEATS: Number of times to repeat the benchmarking
    REQUIRED_COLUMNS: Columns required for the assignment, ordered

    All the write_to_file_format functions should be named as `write_to_<format>`
    If the same format has multiple testing methods, then the function name should be `write_to_<format>_<method>`
    The file should be saved as `<SYMBOL>_<method>.<format>`
    This enables automated benchmarking of those functions.
    """

    BENCHMARK_REPEATS = 100
    REQUIRED_COLUMNS = [
        "DATE",
        "OPEN",
        "CLOSE",
        "HIGH",
        "LOW",
        "LTP",
        "VOLUME",
        "VALUE",
        "NO OF TRADES",
    ]

    def __init__(self, SYMBOL: str, YEARS: int) -> None:
        self.SYMBOL = SYMBOL
        self.YEARS = YEARS
        self.download_data()

    def download_data(self) -> pandas.DataFrame:
        try:
            df: pandas.DataFrame = stock_df(
                symbol=self.SYMBOL,
                from_date=date.today() - relativedelta(years=self.YEARS),
                to_date=date.today(),
            )
            df = df[self.REQUIRED_COLUMNS]
            self.data: pandas.DataFrame = df
            return df
        except Exception as e:
            self.data = pandas.DataFrame()
            print(f"Error during downloading {self.SYMBOL} data\n{e}")

    def benchmark(self) -> list[(str, float, float)]:
        benchmark_results = list[(str, float, float)]()
        for f in dir(self):
            if "write_to" in f and callable(getattr(self, f)):
                splitted = f.removeprefix("write_to_").split("_")
                fileformat = splitted[0]
                test_name = ("_" + splitted[1]) if len(splitted) > 1 else ""
                time = (
                    timeit.Timer(lambda: getattr(self, f)()).timeit(
                        number=self.BENCHMARK_REPEATS
                    )
                    / self.BENCHMARK_REPEATS
                )
                size = os.stat(f"{self.SYMBOL}{test_name}.{fileformat}").st_size
                benchmark_results.append((f"{fileformat}{test_name}", time, size))
        return benchmark_results

    def write_to_csv(self) -> None:
        self.data.to_csv(f"{self.SYMBOL}.csv", index=False)

    # def write_to_txt_asStr(self) -> None:
    #     with open(f"{self.SYMBOL}_asStr.txt", "w") as f:
    #         f.write(self.data.to_string(index=False))

    def write_to_txt(self) -> None:
        self.data.to_csv(f"{self.SYMBOL}.txt", index=False, sep="\t")

    # def write_to_json_records(self) -> None:
    #     self.data.to_json(f"{self.SYMBOL}_records.json", orient="records")

    # def write_to_json_columns(self) -> None:
    #     self.data.to_json(f"{self.SYMBOL}_columns.json", orient="columns")

    # def write_to_json_split(self) -> None:
    #     self.data.to_json(f"{self.SYMBOL}_split.json", orient="split")

    # def write_to_json_index(self) -> None:
    #     self.data.to_json(f"{self.SYMBOL}_index.json", orient="index")

    def write_to_json(self) -> None:
        # json_split is found to be the best
        self.data.to_json(f"{self.SYMBOL}.json", orient="values")

    # def write_to_json_table(self) -> None:
    #     self.data.to_json(f"{self.SYMBOL}_table.json", orient="table")

    def write_to_xlsx(self) -> None:
        self.data.to_excel(f"{self.SYMBOL}.xlsx", index=False)

    def write_to_html(self) -> None:
        self.data.to_html(f"{self.SYMBOL}.html", index=False)

    def write_to_tex(self) -> None:
        self.data.to_latex(f"{self.SYMBOL}.tex", index=False)

    def write_to_xml(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_xml(
            f"{self.SYMBOL}.xml", index=False
        )

    def write_to_feather(self) -> None:
        self.data.to_feather(f"{self.SYMBOL}.feather")

    def write_to_parquet(self) -> None:
        self.data.to_parquet(f"{self.SYMBOL}.parquet")

    def write_to_orc(self) -> None:
        self.data.to_orc(f"{self.SYMBOL}.orc")

    def write_to_dta(self) -> None:
        self.data.rename(lambda x: x.replace(" ", "_"), axis=1).to_stata(
            f"{self.SYMBOL}.dta", write_index=False
        )

    def write_to_hdf(self) -> None:
        self.data.to_hdf(f"{self.SYMBOL}.hdf", key="data", mode="w")

    def write_to_pkl(self) -> None:
        self.data.to_pickle(f"{self.SYMBOL}.pkl")


def make_graph(benchmark_results: list[(str, float, float)]) -> plt.Figure:
    """pretty print the benchmark results and show plot
    The benchmark results should be a list of tuples of the form (format, time, size)"""
    print(f"{'FORMAT':<20}{'TIME (s)':<20}{'SIZE (bytes)':<30}")
    for result in benchmark_results:
        print(f"{result[0]:<20}{result[1]:<20}{result[2]:<30}")

    # display two bar graphs for time and size alongside, with two y-axes for each unit(time and size)
    fig, ax1 = plt.subplots(figsize=(10, 5))
    SEPERATION = 0.45
    ax1.set_xlabel("FORMAT")
    ax1.set_ylabel("TIME (s)")
    ax1.bar(
        np.arange(len(benchmark_results)),
        # [result[0] for result in benchmark_results],
        [result[1] for result in benchmark_results],
        width=SEPERATION,
        color="blue",
    )
    ax1.tick_params(axis="y")
    ax1.set_yscale("log")
    ax2 = ax1.twinx()
    ax2.set_ylabel("SIZE (bytes)")
    ax2.bar(
        SEPERATION + np.arange(len(benchmark_results)),
        [result[2] for result in benchmark_results],
        color="red",
        width=SEPERATION,
    )
    ax1.legend(["TIME (s)"], loc="upper left")
    ax2.legend(["SIZE (bytes)"], loc="upper right")
    ax2.tick_params(axis="y")
    ax1.set_xticks(np.arange(len(benchmark_results)) + 0.2)
    ax1.set_xticklabels([result[0] for result in benchmark_results])
    fig.tight_layout()
    return fig


def main():
    """
    Arguments should be given as Symbol, Year
    """
    if len(sys.argv) > 3:
        processor.BENCHMARK_REPEATS = int(sys.argv[3])
    p = processor(sys.argv[1], int(sys.argv[2]))
    graph = make_graph(p.benchmark())
    graph.savefig(f"{p.SYMBOL}.png", dpi=400)


if __name__ == "__main__":
    main()
