import pytest
import sys
import hypothesis
import numpy as np
from rosette import rcl
from random import random
import rosette


def randang():
    return 360.0 * random() - 180.0


# for different elec types
def exatype_from_names(res_name3, atom_name):
    return 0


def make_random_pose(seq):
    p = rcl.rosetta.core.pose.Pose()
    rcl.rosetta.core.pose.make_pose_from_sequence(p, seq, 'fa_standard')
    for ir in range(1, p.size() + 1):
        p.set_phi(ir, randang())
        p.set_psi(ir, randang())
        for ic in range(1, p.residue(ir).nchi() + 1):
            p.set_chi(ic, ir, randang())
    return p


def pose_to_rosette(pose):
    Ntot = sum(r.natoms() for r in pose)
    atoms = np.zeros((Ntot, 4), dtype='f8')
    atypes = np.zeros(Ntot, dtype='i4')
    exatypes = np.zeros(Ntot, dtype='i4')
    bonds = np.zeros((Ntot, 4), dtype='i4')
    bonds -= 1
    atoms[:, 3] = 1.0
    aid_to_index = dict()
    count = 0
    for ir, r in enumerate(pose):
        rbegin = count - 1  # indexing correction
        for ia in range(1, r.natoms() + 1):
            atoms[count, 0] = r.xyz(ia).x
            atoms[count, 1] = r.xyz(ia).y
            atoms[count, 2] = r.xyz(ia).z
            atypes[count] = r.atom_type_index(ia)
            exatypes[count] = exatype_from_names(r.name3(), r.atom_name(ia))
            for ib, neighbor in enumerate(r.bonded_neighbor(ia)):
                assert ib <= 3
                bonds[count, ib] = rbegin + neighbor
            if r.has_lower_connect():
                assert ib < 3
                uconn = pose.residue(ir).upper_connect_atom()
                bonds[count, ib] = aid_to_index[ir, uconn]
            if r.has_upper_connect():
                assert ib < 3
                lconn = pose.residue(ir + 2).lower_connect_atom()
                bonds[count, ib] = aid_to_index[ir, lconn]
            aid_to_index[ir + 1, ia] = count
            count += 1

    assert count == Ntot
    return atoms, atypes, exatypes, bonds


def get_scores_from_oracle(pose):
    sfxn = rcl.get_score_function()
    sfxn.score(pose)
    fa_atr = pose.energies().total_energies()[rcl.rosetta.core.scoring.fa_atr]
    fa_rep = pose.energies().total_energies()[rcl.rosetta.core.scoring.fa_rep]
    fa_sol = pose.energies().total_energies()[rcl.rosetta.core.scoring.fa_sol]
    return np.array([fa_atr, fa_rep, fa_sol])


@hypothesis.given(hypothesis.strategies.text(
    alphabet='ACDEFGHIKLMNPQRSTVWY', min_size=1))
@pytest.mark.skipif(not rcl.HAVE_PYROSETTA, reason='no pyrosetta')
def test_rosette_score_safe(seq):
    rcl.init_check('-beta -mute all')
    print('random pose seq:', seq)
    pose = make_random_pose(seq)
    atoms, atypes, exatypes, bonds = pose_to_rosette(pose)
    score = rosette.score.rosette_score_safe(atoms, atypes, exatypes, bonds)
    etype_tot = score.sum(axis=0)
    etype_hat = get_scores_from_oracle(pose)
    np.testing.assert_array_almost_equal(etype_tot, etype_hat)
