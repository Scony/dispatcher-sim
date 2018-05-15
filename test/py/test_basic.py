import os
import subprocess

from fixtures import *
from helpers import execute_process, sorted_opfins


def test_binaries_in_place(simulator_path):
    assert os.path.isfile(simulator_path)


def test_qopt(simulator_path):
    input = b"""
    5

    0 1 1
    2
    0 1 0 1
    1 1 0 99

    1 1 2
    1
    2 1 0 1

    2 1 3
    1
    3 1 0 1

    3 1 4
    1
    4 1 0 1

    4 1 5
    1
    5 1 0 1
    """
    expected_output = """104
1
1
1
1
"""
    retcode, stdout, _ = execute_process(simulator_path,
                                         [
                                             'qopt',
                                             '-m1',
                                             '-eno',
                                         ],
                                         input)
    assert retcode == 0
    assert stdout.startswith(expected_output)


def test_fifo(simulator_path):
    input = b"""
    5

    0 1 1
    2
    0 1 0 1
    1 1 0 99

    1 1 2
    1
    2 1 0 1

    2 1 3
    1
    3 1 0 1

    3 1 4
    1
    4 1 0 1

    4 1 5
    1
    5 1 0 1
    """
    expected_output = """100
100
100
100
100
"""
    retcode, stdout, _ = execute_process(simulator_path,
                                         [
                                             'fifo',
                                             '-m1',
                                             '-eno',
                                         ],
                                         input)
    assert retcode == 0
    assert stdout.startswith(expected_output)


def test_no_estimation(simulator_path):
    input = b"""
    3

    0 0 0
    3
    0 0 0 1000
    1 0 0 2000
    2 0 0 3000

    1 0 6000
    1
    3 1 0 10

    2 0 6001
    2
    4 0 0 1
    5 3 0 2
    """
    expected_opfins = [
        (3000, 2, 0),
        (5000, 1, 0),
        (6000, 0, 0),
        (6010, 3, 0),
        (6012, 5, 0),
        (6013, 4, 0),
    ]
    retcode, stdout, _ = execute_process(simulator_path,
                                         [
                                             'max',
                                             '-m1',
                                             '-eno',
                                             '-oopfins',
                                         ],
                                         input)
    assert retcode == 0
    assert expected_opfins == sorted_opfins(stdout)



def test_estimation(simulator_path):
    input = b"""
    3

    0 0 0
    3
    0 0 0 1000
    1 0 0 2000
    2 0 0 3000

    1 0 6000
    1
    3 1 0 10

    2 0 6001
    2
    4 0 0 1
    5 3 0 2
    """
    expected_opfins = [
        (3000, 2, 0),
        (5000, 1, 0),
        (6000, 0, 0),
        (6010, 3, 0),
        (6011, 4, 0),
        (6013, 5, 0),
    ]
    retcode, stdout, _ = execute_process(simulator_path,
                                         [
                                             'max',
                                             '-m1',
                                             '-ekrec',
                                             '-k3',
                                             '-oopfins',
                                         ],
                                         input)
    assert retcode == 0
    assert expected_opfins == sorted_opfins(stdout)
