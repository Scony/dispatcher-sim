import os
import subprocess

# TODO: fixture


def test_binaries_in_place():
    my_dir = os.path.dirname(__file__)
    bin_dir = os.path.abspath('%s/../../bin' % my_dir)
    simulator_path = '%s/simulator' % bin_dir
    assert os.path.isfile(simulator_path)


def test_simulator1():
    my_dir = os.path.dirname(__file__)
    bin_dir = os.path.abspath('%s/../../bin' % my_dir)
    simulator_path = '%s/simulator' % bin_dir
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
    completed_process = subprocess.run([simulator_path,
                                        'qopt',
                                        '-m1',
                                        '-eno'],
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE,
                                       input=input)
    process_stdout = completed_process.stdout.decode('utf-8')
    assert process_stdout.startswith(expected_output)


def test_simulator2():
    my_dir = os.path.dirname(__file__)
    bin_dir = os.path.abspath('%s/../../bin' % my_dir)
    simulator_path = '%s/simulator' % bin_dir
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
    completed_process = subprocess.run([simulator_path,
                                        'fifo',
                                        '-m1',
                                        '-eno'],
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE,
                                       input=input)
    process_stdout = completed_process.stdout.decode('utf-8')
    assert process_stdout.startswith(expected_output)
