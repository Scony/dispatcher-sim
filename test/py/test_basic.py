import os


def test_binaries_in_place():
    my_dir = os.path.dirname(__file__)
    bin_dir = os.path.abspath('%s/../../bin' % my_dir)
    simulator_path = '%s/simulator' % bin_dir
    stat_desc_path = '%s/stat-desc' % bin_dir
    assert os.path.isfile(simulator_path)
    assert os.path.isfile(stat_desc_path)
