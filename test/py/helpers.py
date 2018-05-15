import subprocess


def execute_process(executable, opts, input_bytes):
    completed_process = subprocess.run([executable] + opts,
                                       stdout=subprocess.PIPE,
                                       stderr=subprocess.PIPE,
                                       input=input_bytes)
    retcode = completed_process.returncode
    stdout = completed_process.stdout.decode('utf-8')
    stderr = completed_process.stderr.decode('utf-8')
    return retcode, stdout, stderr


def sorted_opfins(stdout_w_opfins):
    nonempty_lines = stdout_w_opfins.strip().split('\n')
    opfins = [tuple([int(x) for x in line.strip().split()]) for line in nonempty_lines]
    return sorted(opfins)
