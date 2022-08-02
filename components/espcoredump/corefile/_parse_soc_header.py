"""
This file is used to generate soc header constants into sub-package soc_headers
"""
import os
from ast import literal_eval

from corefile import IDF_PATH, SUPPORTED_TARGETS


def main():  # type: () -> None
    constants = [
        'SOC_DRAM_LOW',
        'SOC_DRAM_HIGH',
        'SOC_IRAM_LOW',
        'SOC_IRAM_HIGH',
        'SOC_RTC_DATA_LOW',
        'SOC_RTC_DATA_HIGH',
        'SOC_RTC_DRAM_LOW',
        'SOC_RTC_DRAM_HIGH',
    ]

    for target in SUPPORTED_TARGETS:
        target_constants = {}
        soc_header_fp = os.path.join(
            IDF_PATH, f'components/soc/{target}/include/soc/soc.h'
        )

        module_fp = os.path.join(
            IDF_PATH,
            'components',
            'espcoredump',
            'corefile',
            'soc_headers',
            f'{target}.py',
        )


        with open(soc_header_fp) as fr:
            for line in fr:
                for attr in constants:
                    if f'#define {attr}' in line:
                        target_constants[attr] = literal_eval(line.strip().split()[-1])

        for attr in constants:
            if attr not in target_constants:
                raise ValueError(f'ERROR: Attr {attr} is missing in {soc_header_fp}')

        with open(module_fp, 'w') as fw:
            for k, v in target_constants.items():
                fw.write(f'{k} = {hex(v)}\n')


if __name__ == '__main__':
    main()
