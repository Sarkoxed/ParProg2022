import seaborn as sns
import pandas as pd
from sys import argv

ts = [(1, 0.0304199), (2, 0.0162707), (3, 0.0143061), (4, 0.0129716), (5, 0.0104899), (6, 0.009008), (7, 0.00844967), (8, 0.0119082), (9, 0.00981355), (10, 0.0117491), (11, 0.0118373), (12, 0.0117609), (13, 0.0093371), (14, 0.00872779), (15, 0.00959356), (16, 0.00983097), ]

threadbound = 16
cores = 8
ts = ts[:threadbound]
t_succ = ts[0][1]

ts = [[x[0], x[1] * 1000] for x in ts]
ts_ds = [[x[0], x[1], "Real"] for x in ts]
exp_ts = [[i, t_succ * 1000 / i, "Expected"] for i in range(1, cores + 1)] + [
    [i, t_succ * 1000 / cores, "Expected"] for i in range(cores + 1, threadbound + 1)
]

columns = ["Threads", "AvgTime", "Type"]
got_ts_df = pd.DataFrame(ts_ds, columns=columns)
exp_ts_df = pd.DataFrame(exp_ts, columns=columns)
all_ts = pd.concat([got_ts_df, exp_ts_df]).astype({"Threads": "int64", "AvgTime": "float64"})

print(all_ts)
p = sns.lineplot(x="Threads", y="AvgTime", hue="Type", marker=".", data=all_ts)

