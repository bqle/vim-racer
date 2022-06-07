
import pathlib

p = pathlib.Path(__file__).parent.parent.joinpath('README.md')

with open(p, 'rb') as f:   
    file_list = [line.rstrip() for line in f.readlines()]

errors = []
