"""
This module contains test driver code.
"""
import sys
import os
import re

def test():
    """
    This function is the test driver.
    """
    tests = os.listdir('tests/expected')
    for test_case in tests:
        output = os.popen(f'./game < tests/input/{test_case}').read()
        output = re.sub(r'\033\[[0-9]+m', '', output)
        with open(f'tests/expected/{test_case}', 'r', encoding = 'utf-8') as expected:
            if output == expected.read():
                sys.stdout.write(f'{test_case}: ✅\n')
                continue
        sys.stdout.write(f'{test_case}: ⛔️\n')
        with open('tests/result.txt', 'w', encoding = 'utf-8') as result:
            result.write(output)
        os.system(f'diff tests/expected/{test_case} tests/result.txt')

if __name__ == '__main__':
    test()
