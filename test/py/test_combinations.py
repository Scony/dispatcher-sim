import pytest

from fixtures import *
from helpers import execute_process


@pytest.mark.parametrize('algorithm,representation', [
    ('random', 'queue'),
    ('max', 'queue'),
    ('min', 'queue'),
    ('fifo', 'queue'),
    ('sjlo', 'queue'),
    ('sjso', 'queue'),
    ('ljso', 'queue'),
    ('lo', 'queue'),
    ('elo', 'queue'),
    ('so', 'queue'),
    ('qopt', 'queue'),
    ('qworst', 'queue'),
    ('jopt', 'queue'),
    ('jsa', 'queue'),
    ('sasa', 'queue'),
    ('sjsa', 'queue'),
    ('sa', 'queue'),
    ('rvr', 'queue'),
    ('dvr', 'queue'),
    ('random', 'schedule'),
    ('sa', 'schedule'),
    ('sjlo', 'schedule'),
])
def test_algorithm_representation_v1_combinations(simulator_path, algorithm, representation):
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
    retcode, _, _ = execute_process(simulator_path,
                                    [
                                        algorithm,
                                        '-m1',
                                        '-eno',
                                        '-r%s' % representation,
                                        '-v1',
                                    ],
                                    input)
    assert retcode == 0


@pytest.mark.parametrize('algorithm,representation', [
    ('random', 'queue'),
    ('max', 'queue'),
    ('min', 'queue'),
    ('fifo', 'queue'),
    ('sjlo', 'queue'),
    ('sjso', 'queue'),
    ('ljso', 'queue'),
    ('lo', 'queue'),
    ('elo', 'queue'),
    ('so', 'queue'),
    ('qopt', 'queue'),
    ('qworst', 'queue'),
    ('jopt', 'queue'),
    ('jsa', 'queue'),
    ('sasa', 'queue'),
    ('sjsa', 'queue'),
    ('sa', 'queue'),
    # ('rvr', 'queue'),           # crashing
    # ('dvr', 'queue'),           # crashing
    # ('random', 'schedule'),     # TODO: implement & enable
])
def test_algorithm_representation_v2_combinations(simulator_path, algorithm, representation):
    input = b"""
    0
    2
    1
    1 1 285
    5
    1 3 1022 0 9 1
    1 4 740 0 13 1
    1 5 3237 0 65 1
    1 6 1692 0 8 1
    1 7 3352 0 10 4
    1
    2
    0 1
    1 4
    """
    retcode, _, _ = execute_process(simulator_path,
                                    [
                                        algorithm,
                                        '-eno',
                                        '-r%s' % representation,
                                        '-v2',
                                    ],
                                    input)
    assert retcode == 0
