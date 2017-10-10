import pytest
import sys
import hypothesis
import numpy as np
from rosette import rcl
from random import random
import rosette
import hypothesis.strategies as strat
from hypothesis.extra.numpy import arrays


# for different elec types, needs to be filled in...
def exatype_from_names(res_name3, atom_name):
    return 0


def to_omega(x):
    return (360.0 + x / 10.0) % 360.0 - 180.0


# here's a toy example of hypothesis...
@hypothesis.given(strat.floats(min_value=-180, max_value=180))
def test_to_omega(tor):
    assert -180.0 <= to_omega(tor) <= 180.0


def make_pose_from_seqtor(seq, tor):
    'make_pose_from_sequence and set torsions'
    p = rcl.rosetta.core.pose.Pose()
    rcl.rosetta.core.pose.make_pose_from_sequence(p, seq, 'fa_standard')
    for ir in range(1, p.size() + 1):
        p.set_phi(ir, tor[ir - 1][0])
        p.set_psi(ir, tor[ir - 1][1])
        p.set_psi(ir, to_omega(tor[ir - 1][2]))
        for ic in range(1, p.residue(ir).nchi() + 1):
            p.set_chi(ic, ir, tor[ir - 1][2 + ic])
    return p


# makes an iterator -- beware, you can only loop through once
def pose_iter(pose):
    count = 0
    for ir in range(1, pose.size() + 1):
        r = pose.residue(ir)
        for ia in range(1, r.natoms() + 1):
            yield count, ir, ia, r


# almost certainly not correct....
def pose_to_basic(pose):
    aid_to_index = {(ir, ia): i for i, ir, ia, _ in pose_iter(pose)}
    Ntot = len(aid_to_index)
    atoms = np.zeros((Ntot, 4), dtype='f8')
    atypes = np.zeros(Ntot, dtype='i4')
    exatypes = np.zeros(Ntot, dtype='i4')
    bonds = np.zeros((Ntot, 4), dtype='i4') - 1  # -1 means empty

    for i, ir, ia, r in pose_iter(pose):
        atoms[i, 0] = r.xyz(ia).x
        atoms[i, 1] = r.xyz(ia).y
        atoms[i, 2] = r.xyz(ia).z
        atoms[i, 3] = 1.0
        atypes[i] = r.atom_type_index(ia)
        exatypes[i] = exatype_from_names(r.name3(), r.atom_name(ia))
        for ib, neighbor in enumerate(r.bonded_neighbor(ia)):
            assert ib <= 3
            bonds[i, ib] = aid_to_index[ir, 1] + neighbor - 1
        if r.has_lower_connect():
            assert ib <= 3
            uconn = pose.residue(ir - 1).upper_connect_atom()
            bonds[i, ib] = aid_to_index[ir - 1, uconn]
        if r.has_upper_connect():
            assert ib <= 3
            lconn = pose.residue(ir + 1).lower_connect_atom()
            bonds[i, ib] = aid_to_index[ir + 1, lconn]
    return atoms, atypes, exatypes, bonds


# use pyrosetta as a "test oracle"
def get_scores_from_oracle(pose):
    sfxn = rcl.get_score_function()
    sfxn.score(pose)
    tot_emap = pose.energies().total_energies()
    fa_atr = tot_emap[rcl.core.scoring.fa_atr]
    fa_rep = tot_emap[rcl.core.scoring.fa_rep]
    fa_sol = tot_emap[rcl.core.scoring.fa_sol]
    return np.array([fa_atr, fa_rep, fa_sol])


# ok, this hypothesis thing is awesome... it's called property based testing
# you describe how to generate examples with @given, then it runs the test
# function on cleverly generated cases.. mix of simple and random examples
# then it'll narrow down to the simplest failure case it can. It's fucking
# perfect for cases like this where you have a "test oracle"... in our case
# pyrosetta energys to compare to
# http://hypothesis.works/
# this particular @given generates a list of tuples of (AA, 6_torsion_ang)
# which are used to make pose from sequence then set the phi/psi/chi angles
@hypothesis.given(
    strat.lists(min_size=1, elements=strat.tuples(
        strat.text(min_size=1, max_size=1,
                   alphabet='ACDEFGHIKLMNPQRSTVWY'),
        arrays('f8', (7,), elements=strat.floats(min_value=-180, max_value=180),
               ))))
@pytest.mark.skipif(not rcl.HAVE_PYROSETTA, reason='no pyrosetta')
# @pytest.mark.xfail()  # uncomment this to run but ignore this test
def test_basic_energy_safe(seqtor):
    rcl.init_check('-beta -mute all')
    seq = ''.join(t[0] for t in seqtor)
    tors = [t[1] for t in seqtor]
    pose = make_pose_from_seqtor(seq, tors)
    atoms, atypes, exatypes, bonds = pose_to_basic(pose)
    energy = rosette.energy.basic_energy_safe(atoms, atypes, exatypes, bonds)
    emap = energy.sum(axis=0)
    ehat = get_scores_from_oracle(pose)
    np.testing.assert_array_almost_equal(emap, ehat)
