from __future__ import unicode_literals

import os
import re

import ttfw_idf


@ttfw_idf.idf_example_test(env_tag='Example_WIFI_Protocols')
def test_examples_icmp_echo(env, extra_data):

    dut = env.get_dut('icmp_echo', 'examples/protocols/icmp_echo')
    dut.start_app()

    dut.expect('example_connect: Connected to')
    dut.expect('esp>')

    ping_dest = os.getenv('EXAMPLE_ICMP_SERVER', 'www.espressif.com')
    dut.write(f'ping {ping_dest}')

    ip_re = r'\.'.join((r'\d{1,3}',) * 4)
    ip = dut.expect(
        re.compile(f'64 bytes from ({ip_re}) icmp_seq=1 ttl=\d+ time=\d+ ms')
    )[0]


    # expect at least one more (there could be lost packets)
    dut.expect(re.compile(f'64 bytes from {ip} icmp_seq=[2-5] ttl=\d+ time='))

    dut.expect(re.compile(r'5 packets transmitted, [2-5] received, \d{1,3}% packet loss'))
    dut.write('')
    dut.expect('esp>')


if __name__ == '__main__':
    test_examples_icmp_echo()
