from bisect import bisect_right
import importlib.util
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('divisor_bounds', ROOT/'scripts/divisor_bounds.py')
bounds = importlib.util.module_from_spec(spec)
spec.loader.exec_module(bounds)


class DivisorBoundsTests(unittest.TestCase):
    def test_records_against_exhaustive_divisor_sieve(self):
        limit = 100000
        counts = [0]*(limit+1)
        for d in range(1,limit+1):
            for n in range(d,limit+1,d):
                counts[n] += 1
        expected = []
        best = 0
        for n in range(1,limit+1):
            if counts[n] > best:
                best = counts[n]
                expected.append((n,best))
        self.assertEqual(bounds.divisor_records(limit),expected)
        self.assertEqual(bounds.divisor_records(1),[(1,1)])

    def test_large_records_and_witness_factorizations(self):
        records = bounds.divisor_records(bounds.MAXIMUM)
        values = [n for n,d in records]
        # OEIS A002182 / A002183, indexed pairs for the relevant limits.
        expected = {
            10**9: (735134400,1344),
            10**12: (963761198400,6720),
            10**18: (897612484786617600,103680),
            2**63-1: (9200527969062830400,161280),
            2**64-1: (18401055938125660800,184320),
        }
        for limit,pair in expected.items():
            self.assertEqual(records[bisect_right(values,limit)-1],pair)
        for n,d in records:
            remaining,divisors = n,1
            for p in bounds.PRIMES:
                exponent = 0
                while remaining % p == 0:
                    remaining //= p
                    exponent += 1
                divisors *= exponent+1
            self.assertEqual(remaining,1)
            self.assertEqual(divisors,d)

    def test_distinct_prime_thresholds_and_small_sieve(self):
        primorial = 1
        self.assertEqual(bounds.max_distinct_primes(1),0)
        for k,p in enumerate(bounds.PRIMES,1):
            primorial *= p
            self.assertEqual(bounds.max_distinct_primes(primorial-1),k-1)
            if primorial <= bounds.MAXIMUM:
                self.assertEqual(bounds.max_distinct_primes(primorial),k)
        omega = [0]*10001
        for p in range(2,len(omega)):
            if omega[p] == 0:
                for n in range(p,len(omega),p):
                    omega[n] += 1
        best = 0
        for n in range(1,len(omega)):
            best = max(best,omega[n])
            self.assertEqual(bounds.max_distinct_primes(n),best)

    def test_printed_table_matches_generator(self):
        self.assertEqual((ROOT/'content/extra/divisor-bounds.tex').read_text(),bounds.render())


if __name__ == '__main__':
    unittest.main()
