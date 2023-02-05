import tempfile

file = tempfile.NamedTemporaryFile("w", suffix=".wcnf")

print(file.name)
file.close()
