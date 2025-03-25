rows = []
for r in range(50):
    row = []
    if r % 2 == 0:  # четная строка: слева направо
        for c in range(50):
            val = (r * 50 + c) % 286 + 1
            row.append(str(val))
    else:  # нечетная строка: справа налево
        for c in range(50):
            val = (r * 50 + (49 - c)) % 286 + 1
            row.append(str(val))
    rows.append(",".join(row))
csv_data = "\n".join(rows)
print(csv_data)
