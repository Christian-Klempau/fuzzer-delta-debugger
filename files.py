import tempfile
import string
import random
from time import perf_counter, time


str_size = 1_00
str_count = 1_0000
n_times = 1_000

strings = [
    "".join([random.choice(string.ascii_letters) for i in range(str_size)]) + "\n"
    for j in range(str_count)
]


def test(file_ptr, strings, n_times, timer_func):
    t0 = timer_func()

    for _ in range(n_times):
        file_ptr.seek(0)
        file_ptr.truncate()
        file_ptr.writelines(strings)
        file_ptr.flush()

    t1 = timer_func()

    return t1 - t0


tmp_file = tempfile.NamedTemporaryFile("w")
real_file = open("real_file.txt", "w")

t1 = test(tmp_file, strings, n_times, perf_counter)
t2 = test(real_file, strings, n_times, perf_counter)

tmp_file.close()
real_file.close()

print("Temp file write time: ", t1)
print("Real file write time: ", t2)
