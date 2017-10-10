import pytest
import os
from os.path import join, dirname, abspath, exists
from rif import rcl


@pytest.fixture(scope='session')
def rootdir():
    d = abspath(dirname(__file__))
    assert exists(d)
    return d


@pytest.fixture(scope='session')
def datadir(rootdir):
    d = join(rootdir, 'data')
    assert exists(d)
    return d


@pytest.fixture(scope='session')
def pdbfname(datadir):
    f = join(datadir, 'pdb', '3uc7A.pdb')
    assert exists(f)
    return f


@pytest.fixture(scope='session')
def cleanpdbfname(datadir):
    f = join(datadir, 'pdb', '3uc7A_clean.pdb')
    assert exists(f)
    return f


@pytest.fixture(scope='session')
def pdbsmallfname(datadir):
    f = join(datadir, 'pdb', 'small.pdb')
    assert exists(f)
    return f


@pytest.fixture(scope='session')
def pose(pdbsmallfname):
    if not rcl.HAVE_PYROSETTA:
        return None
    rcl.init_check(strict=False)
    pose = rcl.pose_from_file(pdbsmallfname)
    return pose


@pytest.fixture(scope='session')
def bigpose(cleanpdbfname):
    if not rcl.HAVE_PYROSETTA:
        return None
    rcl.init_check(strict=False)
    pose = rcl.pose_from_file(cleanpdbfname)
    return pose
