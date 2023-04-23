import time

def get_cpu_usage():
    with open('/proc/stat', 'r') as file:
        lines = file.readlines()

    cpu_stats = []
    for line in lines:
        if line.startswith('cpu'):
            cpu_stats.append(list(map(int, line.split()[1:])))

    usage = []
    for i in range(len(cpu_stats)):
        total = sum(cpu_stats[i])
        idle = cpu_stats[i][3]
        usage.append(100.0 * (total - idle) / total)

    return usage

while True:
    cpu_usage = get_cpu_usage()
    for i in range(len(cpu_usage)):
        print(f"CPU{i} usage: {cpu_usage[i]:.2f}%")
        print("\n")
    time.sleep(1)

