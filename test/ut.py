# -*- coding: utf-8 -*-
# @Author: tlucanti
# @Date:   2022-02-23 22:42:26
# @Last Modified by:   tlucanti
# @Last Modified time: 2022-02-24 15:29:48

import unittest
from netcat import netcat as Netcat

NOT_REGISTER = ":__TLUCANTI__ 451 * :You need to register to use this command\r\n"
EXTRA_ARGS_UNREG = ":__TLUCANTI__ NOTICE * :extra tokens at the end of command\r\n"
EXTRA_ARGS = ":__TLUCANTI__ NOTICE nc :extra tokens at the end of command\r\n"

netcat = Netcat('0.0.0.0', 8080, True)
netcat2 = Netcat('0.0.0.0', 8080, True)

class UnregTest(unittest.TestCase):

    def setUp(self):
        pass

    """
    RANDOM INVALID TESTS
    """

    def test_unreg_random_command(self):
        cmd = 'hhhoiOhgiGioPGigipgiyGPIyGPYgipyGYIPgiypgypi'
        netcat.send(cmd)
        cmd = cmd.upper()
        self.assertEqual(netcat.recv(), f":__TLUCANTI__ 421 * {cmd} :Command `{cmd}` is unknown\r\n")

    """
    CAP COMMAND TEST
    """

    def test_unreg_cap_no_args(self):
        netcat.send('cap')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_unreg_cap_valid(self):
        netcat.send('CAP LS 302')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_unreg_cap_extra_args(self):
        netcat.send('cap 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), "OK\r\n")

    """
    NICK COMMAND TEST
    """

    def test_unreg_nick_no_args(self):
        netcat.send('NICK')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 431 * :expected nickname\r\n")

    def test_unreg_nick_valid(self):
        netcat.send('nick nc')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_nick_extra_args(self):
        netcat.send('nick 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    PING COMMAND TEST
    """

    def test_unreg_ping_no_args(self):
        netcat.send('PING')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * PING :expected password\r\n")

    def test_unreg_ping_valid(self):
        msg = '9697(&SDG97s&D3^VG235(^GF(73582W32#YFn0&*WF'
        netcat.send(f'PING {msg}')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_ping_extra_args(self):
        netcat.send('ping 1 2 3 4 5 6 7 8')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    PONG COMMAND TEST
    """

    def test_unreg_pong_no_args(self):
        netcat.send('PONG')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_unreg_pong_valid(self):
        netcat.send('PONG :lol')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_unreg_pong_extra_args(self):
        netcat.send('pong 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    OPER COMMAND TEST
    """

    def test_unreg_oper_no_args(self):
        netcat.send('OPER')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * OPER :expected argument\r\n")

    def test_unreg_oper_no_pass(self):
        netcat.send('OPER login')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * OPER :expected password\r\n")

    def test_unreg_oper_invalid_login_pass(self):
        netcat.send('OPER lol kek')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_oper_invalid_login(self):
        netcat.send('OPER lol pass')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_oper_invalid_pass(self):
        netcat.send('OPER tlucanti kek')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_oper_valid(self):
        netcat.send('OPER tlucanti pass')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_unreg_oper_extra_args(self):
        netcat.send('oper 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)


# ======================================================================================================================
# ---------------------------------------------------- PASS + part -----------------------------------------------------
class PassTest(unittest.TestCase):
    """
    PASS COMMAND TEST
    """
    registered = False

    def setUp(self):
        if not PassTest.registered:
            netcat.send('pass pass')
            netcat.recv()
            PassTest.registered = True

    def test_pass_pass_no_args(self):
        netcat.send('PASS')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * PASS :expected password\r\n")

    def test_pass_pass_extra_args(self):
        netcat.send('pass 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    def test_pass_pass_invalid_pass(self):
        netcat.send('pass jhshorsiuhiuIGUIUGLgLiuyGLYgLUyGluVUY')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 464 * :Password incorrect\r\n")

    def test_pass_already_registered_pass(self):
        netcat.send('pass pass')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 462 * :You are already registered\r\n")

    """
    RANDOM INVALID TESTS
    """

    def test_pass_random_command(self):
        cmd = 'hhhoiOhgiGioPGigipgiyGPIyGPYgipyGYIPgiypgypi'
        netcat.send(cmd)
        cmd = cmd.upper()
        self.assertEqual(netcat.recv(), f":__TLUCANTI__ 421 * {cmd} :Command `{cmd}` is unknown\r\n")

    """
    CAP COMMAND TEST
    """

    def test_pass_cap_no_args(self):
        netcat.send('cap')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    def test_pass_cap_valid(self):
        netcat.send('cap LS * 302')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    def test_pass_cap_extra_args(self):
        netcat.send('cap 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    """
    NICK COMMAND TEST
    """

    def test_pass_nick_no_args(self):
        netcat.send('PASS')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * PASS :expected password\r\n")

    def test_pass_nick_too_long(self):
        netcat.send('nick nc10970831085710837817508137508137508137501')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 432 * :nickname too long (max len is 20)\r\n")

    def test_pass_nick_bad_symbols(self):
        netcat.send('nick #%^&*}]`~')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 432 * :nickname can contain only digits, letters or special symbols -[]\\`^{}\r\n")

    def test_pass_nick_extra_args(self):
        netcat.send('nick 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    PING COMMAND TEST
    """
    def test_pass_ping_no_args(self):
        netcat.send('PING')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * PING :expected password\r\n")

    def test_pass_ping_valid(self):
        msg = '9697(&SDG97s&D3^VG235(^GF(73582W32#YFn0&*WF'
        netcat.send(f'PING {msg}')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_pass_ping_extra_args(self):
        netcat.send('ping 1 2 3 4 5 6 7 8')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    PONG COMMAND TEST
    """
    def test_pass_pong_no_args(self):
        netcat.send('PONG')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_pass_pong_valid(self):
        netcat.send('PONG :lol')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_pass_pong_extra_args(self):
        netcat.send('pong 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    """
    OPER COMMAND TEST
    """

    def test_pass_oper_no_args(self):
        netcat.send('OPER')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * OPER :expected argument\r\n")

    def test_pass_oper_no_pass(self):
        netcat.send('OPER login')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * OPER :expected password\r\n")

    def test_pass_oper_invalid_login_pass(self):
        netcat.send('OPER lol kek')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_pass_oper_invalid_login(self):
        netcat.send('OPER lol pass')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_pass_oper_invalid_pass(self):
        netcat.send('OPER tlucanti kek')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_pass_oper_valid(self):
        netcat.send('OPER tlucanti pass')
        self.assertEqual(netcat.recv(), NOT_REGISTER)

    def test_pass_oper_extra_args(self):
        netcat.send('oper 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    # ======================================================================================================================
    # ---------------------------------------------------- NICK + part -----------------------------------------------------
class NickTest(unittest.TestCase):
    """
    PASS COMMAND TEST
    """

    registered = False

    def setUp(self):
        if not NickTest.registered:
            netcat.send('pass pass')
            netcat.recv()
            netcat.send('nick nc')
            netcat.recv()
            NickTest.registered = True

    def test_nick_pass_no_args(self):
        netcat.send('PASS')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 nc PASS :expected password\r\n")

    def test_nick_pass_extra_args(self):
        netcat.send('pass 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS)

    def test_nick_already_registered_pass(self):
        netcat.send('pass pass')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 462 nc :You are already registered\r\n")

    """
    RANDOM INVALID TESTS
    """

    def test_nick_random_command(self):
        cmd = 'hhhoiOhgiGioPGigipgiyGPIyGPYgipyGYIPgiypgypi'
        netcat.send(cmd)
        cmd = cmd.upper()
        self.assertEqual(netcat.recv(), f":__TLUCANTI__ 421 nc {cmd} :Command `{cmd}` is unknown\r\n")

    """
    CAP COMMAND TEST
    """

    def test_nick_cap_no_args(self):
        netcat.send('cap')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    def test_nick_cap_valid(self):
        netcat.send('cap LS * 302')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    def test_nick_cap_extra_args(self):
        netcat.send('cap 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), 'OK\r\n')

    """
    NICK COMMAND TEST
    """

    def test_nick_nick_no_args(self):
        netcat.send('NICK')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 431 nc :expected nickname\r\n")

    def test_nick_nick_too_long(self):
        netcat.send('nick nc10970831085710837817508137508137508137501')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 432 nc :nickname too long (max len is 20)\r\n")

    def test_nick_nick_bad_symbols(self):
        netcat.send('nick #%^&*}]`~')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 432 nc :nickname can contain only digits, letters or some special symbols\r\n")

    def test_nick_nick_same(self):
        netcat.send('nick nc')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_nick_nick_change(self):
        netcat.send('nick __nc__')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_nick_nick_extra_args(self):
        netcat.send('nick 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS)

    def test_nick_nick_collision(self):
        netcat2.send('pass pass')
        self.assertEqual(netcat2.recv(), "OK\r\n")
        netcat2.send('nick nc2')
        self.assertEqual(netcat2.recv(), "OK\r\n")
        netcat.send(netcat.recv(), 'nick nc2')
        self.assertEqual(':__TLUCANTI__ nc nc2 :Nickname is already in use')

    """
    PING COMMAND TEST
    """

    def test_nick_ping_no_args(self):
        self.test_unreg_ping_no_args()

    def test_nick_ping_valid(self):
        self.test_unreg_ping_valid()

    def test_nick_ping_extra_args(self):
        self.test_unreg_ping_extra_args()

    """
    PONG COMMAND TEST
    """

    def test_nick_pong_no_args(self):
        self.test_unreg_pong_no_args()

    def test_nick_pong_valid(self):
        self.test_unreg_pong_valid()

    def test_nick_pong_extra_args(self):
        self.test_unreg_pong_valid()

    """
    OPER COMMAND TEST
    """

    def test_nick_oper_no_args(self):
        self.test_unreg_oper_no_args()

    def test_nick_oper_no_pass(self):
        self.test_unreg_oper_no_pass()

    def test_nick_oper_invalid_login_pass(self):
        self.test_unreg_oper_invalid_login_pass()

    def test_nick_oper_invalid_login(self):
        self.test_unreg_oper_invalid_login()

    def test_nick_oper_invalid_pass(self):
        self.test_unreg_oper_invalid_pass()

    def test_nick_oper_valid(self):
        self.test_unreg_oper_valid()

    def test_nick_oper_extra_args(self):
        self.test_unreg_oper_extra_args()

    # ======================================================================================================================
    # ----------------------------------------------- MAIN (registered) part -----------------------------------------------
class MainTest(unittest.TestCase):
    """
    USER COMMAND TEST
    """

    def test_user_valid(self):
        netcat.send('user tlucanti * 0 :real name')
        resp = netcat.recv()
        self.assertEqual(resp[:4], 'PING')
        resp = resp.strip().split(':')[-1]
        netcat.send(f'pong {resp}')
        self.assertNotEqual(resp, "OK\r\n")
        self.assertTrue(resp[:36], (':__TLUCANTI__ 001 nc :Welcome to the'))

    """
    PASS COMMAND TEST
    """

    def test_pass_no_args(self):
        netcat.send('PASS')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 461 * PASS :expected password\r\n")

    def test_pass_extra_args(self):
        netcat.send('pass 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS_UNREG)

    def test_pass_invalid_pass(self):
        netcat.send('pass jhshorsiuhiuIGUIUGLgLiuyGLYgLUyGluVUY')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 464 * :Password incorrect\r\n")

    def test_already_registered_pass(self):
        netcat.send('pass pass')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 462 * :You are already registered\r\n")

    """
    RANDOM INVALID TESTS
    """

    def test_random_command(self):
        self.test_unreg_random_command()

    """
    CAP COMMAND TEST
    """

    def test_cap_no_args(self):
        self.test_unreg_cap_no_args()

    def test_cap_valid(self):
        self.test_unreg_cap_valid()

    def test_cap_extra_args(self):
        self.test_unreg_cap_extra_args()

    """
    NICK COMMAND TEST
    """

    def test_nick_no_args(self):
        self.test_unreg_nick_no_args()

    def test_nick_too_long(self):
        netcat.send('nick nc10970831085710837817508137508137508137501')
        self.assertEqual(netcat.recv(), ":__TLUCANTI__ 432 * :nickname is too long\r\n")

    def test_nick_bad_symbols(self):
        netcat.send('nick #%^&*}]`~')
        self.assertEqual(netcat.recv(),
            ":__TLUCANTI__ 432 * :nickname can contain only digits, letters or special symbols -[]\\`^{}\r\n")

    def test_nick_valid(self):
        netcat.send('nick nc')
        self.assertEqual(netcat.recv(), "OK\r\n")

    def test_nick_extra_args(self):
        netcat.send('nick 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS)

    def test_nick_collision(self):
        netcat2.send('pass pass')
        self.assertEqual(netcat2.recv(), "OK\r\n")
        netcat2.send('nick nc2')
        self.assertEqual(netcat2.recv(), "OK\r\n")
        netcat.send('nick nc2')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 433 nc nc2 :nickname `nc2` is already in use\r\n')

    """
    PING COMMAND TEST
    """

    def test_ping_no_args(self):
        self.test_unreg_ping_no_args()

    def test_ping_valid(self):
        msg = '9697(&SDG97s&D3^VG235(^GF(73582W32#YFn0&*WF'
        netcat.send(f'PING {msg}')
        self.assertEqual(netcat.recv(), f':__TLUCANTI__ PONG nc :{msg}')

    def test_ping_extra_args(self):
        netcat.send('ping 1 2 3 4 5 6 7 8')
        self.assertEqual(netcat.recv(), EXTRA_ARGS)

    """
    PONG COMMAND TEST
    """

    def test_pong_no_args(self):
        self.test_unreg_pong_no_args()

    def test_pong_valid(self):
        self.test_unreg_pong_valid()

    def test_pong_extra_args(self):
        self.test_unreg_pong_valid()

    """
    OPER COMMAND TEST
    """

    def test_oper_no_args(self):
        netcat.send('OPER')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 461 * :Expected nickname')

    def test_oper_no_pass(self):
        netcat.send('OPER login')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 461 * :Expected password')

    def test_oper_invalid_login_pass(self):
        netcat.send('OPER lol kek')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 464 nc :Password incorrect')

    def test_oper_invalid_login(self):
        netcat.send('OPER lol pass')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 464 nc :Password incorrect')

    def test_oper_invalid_pass(self):
        netcat.send('OPER tlucanti kek')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 464 nc :Password incorrect')

    def test_oper_valid(self):
        netcat.send('OPER tlucanti pass')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 381 nc :You are now an IRC operator')

    def test_oper_already_oper(self):
        netcat.send('OPER tlucanti pass')
        self.assertEqual(netcat.recv(), ':__TLUCANTI__ 381 nc :You are already operator')

    def test_oper_extra_args(self):
        netcat.send('oper 1 2 3 4 5 6 7 8 9')
        self.assertEqual(netcat.recv(), EXTRA_ARGS)


if __name__ == '__main__':
    pass
    # unittest.
    # PassTest()
    # MainTest()
