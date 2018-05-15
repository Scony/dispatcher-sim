import os

import pytest


@pytest.fixture(scope="session")
def simulator_path():
    my_dir = os.path.dirname(__file__)
    bin_dir = os.path.abspath('%s/../../bin' % my_dir)
    simulator_path = '%s/simulator' % bin_dir
    return simulator_path
